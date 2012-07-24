#if IRQ_TIMER_ENABLE



//Private Const
static LPC_TIM_TypeDef * const lpc22xx_tblTimerBase[] = {
	LPC_TIM0,
	LPC_TIM1,
};




void arch_TimerInit(uint_t nId)
{

	switch (nId) {
	case 0:
		rt_hw_interrupt_install(TIMER0_INT, TIMER0_IRQHandler, RT_NULL);
		rt_hw_interrupt_umask(TIMER0_INT);
		break;
	case 1:
		rt_hw_interrupt_install(TIMER1_INT, TIMER1_IRQHandler, RT_NULL);
		rt_hw_interrupt_umask(TIMER1_INT);
		break;
	default:
		break;
	}
}

void arch_TimerIntClear(uint_t nId)
{
	LPC_TIM_TypeDef *pTimer = lpc22xx_tblTimerBase[nId];

	pTimer->IR = 0xFF;
}

void arch_TimerStart(uint_t nId, uint_t nValue)
{
	LPC_TIM_TypeDef *pTimer = lpc22xx_tblTimerBase[nId];

	pTimer->TCR = BITMASK(1);
	pTimer->TC = 0;
	pTimer->IR = 0xFF;
	pTimer->PR = 0;
	pTimer->PC = 0;
	pTimer->MR0 = nValue;
	pTimer->MCR = BITMASK(0) | BITMASK(1);
	pTimer->CCR0 = 0x0C00;
	pTimer->TCR = BITMASK(0);
}

void arch_TimerStop(uint_t nId)
{
	LPC_TIM_TypeDef *pTimer = lpc22xx_tblTimerBase[nId];

	pTimer->TCR = 0;
}

uint_t arch_TimerClockGet()
{

	return PERI_CLOCK;
}

void arch_TimerCapConf(uint_t nPort, uint_t nPin)
{

	arch_GpioSel(nPort, nPin, 2);
}

#if 0
void arch_TimerCapStart(uint_t nId)
{
	LPC_TIM_TypeDef *pTimer = lpc22xx_tblTimerBase[nId];

	pTimer->TCR = BITMASK(1);
	pTimer->TC = 0;
	pTimer->IR = 0xFF;
	pTimer->PR = 0;
	pTimer->PC = 0;
	pTimer->CCR0 = 0x0C00;
	pTimer->TCR = BITMASK(0);
}
#else
void arch_TimerCapStart(uint_t nId, uint_t nValue)
{
//	LPC_TIM_TypeDef *pTimer = lpc22xx_tblTimerBase[nId];

  	PINSEL1 &= 0xf0ffffff;
  	PINSEL1 |= 0x08000000;                  	//�趨TXD���Ź���CPIO��RXD���Ź����ڲ�׽(3)ģʽ
  	IO0DIR |= 1<<28;
 	IO0SET |= 1<<28;                         	//��ʼ��TXD����
	
  	T0TC=0;
  	T0TCR=0x03;
  	T0TCR=0;
  	T0PR=0;
  	T0PC=0;                                 	//����Ƶ
  	T0CCR=0xc00;                            	//�趨��׽CAP3�½��ز��ж�
  	T0MR1=nValue / 3;
  	T0MR2=nValue;
  	T0IR=0xff;
/*
  	VICIntSelect = 0x00000000;           		// ��������ͨ��ΪIRQ�ж�
  	VICVectCntl5 = 0x24;                 		// �����ж�ͨ�����䵽IRQ slot 1��
  	VICVectAddr5 = (uint32)IRQ_SimuUart;		// ��������������ַ
  	VICIntEnable |= 0x00000010;           		// ʹ���ж�
*/
  	T0EMR=0;
  	//T0TCR=0x01;
	

/*
	pTimer->TC = 0;
	pTimer->TCR = 0x03;
	pTimer->TCR = 0;
	pTimer->PR = 0;
	pTimer->PC = 0;
	pTimer->CCR0 = 0x0C00;						//�趨��׽CAPn.3�½��ز��ж�
  	pTimer->MR1 = pSW->tick / 2;				//?
  	pTimer->MR2= pSW->tick;						//?
	pTimer->IR = 0xFF;
*/
/*  	
	VICIntSelect = 0x00000000;           		// ��������ͨ��ΪIRQ�ж�
  	VICVectCntl5 = 0x24;                 		// �����ж�ͨ�����䵽IRQ slot 1��
  	VICVectAddr5 = (uint32_t)pHandler;			// ��������������ַ
  	VICIntEnable |= 0x00000010;           		// ʹ���ж�
*/
//  	pTimer->EMR=0;                                    
}
#endif

#endif
