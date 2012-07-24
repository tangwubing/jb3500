#include <LPC22xx.H>



//Internal Functions
static void lpc22xx_RccInit()
{

}

#if IRQ_ENABLE
static void lpc22xx_IrqInit()
{

	rt_hw_interrupt_init();

	/* prescaler = 0*/
	T1PR = 0;
	T1PC = 0;

	/* reset and enable MR0 interrupt */
	T1MCR = 0x3;
	T1MR0 = PERI_CLOCK / RT_TICK_PER_SECOND;

	/* enable timer 0 */
	T1TCR = 1;

	/* install timer handler */
	rt_hw_interrupt_install(TIMER1_INT, rt_hw_timer_handler, RT_NULL);
	rt_hw_interrupt_umask(TIMER1_INT);
}
#endif

static void lpc22xx_GpioIdleInit()
{

	//������û��ʹ�õ�IO��Ϊģ������
	//���Խ��͹��ĺ͸���EMC/EMI����
}


#if DEBUG_ENABLE
static void lpc22xx_DbgInit()
{

}
#endif

//External Functions
void arch_Init()
{

	//ʱ��ϵͳ��ʼ��
	lpc22xx_RccInit();
#if DEBUG_ENABLE
	lpc22xx_DbgInit();
#endif
	//�жϳ�ʼ��
#if IRQ_ENABLE
	lpc22xx_IrqInit();
#endif
	//GPIO��ʼ��
	lpc22xx_GpioIdleInit();
}


#if IDLE_ENABLE
void arch_IdleEntry()
{

	PWR_EnterSTANDBYMode();
}
#endif


void arch_Reset()
{

#if OS_TYPE
	os_interrupt_Disable();
#endif
	while(1);
}


