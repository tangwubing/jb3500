#include <LPC22xx.H>



//Internal Functions
#define MAX_HANDLERS	32
#define SVCMODE		    0x13


#if OS_TYPE

extern volatile rt_uint8_t rt_interrupt_nest;

/**
 * @addtogroup LPC22xx
 */
/*@{*/

/**
 * This function will initialize thread stack
 *
 * @param tentry the entry of thread
 * @param parameter the parameter of entry
 * @param stack_addr the beginning stack address
 * @param texit the function will be called when thread exit
 *
 * @return stack address
 */
rt_uint8_t *rt_hw_stack_init(void *tentry, void *parameter,
	rt_uint8_t *stack_addr, void *texit)
{
	unsigned long *stk;

	stk 	 = (unsigned long *)stack_addr;
	*(stk) 	 = (unsigned long)tentry;		/* entry point */
	*(--stk) = (unsigned long)texit;		/* lr */
	*(--stk) = 0;							/* r12 */
	*(--stk) = 0;							/* r11 */
	*(--stk) = 0;							/* r10 */
	*(--stk) = 0;							/* r9 */
	*(--stk) = 0;							/* r8 */
	*(--stk) = 0;							/* r7 */
	*(--stk) = 0;							/* r6 */
	*(--stk) = 0;							/* r5 */
	*(--stk) = 0;							/* r4 */
	*(--stk) = 0;							/* r3 */
	*(--stk) = 0;							/* r2 */
	*(--stk) = 0;							/* r1 */
	*(--stk) = (unsigned long)parameter;	/* r0 : argument */

	/* cpsr */
	if ((rt_uint32_t)tentry & 0x01)
		*(--stk) = SVCMODE | 0x20;			/* thumb mode */
	else
		*(--stk) = SVCMODE;					/* arm mode   */

	/* return task's current stack address */
	return (rt_uint8_t *)stk;
}

/* exception and interrupt handler table */
rt_uint32_t rt_interrupt_from_thread, rt_interrupt_to_thread;
rt_uint32_t rt_thread_switch_interrupt_flag;

void rt_hw_interrupt_handler(int vector)
{
	rt_kprintf("Unhandled interrupt %d occured!!!\n", vector);
}

/**
 * This function will initialize hardware interrupt
 */
void rt_hw_interrupt_init()
{
	rt_base_t index;
	rt_uint32_t *vect_addr, *vect_ctl;

	/* initialize VIC*/
	VICIntEnClr = 0xffffffff;
	VICVectAddr = 0;
	/* set all to IRQ */
	VICIntSelect = 0;

	for (index = 0; index < MAX_HANDLERS; index ++)
	{
		vect_addr 	= (rt_uint32_t *)(VIC_BASE_ADDR + 0x100 + (index << 2));
		vect_ctl 	= (rt_uint32_t *)(VIC_BASE_ADDR + 0x200 + (index << 2));

		*vect_addr 	= (rt_uint32_t)rt_hw_interrupt_handler;
		*vect_ctl 	= 0xF;
	}

	/* init interrupt nest, and context in thread sp */
	rt_interrupt_nest = 0;
	rt_interrupt_from_thread = 0;
	rt_interrupt_to_thread = 0;
	rt_thread_switch_interrupt_flag = 0;
}

/**
 * This function will mask a interrupt.
 * @param vector the interrupt number
 */
void rt_hw_interrupt_mask(int vector)
{
	VICIntEnClr = (1 << vector);
}

/**
 * This function will un-mask a interrupt.
 * @param vector the interrupt number
 */
void rt_hw_interrupt_umask(int vector)
{
	VICIntEnable = (1 << vector);
}

/**
 * This function will install a interrupt service routine to a interrupt.
 * @param vector the interrupt number
 * @param new_handler the interrupt service routine to be installed
 * @param old_handler the old interrupt service routine
 */
void rt_hw_interrupt_install(int vector, rt_isr_handler_t new_handler, rt_isr_handler_t *old_handler)
{
	if(vector >= 0 && vector < MAX_HANDLERS)
	{
		/* get VIC address */
		rt_uint32_t* vect_addr 	= (rt_uint32_t *)(VIC_BASE_ADDR + 0x100 + (vector << 2));
		rt_uint32_t* vect_ctl 	= (rt_uint32_t *)(VIC_BASE_ADDR + 0x200 + (vector << 2));

		/* assign IRQ slot and enable this slot */
		*vect_ctl = 0x20 | (vector & 0x1F);

		if (old_handler != RT_NULL) *old_handler = (rt_isr_handler_t) *vect_addr;
		if (new_handler != RT_NULL) *vect_addr = (rt_uint32_t) new_handler;
	}
}

/**
 * this function will reset CPU
 *
 */
void rt_hw_cpu_reset()
{
}

/**
 * this function will shutdown CPU
 *
 */
void rt_hw_cpu_shutdown()
{
	rt_kprintf("shutdown...\n");

	while (1);
}

void rt_hw_trap_irq()
{
	rt_isr_handler_t isr_func;

	isr_func = (rt_isr_handler_t) VICVectAddr;
	isr_func(0);

	/* acknowledge Interrupt */
	VICVectAddr = 0;
}

void rt_hw_trap_fiq()
{
    rt_kprintf("fast interrupt request\n");
}

/**
 * This is the timer interrupt service routine.
 * @param vector the irq number for timer
 */
void rt_hw_timer_handler(int vector)
{
	rt_tick_increase();

	/* clear interrupt flag */
	T1IR |= 0x01;
}
#endif


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

	os_interrupt_Disable();
	while(1);
}


