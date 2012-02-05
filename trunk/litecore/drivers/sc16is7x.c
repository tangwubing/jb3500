#if SC16IS7X_ENABLE
#include <drivers/sc16is7x.h>

//In litecore.c
#if IO_BUF_TYPE == BUF_T_DQUEUE
extern dque dqueue;
#endif


//Private Defines
#define SC16IS7X_LCR 				0x03
#define SC16IS7X_FCR 				0x02
#define SC16IS7X_IIR				0x02
#define SC16IS7X_IER 				0x01
#define SC16IS7X_MCR 				0x04
#define SC16IS7X_MSR 				0x06
#define SC16IS7X_TCR 				0x06
#define SC16IS7X_TLR 				0x07
#define SC16IS7X_RHR 				0x00  //��ģʽ��Ч
#define SC16IS7X_THR 				0x00  //дģʽ��Ч
#define SC16IS7X_LSR 				0x05
#define SC16IS7X_IODir 				0x0A
#define SC16IS7X_IOState 			0x0B
#define SC16IS7X_IOIntEna 			0x0C
#define SC16IS7X_IOControl			0x0E
#define SC16IS7X_RXLVL				0x09
#define SC16IS7X_TXLVL 				0x08

#define SC16IS7X_DLL 				0x00  //����LCR=0x80ʱ�ɷ��ʼĴ���DLL��DHL
#define SC16IS7X_DLH 				0x01

#define SC16IS7X_EFR 				0x02  //����LCR=0xBFʱ�ɷ�����ǿ�ͼĴ���EFR


//Private Macros
#define sc16is7x_Rst(x)					sys_GpioSet(&tbl_bspSc16is7x.tbl[0], x)
#define sc16is7x_Nss(x)					sys_GpioSet(&tbl_bspSc16is7x.tbl[1], x)



//Private Variables
static p_dev_spi sc16is7x_spi;


//Internal Functions
static sys_res sc16is7x_RegWrite(uint_t nReg, uint_t nChl, uint_t nCmd)
{
	sys_res res;

	nReg = (nCmd << 8) | (nReg << 3) | nChl;
	sc16is7x_Nss(0);
	res = spi_Send(sc16is7x_spi, &nReg, 2);
	sc16is7x_Nss(1);
	return res;
}

static uint_t sc16is7x_RegRead(uint_t nReg, uint_t nChl)
{
	uint_t nData = 0;

	nReg = BITMASK(7) | (nReg << 3) | nChl;
	sc16is7x_Nss(0);
	spi_Transce(sc16is7x_spi, nReg, &nData, 1);
	sc16is7x_Nss(1);
	return nData;
}



//-------------------------------------------------------------------------
//External Functions
//-------------------------------------------------------------------------
sys_res sc16is7x_Init()
{
	uint_t i;

	for (i = 0; i < tbl_bspSc16is7x.qty; i++)
		sys_GpioConf(&tbl_bspSc16is7x.tbl[i]);
	while ((sc16is7x_spi = spi_Get(SC16IS7X_COMID, 1000)) == NULL);
	spi_Config(sc16is7x_spi, SPI_SCKIDLE_LOW, SPI_LATCH_1EDGE, SC16IS7X_SPI_SPEED);
	irq_ExtRegister(0, &tbl_bspSc16is7x.tbl[2], IRQ_TRIGGER_FALLING, sc16is7x_ItHandler);
	sc16is7x_Reset();
	return SYS_R_OK;
}



