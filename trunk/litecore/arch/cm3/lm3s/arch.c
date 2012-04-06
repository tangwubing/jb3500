


//Internal Functions
static void lm3s_RccInit()
{

	/* set ldo */
	MAP_SysCtlLDOSet(SYSCTL_LDO_2_50V);

	/* set clock */
#if MCU_FREQUENCY == MCU_SPEED_LOW
	MAP_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
#elif MCU_FREQUENCY == MCU_SPEED_HALF
	MAP_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
#else
	MAP_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
#endif

	/* init systick */
	MAP_SysTickDisable();
#if OS_TYPE
	MAP_SysTickPeriodSet(MAP_SysCtlClockGet() / RT_TICK_PER_SECOND);
	MAP_SysTickIntEnable();
	MAP_SysTickEnable();
#endif
}


#if DEBUG_ENABLE
static void lm3s_DbgInit()
{

}
#endif

static void lm3s_IrqInit()
{

	__raw_writel(BOOTLOADER_SIZE, NVIC_VTABLE);
	MAP_IntMasterEnable();
}

static void lm3s_GpioIdleInit()
{

	//������û��ʹ�õ�IO��Ϊģ������
	//���Խ��͹��ĺ͸���EMC/EMI����
}


//External Functions
void arch_Init()
{

	//ʱ��ϵͳ��ʼ��
	lm3s_RccInit();
#if DEBUG_ENABLE
	lm3s_DbgInit();
#endif
	//�жϳ�ʼ��
	lm3s_IrqInit();
	//GPIO��ʼ��
	lm3s_GpioIdleInit();
#if EPI_ENABLE && !EPI_SOFTWARE
	lm3s_EpiInit();
#endif
}

#if IDLE_ENABLE
void arch_IdleEntry()
{


}
#endif


void arch_Reset()
{

	MAP_SysCtlReset();
}


