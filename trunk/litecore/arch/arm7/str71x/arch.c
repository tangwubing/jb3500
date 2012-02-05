#if ARCH_TYPE == ARCH_T_STR71X

static void str7_RccInit()
{
	//ϵͳ������ʼ��
	RCCU_MCLKConfig(RCCU_DEFAULT);							//MCLK = RCLK
	RCCU_FCLKConfig(RCCU_RCLK_2);							//FCLK = RCLK/2
	RCCU_PCLKConfig(RCCU_RCLK_4);							//PCLK = RCLK/4
	RCCU_PLL1Config(RCCU_PLL1_Mul_12, RCCU_Div_2);			//48MHz PLL @ 16MHz XTAL
	
	while (RCCU_FlagStatus(RCCU_PLL1_LOCK) == RESET);		//Wait for PLL to Lock
	RCCU_RCLKSourceConfig(RCCU_PLL1_Output);				//Select PLL for RCLK	
	
}

static void str7_IrqInit()
{

	//�����ⲿ�ж�
	EIC_IRQConfig(ENABLE);
	XTI_Init();
	XTI_ModeConfig(XTI_Interrupt, ENABLE );
	EIC->SIR[XTI_IRQChannel] = ((int)XTI_IRQHandler << 16) | 8;
	EIC_IRQChannelConfig(XTI_IRQChannel, ENABLE);
	
}


void arch_Init()
{

	//ʱ��ϵͳ��ʼ��
	str7_RccInit();
	//�жϳ�ʼ��
	str7_IrqInit();
	//GPIO��ʼ��
	str7_GpioIdleInit();
#if EPI_ENABLE
	str7_EmiInit();
#endif
}



#if DEBUG_ENABLE
void arch_DbgInit()
{

}
#endif


#if IDLE_ENABLE
void arch_IdleEntry()
{

	RCCU->SMR &= ~1;
}
#endif

void arch_Reset()
{

	
}


#endif

