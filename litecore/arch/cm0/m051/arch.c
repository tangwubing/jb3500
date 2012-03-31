



//Internal Functions
static void m051_RccInit()
{

	UNLOCKREG();

    SYSCLK->PWRCON.XTL12M_EN = 1;
    /* Waiting for 12M Xtal stalble */
	while (SYSCLK->CLKSTATUS.XTL12M_STB == 0);

	/* PLL Enable */
	SYSCLK->PLLCON.PD = 0;
	SYSCLK->PLLCON.OE = 0;
	/* Waiting for PLL stalble */
	while (SYSCLK->CLKSTATUS.PLL_STB == 0);

	/* Select PLL as system clock */
	SYSCLK->CLKSEL0.HCLK_S = 2;

	LOCKREG();

	/* init systick */
	SysTick_Config(XTAL / RT_TICK_PER_SECOND - 1);
}

static void m051_IrqInit()
{

	/* set pend exception priority */
	NVIC_SetPriority(PendSV_IRQn, (1 << __NVIC_PRIO_BITS) - 1);
}

static void m051_GpioIdleInit()
{

	//������û��ʹ�õ�IO��Ϊģ������
	//���Խ��͹��ĺ͸���EMC/EMI����
}


#if DEBUG_ENABLE
static void m051_DbgInit()
{

	DBGMCU_Config(0x00000127, ENABLE);
	*(vu32 *)0xE0000FB0 = 0xC5ACCE55;
	*(vu32 *)0xE0000E80 = 0x00010017;
	*(vu32 *)0xE0000E40 = 0x00000081;
	*(vu32 *)0xE0000E00 = 0x80000001;
	*(vu32 *)0xE00400F0 = 0x00000002;
	*(vu32 *)0xE0040304 = 0x00000102;
}
#endif

//External Functions
void arch_Init()
{

	//ʱ��ϵͳ��ʼ��
	m051_RccInit();
#if DEBUG_ENABLE
	m051_DbgInit();
#endif
	//�жϳ�ʼ��
	m051_IrqInit();
	//GPIO��ʼ��
	m051_GpioIdleInit();
}


#if IDLE_ENABLE
void arch_IdleEntry()
{

}
#endif


void arch_Reset()
{

}


