#if UART_ENABLE



//In litecore.c
#if IO_BUF_TYPE == BUF_T_DQUEUE
extern dque dqueue;
#endif

//Private Defines
#define IER_RBR		0x01
#define IER_THRE	0x02
#define IER_RLS		0x04

#define IIR_PEND	0x01

#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80


//Private Typedef



//Private Const
static LPC_UART_TypeDef * const lpc22xx_tblUartBase[] = {
	LPC_UART0,
	LPC_UART1,
};

//Private Variables
static p_dev_uart lpc22xx_uart_dev[2];




//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void arch_UartInit(p_dev_uart p)
{
	t_uart_para xUart;
	uint_t nMode;

	lpc22xx_uart_dev[p->def->id] = p;

	/* enable UART clock and interrupt*/
	switch (p->def->id) {
	case 0:
		rt_hw_interrupt_install(UART0_INT, UART0_IRQHandler, RT_NULL);
		rt_hw_interrupt_umask(UART0_INT);
		break;
	case 1:
		rt_hw_interrupt_install(UART1_INT, UART1_IRQHandler, RT_NULL);
		rt_hw_interrupt_umask(UART1_INT);
		break;
	default:
		break;
	}

	/* set UART pin */
	arch_GpioConf(p->def->rxport, p->def->rxpin, GPIO_M_IN_PU, GPIO_INIT_HIGH);
	if (p->def->pinmode == DEV_PIN_PP)
		nMode = GPIO_M_OUT_PP;
	else
		nMode = GPIO_M_OUT_OD;
	arch_GpioConf(p->def->txport, p->def->txpin, nMode, GPIO_INIT_HIGH);

	arch_GpioSel(p->def->rxport, p->def->rxpin, 1);
	arch_GpioSel(p->def->txport, p->def->txpin, 1);
	xUart.baud = 4800;
	xUart.pari = UART_PARI_NO;
	xUart.data = UART_DATA_8D;
	xUart.stop = UART_STOP_1D;
	arch_UartOpen(p->def->id, &xUart);
}


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
sys_res arch_UartOpen(uint_t nId, p_uart_para pPara)
{
	LPC_UART_TypeDef *pUart = lpc22xx_tblUartBase[nId];
	uint_t nDiv, nMode = 0;
	p_dev_uart p;

	p = lpc22xx_uart_dev[nId];
	switch (pPara->stop) {
	case UART_STOP_2D:
		SETBIT(nMode, 2);
		break;
	default:
		break;
	}
	switch (pPara->pari) {
	case UART_PARI_EVEN:
		nMode |= (BITMASK(3) | BITMASK(4));
		break;
	case UART_PARI_ODD:
		nMode |= BITMASK(3);
		break;
	default:
		break;
	}
	switch (pPara->data) {
	case UART_DATA_7D:
		nMode |= BITMASK(1);
		break;
	default:
		nMode |= (BITMASK(0) | BITMASK(1));
		break;
	}
	nDiv = (PERI_CLOCK / 16) / pPara->baud;	/*baud rate */
	pUart->LCR = 0x80;		/* DLAB = 1*/
	pUart->DLM = nDiv / 256;
	pUart->DLL = nDiv % 256;
	if (pPara->baud > 57600)
		pUart->FDR = 0xF1;
	pUart->LCR = nMode; 	/* DLAB = 0*/
	pUart->FCR = 0x07;		/* Enable and reset TX and RX FIFO. */

	while ((pUart->LSR & (LSR_THRE | LSR_TEMT)) != (LSR_THRE | LSR_TEMT));
	pUart->IER = IER_RLS;	/* Enable UART interrupt */
	if (p->def->rxmode == UART_MODE_IRQ)
		pUart->IER |= IER_RBR;

	return SYS_R_OK;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void arch_UartTxIEnable(uint_t nId)
{
	LPC_UART_TypeDef *pUart = lpc22xx_tblUartBase[nId];

	pUart->IER |= IER_THRE;
	arch_UartISR(nId);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void arch_UartScReset(uint_t nId, uint_t nHL)
{

}


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void arch_UartSend(uint_t nId, const void *pData, uint_t nLen)
{
	LPC_UART_TypeDef *pUart = lpc22xx_tblUartBase[nId];
	uint8_t *pBuf = (uint8_t *)pData;

	for (; nLen; nLen--) {
		/* THRE status, contain valid data */
		while ((pUart->LSR & LSR_THRE) == 0);
		/* write data */
		pUart->THR = *pBuf++;
	}
	while ((pUart->LSR & LSR_THRE) == 0);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
int arch_UartGetChar(uint_t nId)
{
	LPC_UART_TypeDef *pUart = lpc22xx_tblUartBase[nId];

	while ((pUart->LSR & LSR_RDR) == 0);
	return pUart->RBR;
}



//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void arch_UartISR(uint_t nId)
{
	LPC_UART_TypeDef *pUart = lpc22xx_tblUartBase[nId];
	p_dev_uart p;
	uint8_t aTemp[16], *pTemp = aTemp;
	int i;

	p = lpc22xx_uart_dev[nId];

	if ((pUart->IIR & IIR_PEND) == 0) {
		while (pUart->LSR & LSR_RDR) {
	        *pTemp++ = pUart->RBR;
			if (pTemp >= ARR_ENDADR(aTemp)) {
#if IO_BUF_TYPE == BUF_T_BUFFER
				buf_Push(p->bufrx, aTemp, sizeof(aTemp));
#elif IO_BUF_TYPE == BUF_T_DQUEUE
				dque_Push(dqueue, p->parent->id | UART_DQUE_RX_CHL, aTemp, sizeof(aTemp));
#endif
				pTemp = aTemp;
			}
	    }
		if (pTemp > aTemp) {
#if IO_BUF_TYPE == BUF_T_BUFFER
			buf_Push(p->bufrx, aTemp, pTemp - aTemp);
#elif IO_BUF_TYPE == BUF_T_DQUEUE
			dque_Push(dqueue, p->parent->id | UART_DQUE_RX_CHL, aTemp, pTemp - aTemp);
#endif
		}
	}

	if (pUart->IER & IER_THRE) {
#if IO_BUF_TYPE == BUF_T_BUFFER
		while (pUart->LSR & LSR_THRE) {
			if (p->buftx->len == 0) {
				pUart->IER &= ~IER_THRE;
				break;
			}
			for (i = 0; i < p->buftx->len; i++) {
				if ((pUart->LSR & LSR_THRE) == 0)
					break;
				pUart->THR = p->buftx->p[i];
			}
			buf_Remove(p->buftx, i);
		}
#elif IO_BUF_TYPE == BUF_T_DQUEUE
		while (pUart->LSR & LSR_THRE) {
			i = dque_PopChar(dqueue, p->parent->id | UART_DQUE_TX_CHL);
			if (i < 0) {
				pUart->IER &= ~IER_THRE;
				break;
			}
			pUart->THR = i;
		}
#endif
	}
}

#endif

