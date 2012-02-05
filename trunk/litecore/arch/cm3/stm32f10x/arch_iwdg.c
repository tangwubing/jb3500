

/*******************************************************************************
* Function Name  : stm32_WdgInit
* Description    : Ƭ�ڶ������Ź�����
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
void arch_WdgInit()
{

	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	/* IWDG counter clock: 30-60KHz(LSI) / 64 = 469-937.5Hz */
	IWDG_SetPrescaler(IWDG_Prescaler_64);
	/* Set counter reload value to 1800 */
	IWDG_SetReload(1800);
	/* Reload IWDG counter */
	IWDG_ReloadCounter();
	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	IWDG_Enable();
}

