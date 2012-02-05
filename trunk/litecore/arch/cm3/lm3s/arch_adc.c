


void arch_AdcInit(void)
{

	// ʹ��AIN4/PD3���ڶ˿�
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

	// ����PD3ΪAD����
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_3);
  
	// ʹ��ADCʱ�� 
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	
	//����ADC��������
	SysCtlADCSpeedSet(SYSCTL_ADCSPEED_125KSPS);
  
	// ���ò������з�����3Ϊ�����������Ϊ������ȼ�							
	ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);  
															  
	// ���ò������з�����3��ͨ��0��ת������ж�ʹ�ܣ�
	// �������һ��ת��ͨ��
	ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH4 | ADC_CTL_IE | ADC_CTL_END);
										
	// ����AD�Ĳ������з�����3
	ADCSequenceEnable(ADC0_BASE, 0); 
  
	// ����жϱ�־		
	ADCIntClear(ADC0_BASE, 0);
}

float arch_AdcData()
{
	uint_t nRes; 

	// ����ADCת��
	ADCProcessorTrigger(ADC0_BASE, 0);

	// �ȴ�ADCת�����
	while(!ADCIntStatus(ADC0_BASE, 0, false));

	// ��ȡADC��ֵ
	ADCSequenceDataGet(ADC0_BASE, 0, &nRes);

	return (float)nRes;
}

