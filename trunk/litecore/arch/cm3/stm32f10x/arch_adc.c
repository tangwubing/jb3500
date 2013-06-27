#if OS_TYPE

void arch_AdcInit()
{

 	/* Enable ADC3 clock                                                        */
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
}


uint_t arch_AdcData(uint_t nPort, uint_t nPin)
{
	uint_t i, nSum;

	ADC_InitTypeDef ADC_InitStructure_ADC;

	ADC_InitStructure_ADC.ADC_Mode = ADC_Mode_Independent; 					// ����ADC1 �����ڶ���ģʽ
	ADC_InitStructure_ADC.ADC_ScanConvMode = ENABLE;						// ����ADC1 ģ��ת��������ɨ��ģʽ(��ͨ��ģʽ)
	ADC_InitStructure_ADC.ADC_ContinuousConvMode = ENABLE; 					// ����ADC1 ģ��ת������������ģʽ
	ADC_InitStructure_ADC.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	// ����ADC1 ģ��ת���������ʽ���������жϷ�ʽ
	ADC_InitStructure_ADC.ADC_DataAlign = ADC_DataAlign_Right; 				// ����ADC1 ģ��ת�����ݶ��뷽ʽΪ�Ҷ���
	ADC_InitStructure_ADC.ADC_NbrOfChannel = 1;								// ����ADC1 ģ��ת����ͨ����Ŀ Ϊ 1��ͨ��
	ADC_Init(ADC3, &ADC_InitStructure_ADC);									// ����ADC1 ��ʼ��

	ADC_RegularChannelConfig(ADC3, ADC_Channel_4, 1, ADC_SampleTime_239Cycles5);//����ADC1 �Ĺ���ͨ����ת��˳��Ͳ���ʱ��

	/* Enable ADC1 ------------------------------------------------------*/
	ADC_Cmd(ADC3, ENABLE);
	/* Enable ADC1 reset calibration register */
	ADC_ResetCalibration(ADC3);
	/* Check the end of ADC1 reset calibration register */
	while (ADC_GetResetCalibrationStatus(ADC3));
	/* Start ADC1 calibration */
	ADC_StartCalibration(ADC3);
	/* Check the end of ADC1 calibration */
	while (ADC_GetCalibrationStatus(ADC3));
 	/* Start ADC1 Software Conversion */ 
// 	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	nSum = 0;
	for (i = 0; i < 5; i++) {
		os_thd_Sleep(10);
		nSum += (ADC3->DR & 0x0FFF);
	}
	nSum /= 5;	
	return nSum;
}

#endif

