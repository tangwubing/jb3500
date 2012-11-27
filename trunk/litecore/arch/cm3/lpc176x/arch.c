


//Internal Functions
static void lpc176x_RccInit()
{

	SystemInit();

	/* Set the Vector Table base location */
	SCB->VTOR  = (BOOTLOADER_SIZE & 0x3FFFFF80);

	/* set pend exception priority */
	NVIC_SetPriority(PendSV_IRQn, (1<<__NVIC_PRIO_BITS) - 1);

	/* init systick */
	SysTick_Config(__CORE_CLK / RT_TICK_PER_SECOND - 1);
}

static void lpc176x_IrqInit()
{

}

static void lpc176x_GpioIdleInit()
{

	//������û��ʹ�õ�IO��Ϊģ������
	//���Խ��͹��ĺ͸���EMC/EMI����
}


#if DEBUG_ENABLE
static void lpc176x_DbgInit()
{

}
#endif

//External Functions
void arch_Init()
{

	//ʱ��ϵͳ��ʼ��
	lpc176x_RccInit();
#if DEBUG_ENABLE
	lpc176x_DbgInit();
#endif
	//�жϳ�ʼ��
	lpc176x_IrqInit();
	//GPIO��ʼ��
	lpc176x_GpioIdleInit();
}


#if IDLE_ENABLE
void arch_IdleEntry()
{

	PWR_EnterSTANDBYMode();
}
#endif


void arch_Reset()
{

	__raw_writel(0x05FA0000 | BITMASK(2), 0xE000ED0C);
}