//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
sys_res sc16is7x_Reset()
{

	sc16is7x_Rst(0);
	sys_Delay(200000);
	sc16is7x_Rst(1);
	sys_Delay(500000);

	sc16is7x_RegWrite(SC16IS7X_IODir, 0, 0x1F); 		//����GPIO5-GPIO7Ϊ����, GPIO0 ~ GPIO4Ϊ���
	sc16is7x_RegWrite(SC16IS7X_IOIntEna, 0, 0x00);		//��ֹGPIO5-GPIO7�ж�
	sc16is7x_RegWrite(SC16IS7X_IOControl, 0, 0x01);	//ʹGPIO�������湦��
	return SYS_R_OK;
}


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
sys_res sc16is7x_UartConfig(uint_t nChl, t_uart_para *pPara)
{
	uint_t nTemp;

	nTemp = SC16IS7X_CRYSTAL / pPara->baud / 16;
	sc16is7x_RegWrite(SC16IS7X_LCR, nChl, BITMASK(7)); 	//��������ʹ��
	sc16is7x_RegWrite(SC16IS7X_DLL, nChl, nTemp);
	sc16is7x_RegWrite(SC16IS7X_DLH, nChl, nTemp >> 8);

	if (pPara->data == UART_DATA_7D)
		nTemp = BITMASK(1);
	else
		nTemp = BITMASK(1) | BITMASK(0);
	if (pPara->stop == UART_STOP_2D)
		nTemp |= BITMASK(6);
	switch (pPara->pari) {
		case UART_PARI_EVEN:
			nTemp |= BITMASK(4) | BITMASK(3);
			break;
		case UART_PARI_ODD:
			nTemp |= BITMASK(3);
			break;
		default:
			break;
	}
	sc16is7x_RegWrite(SC16IS7X_LCR, nChl, nTemp);		//�رճ���������,����ͨ�üĴ���
	sc16is7x_RegWrite(SC16IS7X_FCR, nChl, BITMASK(0));	//ʹ��FIFO
	sc16is7x_RegWrite(SC16IS7X_IER, nChl, BITMASK(0)); 	//�����жϣ�����˯��ģʽ����RX��ʱ��RHR�����ر�RTHR�ж�	
	return SYS_R_OK;
}

sys_res sc16is7x_UartSend(uint_t nChl, void *pData, uint_t nLen)
{
	uint_t i, nReg;

	nReg = (SC16IS7X_THR << 3) | nChl;
	for (; nLen; nLen -= i)
		if ((i = MIN(sc16is7x_RegRead(SC16IS7X_TXLVL, nChl), nLen)) != 0) {
			sc16is7x_Nss(0);
			spi_Send(sc16is7x_spi, &nReg, 1);
			spi_Send(sc16is7x_spi, pData, i);
			sc16is7x_Nss(1);
		}
	return SYS_R_OK;
}

void sc16is7x_ItHandler()
{
	p_dev_uart p;
	p_uart_def pDef;
	uint_t i, nLen, nLsr, nRecd, nReg;
	uint8_t aTemp[64];

	for (pDef = tbl_bspUartDef, i = 0; pDef < ARR_ENDADR(tbl_bspUartDef); pDef++, i++) {
		if (pDef->type != UART_T_SC16IS7X)
			continue;
		p = uart_Dev(i);
		//�ж�ʶ��,Ҳ�ɶ�IIRʵ��
		nLsr = sc16is7x_RegRead(SC16IS7X_LSR, pDef->id);
		if (nLsr & BITMASK(0)) {
			//Rx Fifo Not Empty
			nRecd = sc16is7x_RegRead(SC16IS7X_RXLVL, pDef->id);
			for (nReg = BITMASK(7) | (SC16IS7X_RHR << 3) | pDef->id; nRecd; nRecd -= nLen) {
				nLen = MIN(nRecd, sizeof(aTemp));
				sc16is7x_Nss(0);
				if (spi_Transce(sc16is7x_spi, nReg, aTemp, nLen) == SYS_R_OK) {
#if IO_BUF_TYPE == BUF_T_BUFFER
					buf_Push(p->bufrx, aTemp, nLen);
#else
					dque_Push(dqueue, p->parent->id | UART_DQUE_RX_CHL, aTemp, nLen);
#endif
				}
				sc16is7x_Nss(1);
			}
		}
		if (nLsr & 0xF8)
			sc16is7x_RegWrite(SC16IS7X_IER, pDef->id, 0x01);
	}
}

int sc16is7x_GpioRead(uint_t nPin)
{

	return GETBIT(sc16is7x_RegRead(SC16IS7X_IOState, 0), nPin);
}

void sc16is7x_GpioSet(uint_t nPin, uint_t nHL)
{
	uint_t i;

	i = sc16is7x_RegRead(SC16IS7X_IOState, 0);
	if (nHL)
		SETBIT(i, nPin);
	else
		CLRBIT(i, nPin);
	sc16is7x_RegWrite(SC16IS7X_IOState, 0, i);
}


#endif

