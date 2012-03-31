#include <LPC22xx.H>



//Internal Functions
static void lpc22xx_RccInit()
{

}

static void lpc22xx_IrqInit()
{

	rt_hw_interrupt_init();
}

static void lpc22xx_GpioIdleInit()
{

	//������û��ʹ�õ�IO��Ϊģ������
	//���Խ��͹��ĺ͸���EMC/EMI����
}


#if DEBUG_ENABLE
static void lpc22xx_DbgInit()
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
	lpc22xx_RccInit();
#if DEBUG_ENABLE
	lpc22xx_DbgInit();
#endif
	//�жϳ�ʼ��
	lpc22xx_IrqInit();
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

	__raw_writel(0x05FA0000 | BITMASK(2), 0xE000ED0C);
}


