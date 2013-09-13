#if ATT7022_ENABLE
#include <drivers/att7022.h>


//Private Defines
#define ATT7022_PHASECALI_ENABLE	0

#define ATT7022_SAMPLEPOINT   		128
#define ATT7022_DATA_MASK			0x00FFFFFF
#define ATT7022_DATA_DEC			8192.0f

#define ATT7022_SPI_SPEED			200000



//Private Variables
static t_att7022 att7022;


//Private Macros
#define att7022_Sig()				sys_GpioRead(gpio_node(tbl_bspAtt7022, 1))

#define att7022_WriteEnable(p)		att7022_WriteReg(p, ATT7022_CALIDATA_WEN, 0)
#define att7022_WriteDisable(p)		att7022_WriteReg(p, ATT7022_CALIDATA_WEN, 1)
#define att7022_CaliClear(p)		att7022_WriteReg(p, ATT7022_CALIDATA_CLR, 0)
#define	att7022_SWRST(p)			att7022_WriteReg(p, ATT7022_SOFTRST_CMD, 0)









//Internal Functions
#if ATT7022_DEBUG_ENABLE
#define att7022_DbgOut				dbg_printf
#else
#define att7022_DbgOut(...)
#endif



static p_dev_spi att7022_SpiGet()
{
	p_dev_spi p;
	
	p = spi_Get(ATT7022_COMID, OS_TMO_FOREVER);
	spi_Config(p, SPI_SCKIDLE_LOW, SPI_LATCH_2EDGE, ATT7022_SPI_SPEED);
#if SPI_SEL_ENABLE
	spi_CsSel(p, ATT7022_CSID);
#endif
	return p;
}


static sys_res att7022_WriteReg(p_att7022 p, uint_t nReg, uint32_t nData)
{
	uint32_t nCrc1, nCrc2, nTemp;

	p->spi = att7022_SpiGet();

	//д���ݼĴ���
	nData &= ATT7022_DATA_MASK;
	reverse(&nData, 3);
	nTemp = nReg | 0x80 | (nData << 8);
	spi_Send(p->spi, &nTemp, 4);
	os_thd_Slp1Tick();
	//��У��1�Ĵ���
	nTemp = ATT7022_REG_WSPIData1;
	spi_Transce(p->spi, &nTemp, 1, &nCrc1, 3);
	nCrc1 &= ATT7022_DATA_MASK;
	//��У��2�Ĵ���
#if 0
	nTemp = ATT7022_REG_WSPIData2;
	spi_Transce(p->spi, &nTemp, 1, &nCrc2, 3);
	nCrc2 &= ATT7022_DATA_MASK;
#else
	nCrc2 = nCrc1;
#endif

	spi_Release(p->spi);

	if ((nData != nCrc1) || (nData != nCrc2)) {
		att7022_DbgOut("<ATT7022>WriteReg %02X Err %x %x %x", nReg, nData, nCrc1, nCrc2);
		return SYS_R_ERR;
	}
	return SYS_R_OK;
}

static uint32_t att7022_PhaseCaliData(float err)
{
	uint32_t phase_v = 0;
	float seta;

	if (err) {
		seta = acos((1 + err ) * 0.5);
		seta -= 3.14 / 3;
		if (seta >= 0)
			phase_v = seta * MAX_VALUE1;
		else
			phase_v = MAX_VALUE2 + seta * MAX_VALUE1;
	}	
	return phase_v;
}







//External Functions
void att7022_Init()
{
	p_gpio_def pDef;

	for (pDef = tbl_bspAtt7022[0]; pDef < tbl_bspAtt7022[1]; pDef++)
		sys_GpioConf(pDef);
}

uint32_t att7022_ReadReg(p_att7022 p, uint_t nReg)
{
	uint32_t nData, nCrc;

	p->spi = att7022_SpiGet();

	//�����ݼĴ���
	spi_Transce(p->spi, &nReg, 1, &nData, 3);
	os_thd_Slp1Tick();
	reverse(&nData, 3);
	nData &= ATT7022_DATA_MASK;
	//��У��Ĵ���	
	nReg = ATT7022_REG_RSPIData;
	spi_Transce(p->spi, &nReg, 1, &nCrc, 3);
	reverse(&nCrc, 3);
	nCrc &= ATT7022_DATA_MASK;

	spi_Release(p->spi);

	if (nData != nCrc) {
		att7022_DbgOut("<ATT7022>ReadReg %02X Err %x %x", nReg, nData, nCrc);
		nData = 0;
	}
	return nData;
}


sys_res att7022_Reset(p_att7022 p, p_att7022_cali pCali)
{
	uint32_t nTemp;
	uint_t i;

	//��λATT7022B
	sys_GpioSet(gpio_node(tbl_bspAtt7022, 0), 1);
	sys_Delay(3000);				//delay 30us
	sys_GpioSet(gpio_node(tbl_bspAtt7022, 0), 0);
	os_thd_Slp1Tick();

	for (i = 0; i < 3; i++) {
		if (att7022_WriteEnable(p) == SYS_R_OK)
			break;
	}
	if (i >= 3)
		return SYS_R_ERR;

	att7022_CaliClear(p);

	att7022_WriteReg(p, ATT7022_REG_UADCPga, 0);			//��ѹͨ��ADC��������Ϊ1
	att7022_WriteReg(p, ATT7022_REG_HFConst, pCali->HFConst);	//����HFConst
	nTemp = IB_VO * ISTART_RATIO * CONST_G * MAX_VALUE1;
	att7022_WriteReg(p, ATT7022_REG_Istartup, nTemp);		//������������
	att7022_WriteReg(p, ATT7022_REG_EnUAngle, 0x003584);	//ʹ�ܵ�ѹ�нǲ���
	att7022_WriteReg(p, ATT7022_REG_EnDtIorder, 0x005678);	//ʹ��������
	att7022_WriteReg(p, ATT7022_REG_EAddMode, 0);			//���������ۼ�ģʽ	
	att7022_WriteReg(p, ATT7022_REG_GCtrlT7Adc, 0); 		//�ر��¶Ⱥ�ADC7���� 	
	att7022_WriteReg(p, ATT7022_REG_EnlineFreq, 0x007812);	//����/г����������
	att7022_WriteReg(p, ATT7022_REG_EnHarmonic, 0x0055AA);	//����/г����������

	//д�빦������
	for (i = 0; i < 3; i++) {
		att7022_WriteReg(p, ATT7022_REG_PgainA0 + i, pCali->Pgain0[i]);
		att7022_WriteReg(p, ATT7022_REG_PgainA1 + i, pCali->Pgain1[i]);
	}

	//д���ѹ����
	for (i = 0; i < 3; i++) {
		att7022_WriteReg(p, ATT7022_REG_UgainA + i, pCali->Ugain[i]);
	}

	//д���������
	for (i = 0; i < 3; i++) {
		att7022_WriteReg(p, ATT7022_REG_IgainA + i, pCali->Igain[i]);
	}

	//д����λ������
	for (i = 0; i < 5; i++) {
		att7022_WriteReg(p, ATT7022_REG_PhsregA0 + i, pCali->PhsregA[i]);
		att7022_WriteReg(p, ATT7022_REG_PhsregB0 + i, pCali->PhsregB[i]);
		att7022_WriteReg(p, ATT7022_REG_PhsregC0 + i, pCali->PhsregC[i]);
	}

	return att7022_WriteDisable(p);
}

//------------------------------------------------------------------------
//��	��:  att7022_GetFreq ()
//��	��: 
//��	��:  -
//��	��:  -
//��	��:  Ƶ��ֵ
//��	��:  ��ȡƵ��ֵ
//------------------------------------------------------------------------
float att7022_GetFreq(p_att7022 p)
{
	sint32_t nData;

	nData = att7022_ReadReg(p, ATT7022_REG_Freq);
	if (nData != (-1))
		return (nData / ATT7022_DATA_DEC);
	return 0;
}


//------------------------------------------------------------------------
//��	��: att7022_GetVoltage ()
//��	��: 
//��	��: nPhase - ��λֵ
//��	��: -
//��	��: ��ѹ��Чֵ
//��	��: ��ȡ��ǰ��ѹ
//------------------------------------------------------------------------
float att7022_GetVoltage(p_att7022 p, uint_t nPhase)
{
	uint32_t nVoltage = 0;
	float fResult = 0.0;

	nVoltage = att7022_ReadReg(p, ATT7022_REG_URmsA + nPhase);
	fResult = (float)nVoltage / ATT7022_DATA_DEC;
	return fResult;
}



//------------------------------------------------------------------------
//��	��: att7022_GetCurrent ()
//��	��: 
//��	��: nPhase - ��λֵ
//��	��: -
//��	��: ������Чֵ
//��	��: ��ȡ��ǰ����
//------------------------------------------------------------------------
float att7022_GetCurrent(p_att7022 p, uint_t nPhase)
{
	uint32_t nCurrent = 0;
	float fResult = 0.0;

	nCurrent = att7022_ReadReg(p, ATT7022_REG_IRmsA + nPhase);
	fResult = (float)nCurrent / (ICALI_MUL * ATT7022_DATA_DEC);
	return fResult;
}


float att7022_GetPower(p_att7022 p, uint_t nReg, uint_t nPhase)
{
	float fResult,eck;
	
	eck = 3200.0f / (float)ATT7022_CONST_EC; //�������ϵ��
	fResult = att7022_ReadReg(p, nReg + nPhase);
	if (fResult > MAX_VALUE1)
		fResult -= MAX_VALUE2;
	if (nPhase < 3) {
		//��ȡ���๦��
		fResult = fResult * eck / 256000.0f; //ת���ɹ�����
	} else {
		//��ȡ���๦��
		fResult = fResult * eck / 64000.0f; //ת���ɹ�����
	}
	return fResult;
}

uint32_t att7022_GetFlag(p_att7022 p) 
{
	
	return (att7022_ReadReg(p,ATT7022_REG_SFlag)); //��ȡ״̬�Ĵ��� 
}
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
uint32_t att7022_GetPowerDir(p_att7022 p)
{

	//Bit0-3 �ֱ��ʾA B  C  ������й����ʵķ���		0 ��ʾΪ��	1 ��ʾΪ�� 
	//Bit4-7 �ֱ��ʾA B  C  ������޹����ʵķ��� 	0 ��ʾΪ��	1 ��ʾΪ�� 
	return (att7022_ReadReg(p, ATT7022_REG_PFlag)); //��ȡ���ʷ���Ĵ���
}
//------------------------------------------------------------------------
//��	��:  att7022_GetSV ()
//��	��: 
//��	��:  nPhase - ��λֵ��0->A ��1->B ��2->C ��3->����
//��	��:  -
//��	��:  ���ڹ���ֵ
//��	��:  ��ȡ�������ڹ��ʼ��������ڹ���,
//------------------------------------------------------------------------
float att7022_GetSV(p_att7022 p, uint_t nPhase)
{
	float sv, eck;

	sv = att7022_ReadReg(p,ATT7022_REG_SA + nPhase);
	eck = 3200.0f / (float)ATT7022_CONST_EC; //�������ϵ��
	if (sv > MAX_VALUE1)
		sv -= MAX_VALUE2;
	if (nPhase < 3) {
		//��ȡ���๦��
		return (sv / 256000.0f * eck); //ת���ɹ�����
	} else {
		//��ȡ���๦��
		return (sv / 64000.0f * eck); //ת���ɹ�����
	}
}
//------------------------------------------------------------------------
//��	��:  att7022_GetPFV ()
//��	��: 
//��	��:  nPhase - ��λֵ
//��	��:  -
//��	��:  ��������
//��	��:  ��ȡ��������
//------------------------------------------------------------------------
float att7022_GetPFV(p_att7022 p, uint_t nPhase)
{
	int nData = 0;
	float f_data = 0.0f;

	nData = att7022_ReadReg(p, ATT7022_REG_PfA + nPhase);
	if (nData > MAX_VALUE1)
		nData -= MAX_VALUE2;
	f_data = nData / MAX_VALUE1;
	return f_data;
}
//------------------------------------------------------------------------
//��	��:  att7022_GetPAG ()
//��	��: 
//��	��: nPhase - ��λֵ
//��	��: -
//��	��: �����ڵ�ѹ�ļн�
//��	��: ��ȡ�Ƕ�
//------------------------------------------------------------------------
float att7022_GetPAG(p_att7022 p, uint_t nPhase) 
{
	float sita;
	
	sita = att7022_ReadReg(p,ATT7022_REG_PgA + nPhase);
	if (sita > MAX_VALUE1)
		sita -= MAX_VALUE2;
	return (sita * 360 / MAX_VALUE1 / PI);
}

//-------------------------------------------------------------------------
//��ѹ���
//-------------------------------------------------------------------------
float att7022_GetPVAG(p_att7022 p, uint_t nPhase) 
{

	return ((float)att7022_ReadReg(p, ATT7022_REG_YUaUb + nPhase) / 8192.0f);
}

uint16_t att7022_GetQuanrant(p_att7022 p, uint_t Phase) 
{
	uint32_t pflag;
	uint32_t p_direction, q_direction;

	pflag = att7022_ReadReg(p, ATT7022_REG_PFlag); //�ȶ�ȡ���ʷ���Ĵ���(����Ϊ0,����Ϊ1)
	p_direction = ((pflag >> Phase) & 0x00000001);
	q_direction = ((pflag >> (Phase + 4)) & 0x00000001);
	if (p_direction) {
		if (q_direction) {
			//P- Q-
			return 3; //ATT7022B  3
		} else {
			//P- Q+
			return 2; //ATT7022B  2
		}
	} else {
		if (q_direction) {
			//P+ Q-
			return 4; //ATT7022B  4
		} else {
			//P+ Q+
			return 1; //ATT7022B  1
		}
	}
}

/*=============================================================================
 * ����:			att7022_GetPosEP
 * ����:			��ȡ�����й�����
 * ����:			������   2006��10��23��
 * ����޶�:		2006��10��23�� 18:36:07
 * ����:			None
 * �������:		nPhase ��λֵ
 * ����ֵ:		����ֵ ���ۼ�(0.01kwhΪ����)
 * ����˵��:		RVMDK 3.01a
 *=============================================================================*/
uint32_t att7022_GetPosEP(p_att7022 p, uint_t nPhase)
{

	return (att7022_ReadReg(p,ATT7022_REG_PosEpA2 + nPhase)); //��ȡ���ڼ��������ı���ֵ;
}

/*=============================================================================
 * ����:			att7022_GetNegEP
 * ����:			��ȡ�����й�����ֵ
 * ����:			������   2006��10��24��
 * ����޶�:		2006��10��24�� 9:00:20
 * ����:			None
 * �������:		nPhase ��λֵ
 * ����ֵ:		����ֵ ���ۼ�(0.01kwhΪ����)
 * ����˵��:		RVMDK 3.01a
 *=============================================================================*/
uint32_t att7022_GetNegEP(p_att7022 p, uint_t nPhase)
{

	return att7022_ReadReg(p,ATT7022_REG_NegEpA2 + nPhase); //��ȡ���ڼ��������ı���ֵ;	
}

/*=============================================================================
 * ����:			att7022_GetPhaseEQ
 * ����:			��ȡ�����޹�������
 * ����:			������   2006��10��27��
 * ����޶�:		2006��10��27�� 17:43:40
 * ����:			None
 * �������:		None
 * ����ֵ:			None
 * ����˵��:		RVMDK 3.01a
 *=============================================================================*/
uint32_t att7022_GetPhaseEQ(p_att7022 p, uint_t nQuad, uint_t nDir, uint_t nPhase) 
{
	uint32_t EQ;

	if (nQuad < 3) {
		EQ = att7022_ReadReg(p,ATT7022_REG_PosEqA2 + nPhase);
	} else {
		EQ = att7022_ReadReg(p,ATT7022_REG_NegEqA2 + nPhase);
	}

	return EQ;
}

/**************************
 * ����:			att7022_GetHarmonic
 * ����:			��ȡ��ͨ��г��
 * �������:		Ch(0--B)  �ֱ��Ӧ�����ͨ����������3.2k
 Ua\Ia\Ub\Ib\Uc\Ic\In\Ua+Ia\Ub+Ib\Uc+Ic\Ua+Ub+Uc\Ia+Ib+Ic
 * ����ֵ:			
 * ����˵��:		RVMDK 3.01a
***************************/
sys_res att7022_GetHarmonic(p_att7022 p, uint_t Ch, sint16_t *pbuf)
{
	uint_t i, nData, nReg = 0x7F;

	//������������
	nData=0xCCCCC0|Ch;
	att7022_WriteReg(p, 0xC0, nData);//�����������ݻ���
	//�ȴ������������
#if 0
	os_thd_Sleep(200);
#else
	for (i = 20; i; i--) {   //һ���ظ�3�ξ�����?
		if(att7022_ReadReg(p, 0x7E) >= 240)//��һ��д���ݵ�λ��
			break;
		os_thd_Sleep(20);
	}
#endif
	if (i) {
		//�����û���ȡָ�����ʼλ��
		att7022_WriteReg(p, 0xC1, 0);
		for (i = 0; i < 240; i++) {
			spi_Transce(p->spi, &nReg, 1, &nData, 3);//��ȡ����
			reverse(&nData, 3);
			nData &= ATT7022_DATA_MASK;
			if (i < ATT7022_SAMPLEPOINT)
				pbuf[i] = (sint16_t)(nData >> 8);
		}
		return SYS_R_OK;
	}
	return SYS_R_ERR;
}

//------------------------------------------------------------------------ 
//�� ��: att7022_UgainCalibration () 
//�� ��: 
//�� ��: nPhase - ��У׼����, (0 - A��, 1 - B��, 2 - C��) 
//�� ��: - 
//�� ��: gain - У׼�ɹ�����Ugain�ĵ�ǰֵ 
//�� ��: ��ѹ����UgainУ׼
//------------------------------------------------------------------------ 
uint32_t att7022_UgainCalibration(p_att7022 p, uint8_t nPhase )
{
	float urms, k, gain = 0;

	//У��ʹ��  
	att7022_WriteEnable(p);
	//���õ�ѹУ���Ĵ���Ϊ0
	att7022_WriteReg(p, ATT7022_REG_UgainA + nPhase, 0); 
	//У���ֹ 
	att7022_WriteDisable(p); 
	//��1��
	os_thd_Sleep(1000);
	//��ȡ�����ѹֵ 
	urms = att7022_ReadReg(p, ATT7022_REG_URmsA + nPhase);
	
	if(urms)
	{
		 //����ʵ�ʲ���Ĺ�����
		urms = urms / SYS_KVALUE;
		//�˴���UCALI_CONSTΪ��׼����Ugain 
		k = UCALI_CONST / urms - 1;

		gain = k * MAX_VALUE1;
		if (k < 0)
			gain = MAX_VALUE2 + gain;

		//У��ʹ��  
		att7022_WriteEnable(p);
		//д���ѹУ������
		att7022_WriteReg (p, ATT7022_REG_UgainA + nPhase, (uint32_t)gain);
		//У���ֹ
		att7022_WriteDisable(p); 
	}
	return (uint32_t)gain;
} 

//------------------------------------------------------------------------ 
//�� ��: att7022_IgainCalibration () 
//�� ��: 
//�� ��: nPhase - ��У׼����, (0 - A��, 1 - B��, 2 - C��) 
//�� ��: - 
//�� ��: Igain - У׼�ɹ�����Igain�ĵ�ǰֵ 
//�� ��: ��ѹ����IgainУ׼
//------------------------------------------------------------------------ 
uint32_t att7022_IgainCalibration(p_att7022 p, uint32_t nPhase)
{
	float irms, k, gain = 0;

	//У��ʹ��  
	att7022_WriteEnable(p); 
	//���õ�ѹУ���Ĵ���Ϊ0
	att7022_WriteReg(p, ATT7022_REG_IgainA + nPhase, 0); 
	//У���ֹ 
	att7022_WriteDisable(p);  
	//��1��
	os_thd_Sleep(1000);
	//��ȡ�����ѹֵ 
	irms = att7022_ReadReg(p, ATT7022_REG_IRmsA + nPhase);

	if (irms )
	{
		irms = irms / SYS_KVALUE; 
		//�˴���ICALI_CONSTΪ��׼����Igain	
		k = (ICALI_CONST * ICALI_MUL) / irms - 1;	

		gain = k * MAX_VALUE1;
		if (k < 0)
			gain = MAX_VALUE2 + gain;

		//У��ʹ��  
		att7022_WriteEnable(p); 
		//д���ѹУ������
		att7022_WriteReg (p, ATT7022_REG_IgainA + nPhase, (uint32_t)gain);
		//У���ֹ
		att7022_WriteDisable(p); 
	}
	return (uint32_t)gain;
}

//------------------------------------------------------------------------
//��	��: att7022_PgainCalibration () 
//��	��: 
//��	��: nPhase- ��У׼����, (0 - A��, 1 - B��, 2 - C��)
//��	��: -
//��	��:  pgain - У׼�ɹ�����pgain�ĵ�ǰֵ 
//��	��: ���ʲ���pgainУ׼
//------------------------------------------------------------------------
uint32_t att7022_PgainCalibration(p_att7022 p, uint8_t nPhase)
{
	float pvalue, err, eck, pgain = 0;

	//�������ϵ��
	eck = 3200.0f / (float)ATT7022_CONST_EC;
	//У��ʹ��  
	att7022_WriteEnable(p); 
	//������PgainΪ0
	att7022_WriteReg(p, ATT7022_REG_PgainA0 + nPhase, 0);
	att7022_WriteReg(p, ATT7022_REG_PgainA1 + nPhase, 0);
	//У���ֹ 
	att7022_WriteDisable(p); 						
	//�ȴ���ֵ�ȶ�
	os_thd_Sleep(1000);
	//�����������Ĺ���
	pvalue = att7022_ReadReg(p, ATT7022_REG_PA + nPhase);	

	if (pvalue > MAX_VALUE1 )
	{
		pvalue = pvalue - MAX_VALUE2;
	}
	//ת���ɹ�����
	pvalue = (pvalue / 256.0f) * eck;		
	//������
	err = (pvalue - (float)PCALI_CONST) / (float)PCALI_CONST;					
	if (err )
	{
		err = -err / (1 + err);

		pgain = err * MAX_VALUE1;
		if (err < 0 )
			pgain = MAX_VALUE2 + pgain;			//����Pgain

		//У��ʹ��  
		att7022_WriteEnable(p); 
		att7022_WriteReg(p, ATT7022_REG_PgainA0 + nPhase, pgain);
		att7022_WriteReg(p, ATT7022_REG_PgainA1 + nPhase, pgain);
		//У���ֹ
		att7022_WriteDisable(p); 
	}
			
	return (uint32_t)pgain;
}

//------------------------------------------------------------------------
//��	��: BSP_ATT7022B_UIP_gainCalibration () 
//��	��: 
//��	��: -
//��	��: -
//��	��: -
//��	��: У��U��I��P����
//------------------------------------------------------------------------
void att7022_UIP_gainCalibration(p_att7022 p, p_att7022_cali pCali)
{
	uint_t i;

	for (i = 0; i < 3; i++)
	{
		//��ѹͨ��У��
		pCali->Ugain [i] = att7022_UgainCalibration(p, PHASE_A + i);   
		
		//����ͨ��У��
		pCali->Igain[i] = att7022_IgainCalibration(p, PHASE_A + i);   
		
		//����У��
		pCali->Pgain0[i] = att7022_PgainCalibration(p, PHASE_A + i);   
		pCali->Pgain1[i] = pCali->Pgain0[i];
	}
}
//------------------------------------------------------------------------
//��	��: BSP_ATT7022B_Phase_gainCalibration () 
//��	��: 
//��	��: -
//��	��: -
//��	��: -
//��	��: ��λ��У��,��������Ϊ  0.5L  ��������
//------------------------------------------------------------------------
void att7022_Phase_GainCalibration(p_att7022 p, p_att7022_cali pCali)
{
	uint_t i;
	uint32_t phv;

	phv = att7022_PhaseCalibration(p, PHASE_A, 1); //1.5A��У��
	for(i = 0; i < 5; i++) {
		pCali->PhsregA[i] = phv;   
	}
	phv = att7022_PhaseCalibration(p, PHASE_B, 1); //1.5A��У��
	for(i = 0; i < 5; i++) {
		pCali->PhsregB[i] = phv;  
	}
	phv = att7022_PhaseCalibration(p, PHASE_B, 1); //1.5A��У��
	for(i = 0; i < 5; i++) {
		pCali->PhsregC[i] = phv;  
	}
}
//------------------------------------------------------------------------
//��	��: att7022_PhaseCalibration () 
//��	��: 
//��	��: nPhase - ��У������λ
//			cali_point - У����(0 - 4)
//��	��: -
//��	��: phase_v - ���ؼ��������λУ��ֵ(������) 
//��	��:��λУ��, �˴�������У��
//------------------------------------------------------------------------
uint32_t att7022_PhaseCalibration(p_att7022 p, uint8_t nPhase, uint8_t nCali)
{
	uint32_t i;
	float fPhase = 0;

	switch (nCali) {
	case 0:
		att7022_WriteEnable(p);
		for (i = 0; i < 5; i++) {
			//�������У���Ĵ���ֵ
			att7022_WriteReg(p,ATT7022_REG_PhsregA0 + (nPhase *5) + i, 0);
		}
		for (i = 0; i < 4; i++) {
			//����Ĵ���ֵ
			att7022_WriteReg(p,ATT7022_REG_Irgion1 + i, 0);
		}
		att7022_WriteDisable(p);
		sys_Delay(50000);
		fPhase = att7022_FPhaseCaliData(p, nPhase, nCali);
		break;
	case 1:
	case 2:
	case 3:
	case 4:
		fPhase = att7022_FPhaseCaliData(p, nPhase, nCali);
		break;
	default:
		break;
	}
	return (uint32_t)fPhase;
}

float att7022_FPhaseCaliData(p_att7022 p,uint8_t nPhase, uint8_t cali_point) 
{
	float phase_v = 0, att7022_pvalue, seta, err, pcali_value = 0, eck;
	uint_t nTmp = 0;
	
	eck = 3200.0f / (float)ATT7022_CONST_EC;			//�������ϵ��
	pcali_value = PCALI_CONST * 0.5f;					//����У������й����ʳ���
	nTmp = att7022_ReadReg(p, ATT7022_REG_PA + nPhase);	//��ȡ�й�����ֵ
 	if (nTmp > MAX_VALUE1) {
 		nTmp -= MAX_VALUE2;
 	}
	att7022_pvalue = ((float)nTmp / 256.0f) *eck; //ת���ɹ�����
	err = (att7022_pvalue - pcali_value) / pcali_value; //������
	if (err) {
		seta = acosf((1 + err) * 0.5f);
		seta -= PI / 3.0f;
		phase_v = seta * MAX_VALUE1;
		if (seta < 0)
			phase_v = MAX_VALUE2 + phase_v;
	}
	return phase_v;
}





uint32_t att7022_Status(p_att7022 p)
{

	return att7022_ReadReg(p, ATT7022_REG_SFlag);
}

uint32_t att7022_CheckSum1(p_att7022 p)
{

	return att7022_ReadReg(p, ATT7022_REG_ChkSum1);
}

uint32_t att7022_CheckSum2(p_att7022 p)
{

	return att7022_ReadReg(p, ATT7022_REG_ChkSum2);
}



#if 0

/*=============================================================================
 * ����:			att7022_GetTemp
 * ����:			�¶ȶ�ȡ
 * ����:			������   2006��10��23��
 * ����޶�:		2006��10��23�� 18:11:28
 * ����:			None
 * �������:		cali: 1--��ȡδУ����ֵ 0--��ȡУ�����ֵ
 * ����ֵ:		�¶�ֵ(����10��)
 * ����˵��:		RVMDK 3.01a
 *=============================================================================*/
float att7022_GetTemp(p_att7022 p, uint8_t nCali) {
	float fTemp;
	uint16_t nTempCali;
	fTemp = att7022_ReadReg(ATT7022_REG_TempD); //��ȡ�¶�ֵ
	if (fTemp > 128) {
		fTemp -= 256;
	}
	if (!nCali) {
		ATT7022_REG_Get(TERMINAL, 0x1061, &nTempCali);
		fTemp = nTempCali - fTemp;
	}
	return fTemp;
}


//------------------------------------------------------------------------
//��	��: att7022_PgainCalibration () 
//��	��: 
//��	��: nPhase - ��У׼����, (0 - A��, 1 - B��, 2 - C��)
//��	��: -
//��	��: __FALSE - У׼ʧ��, pgain - У׼�ɹ�����pgain�ĵ�ǰֵ 
//��	��: ����pgainУ׼
//------------------------------------------------------------------------
uint32_t att7022_PgainCalibration(p_att7022 p, uint8_t nPhase) {
	float att7022_pvalue, err, eck;
	float pgain = 0;
	if (nPhase > 2) {
		return __FALSE;
	}
	eck = 3200.0f / (float)ATT7022_CONST_EC; //�������ϵ��
	att7022_WriteEnable(); //У��ʹ��
	att7022_WriteReg(ATT7022_REG_PgainA0 + nPhase, 0); //������PgainΪ0
	att7022_WriteReg(ATT7022_REG_PgainA1 + nPhase, 0);
	att7022_WriteDisable(); //У���ֹ 
	sys_Delay(5000000); //�ȴ���ֵ�ȶ�
	att7022_pvalue = att7022_ReadReg(ATT7022_REG_PA + nPhase); //�����������Ĺ���
	if (att7022_pvalue > MAX_VALUE1) {
		att7022_pvalue -= MAX_VALUE2;
	}
	att7022_pvalue = (att7022_pvalue / 256) *eck; //ת���ɹ�����
	err = (att7022_pvalue - PCALI_CONST) / PCALI_CONST; //������
	if (err) {
		pgain = 0-(err / (1+err));
		if (pgain < 0) {
			//pgain Ϊ��ֵ												
			pgain = MAX_VALUE2 + pgain * MAX_VALUE1; //����Pgain
		} else {
			pgain *= MAX_VALUE1; //����Pgain
		}
		att7022_WriteEnable(); //У��ʹ��
		att7022_WriteReg(ATT7022_REG_PgainA0 + nPhase, (uint32_t)pgain); //����Pgain_0����
		att7022_WriteReg(ATT7022_REG_PgainA1 + nPhase, (uint32_t)pgain); //����Pgain_1����
		att7022_WriteDisable();
	}
	return (uint32_t)pgain;
}


//------------------------------------------------------------------------ 
//�� ��: att7022_IgainCalibration () 
//�� ��: 
//�� ��: nPhase - ��У׼����, (0 - A��, 1 - B��, 2 - C��) 
//�� ��: - 
//�� ��: __FALSE - У׼ʧ��, Igain - У׼�ɹ�����Igain�ĵ�ǰֵ 
//�� ��: ����IgainУ׼ 
//------------------------------------------------------------------------ 
uint32_t att7022_IgainCalibration(p_att7022 p, uint8_t nPhase) {
	float irms, k, gain;
	if (nPhase > 2) {
		if (nPhase != PHASE_ADC7) {
			return __FALSE;
		}
	} //�������󷵻� 
	att7022_WriteEnable(); //У��ʹ�� 
	if (nPhase != PHASE_ADC7) {
		att7022_WriteReg(ATT7022_REG_IgainA + nPhase, 0);
	} //����������Ŵ���Ϊ0 
	else {
		att7022_WriteReg(ATT7022_REG_gainADC7, 0);
	} //����������Ŵ���Ϊ0 
	att7022_WriteDisable(); //У���ֹ 
	sys_Delay(5000000);
	if (nPhase != PHASE_ADC7) {
		irms = att7022_ReadReg(ATT7022_REG_IRmsA + nPhase);
	} //��ȡ�������ֵ 
	else {
		irms = att7022_ReadReg(ATT7022_REG_RmsADC7);
	} //��ȡ�������ֵ 
	if (irms) {
		irms = irms / SYS_KVALUE;
		k = (ICALI_CONST * ICALI_MUL) / irms - 1; //�˴���ICALI_CONSTΪ��׼����Igain	
		gain = k * MAX_VALUE1;
		if (k < 0) {
			gain += MAX_VALUE2;
		}
		att7022_WriteEnable(); //У��ʹ�� 
		if (nPhase != PHASE_ADC7) {
			att7022_WriteReg(ATT7022_REG_IgainA + nPhase, (uint32_t)gain);	//д���ѹУ������ 
		} else {
			att7022_WriteReg(ATT7022_REG_gainADC7, (uint32_t)gain);			//д���ѹУ������ 
		}
		att7022_WriteDisable();
		return (uint32_t)gain;
	}
	return __FALSE;
}

//------------------------------------------------------------------------ 
//�� ��: att7022_UgainCalibration () 
//�� ��: 
//�� ��: nPhase - ��У׼����, (0 - A��, 1 - B��, 2 - C��) 
//�� ��: - 
//�� ��: __FALSE - У׼ʧ��, gain - У׼�ɹ�����Ugain�ĵ�ǰֵ 
//�� ��: ����UgainУ׼ 
//------------------------------------------------------------------------ 
uint32_t att7022_UgainCalibration(p_att7022 p, uint8_t nPhase)
{
	float urms, k, gain;
	if (nPhase > 2) {
		if (nPhase != PHASE_ADC7) {
			return __FALSE;
		}
	} //�������󷵻� 
	att7022_WriteEnable(); //У��ʹ�� 
	if (nPhase != PHASE_ADC7) {
		att7022_WriteReg(ATT7022_REG_UgainA + nPhase, 0);
	} //����������Ŵ���Ϊ0 
	else {
		att7022_WriteReg(ATT7022_REG_gainADC7, 0);
	} //����������Ŵ���Ϊ0 
	att7022_WriteDisable(); //У���ֹ 
	sys_Delay(5000000);
	if (nPhase != PHASE_ADC7) {
		urms = att7022_ReadReg(ATT7022_REG_URmsA + nPhase);
	} //��ȡ�����ѹֵ 
	else {
		urms = att7022_ReadReg(ATT7022_REG_RmsADC7);
	} //��ȡ�����ѹֵ 
	if (urms) {
		urms = urms / SYS_KVALUE; //����ʵ�ʲ���Ĺ����� 
		k = UCALI_CONST / urms - 1; //�˴���UCALI_CONSTΪ��׼����Ugain 
		gain = k * MAX_VALUE1;
		if (k < 0) {
			gain += MAX_VALUE2;
		}
		att7022_WriteEnable(); //У��ʹ�� 
		if (nPhase != PHASE_ADC7) {
			att7022_WriteReg(ATT7022_REG_UgainA + nPhase, (uint32_t)gain);
		} //д���ѹУ������ 
		else {
			att7022_WriteReg(ATT7022_REG_gainADC7, (uint32_t)gain);
		} //д���ѹУ������ 
		att7022_WriteDisable();
		return (uint32_t)gain;
	}
	return __FALSE;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void att7022_GainAD7(uint8_t channel) {
#ifdef ATT7022B_ADC7_CALI
	uint32_t gain;
	att7022_WriteEnable();
	//д������
//	att7022_WriteGain();
	if (channel < 6) {
		//д��AD7��ѹ����
		ATT7022_REG_Get(0xFF, 0x1020 + channel, (uint8_t*) &gain);
		att7022_WriteReg(ATT7022_REG_GainAdc7, (uint32_t)gain);
		if (channel < 3) {
			ATT7022_REG_Get(0xFF, 0x1004 + channel, (uint8_t*) &gain);
			att7022_WriteReg(ATT7022_REG_UgainA + channel, (uint32_t)gain);
			ATT7022_REG_Get(0xFF, 0x1026 + channel, (uint8_t*) &gain);
			att7022_WriteReg(ATT7022_REG_PgainA0 + channel, (uint32_t)gain);
			att7022_WriteReg(ATT7022_REG_PgainA1 + channel, (uint32_t)gain);
		} else {
			ATT7022_REG_Get(0xFF, 0x1010 + (channel - 3), (uint8_t*) &gain);
			att7022_WriteReg(ATT7022_REG_IgainA + (channel - 3), (uint32_t)gain);
			ATT7022_REG_Get(0xFF, 0x1029 + (channel - 3), (uint8_t*) &gain);
			att7022_WriteReg(ATT7022_REG_PgainA0 + (channel - 3), (uint32_t)gain);
			att7022_WriteReg(ATT7022_REG_PgainA1 + (channel - 3), (uint32_t)gain);
		}
	}
	att7022_WriteDisable();
#endif 
}


#if 0
/*=============================================================================
 * ����:			att7022_ADC_ChannelSelect
 * ����:			ѡ��ADC7�Ĳ���ͨ��
 * ����:			������   2007��1��20��
 * ����޶�:		2007��1��20�� 17:25:34
 * ����:			None
 * �������:		None
 * ����ֵ:			None
 * ����˵��:		RVMDK 3.03a + RTL-ARM 3.03a
 *=============================================================================*/
int att7022_ADC_ChannelSelect(uint32_t nChl)
{
	uint32_t nFlag;
	int nResult = 1;

	nFlag = att7022_GetFlag();
	switch (nChl){
		case 0:
			if (nFlag & BITMASK(0)){
				//< 20 ) A��ʧѹ
				nResult = 0;
			} else {
				att7022_AD_Channel_Ua();
			}
			break;
		case 1:
			if (nFlag & BITMASK(1)){
				//< 20 )	b��ʧѹ
				nResult = 0;
			} else {
				att7022_AD_Channel_Ub();
			}
			break;
		case 2:
			if (nFlag & BITMASK(2)){
				//< 20 )	c��ʧѹ
				nResult = 0;
			} else {
				att7022_AD_Channel_Uc();
			}
			break;
		case 3:
			if (nFlag & BITMASK(9)){
				//< 20 )	a��Ǳ��
				nResult = 0;
			} else {
				att7022_AD_Channel_Ia();
			}
			break;
		case 4:
			if (nFlag & BITMASK(10)){
				//< 20 ) b��Ǳ��
				nResult = 0;
			} else {
				att7022_AD_Channel_Ib();
			}
			break;
		case 5:
			if (nFlag & BITMASK(11)){
				//< 20 ) c��Ǳ��
				nResult = 0;
			} else {
				att7022_AD_Channel_Ic();
			}
			break;
		default:
			nResult = 0;
			break;
	}
	return nResult;
}



//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
S16 att7022_Getadc7(void) {
	return ((S16)att7022_ReadReg(ATT7022_REG_InstADC7));
}






//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
uint16_t att7022_GetQuanrant(uint8_t Phase) {
	uint32_t pflag;
	uint32_t p_direction, q_direction;
	pflag = att7022_ReadReg(ATT7022_REG_PFlag); //�ȶ�ȡ���ʷ���Ĵ���(����Ϊ0,����Ϊ1)
	p_direction = ((pflag >> Phase) &0x00000001);
	q_direction = ((pflag >> (Phase + 4)) &0x00000001);
	if (p_direction) {
		if (q_direction) {
			//P- Q-
			return 3; //ATT7022B  3
		} else {
			//P- Q+
			return 2; //ATT7022B  2
		}
	} else {
		if (q_direction) {
			//P+ Q-
			return 4; //ATT7022B  4
		} else {
			//P+ Q+
			return 1; //ATT7022B  1
		}
	}
}



//------------------------------------------------------------------------
//��	��: att7022_IBlanceCalc ()
//��	��: 
//��	��: -
//��	��: -
//��	��: ���������ƽ����(%)
//��	��: ������ƽ���ʼ��㣬ʵ��ֱ����10��
//------------------------------------------------------------------------
float att7022_IBlanceCalc(void) {
	float iin[3];
	float temp1;
	float temp2;
	uint32_t i;
	//�����������ֵ
	for (i = 0; i < 3; i++) {
		iin[i] = att7022_GetCurrent(PHASE_A + i) / 1000;
	} 
	temp1 = iin[0];
	if (iin[0] < iin[1]) {
		temp1 = iin[1];
	}
	if (temp1 < iin[2]) {
		temp1 = iin[2];
	} 
	//�������
	temp2 = (iin[0] + iin[1] + iin[2]) / 3; //����ƽ��	
	iin[0] = (temp1 - temp2) / temp2;
	return iin[0];
}

//------------------------------------------------------------------------
//��	��: att7022_VBlanceCalc ()
//��	��: 
//��	��: -
//��	��: -
//��	��: ��ѹ��ƽ����(%)
//��	��: ��ѹ��ƽ���ʼ��㣬ʵ��ֱ����10��
//------------------------------------------------------------------------
float att7022_VBlanceCalc(void) {
	int vin[3];
	int temp;
	uint32_t i;
	//���������ѹֵ
	for (i = 0; i < 3; i++) {
		vin[i] = att7022_GetVoltage(PHASE_A + i);
	}
	temp = (vin[0] + vin[1] + vin[2]) / 3; //����ƽ��
	vin[0] = fabsf(temp - vin[0]);
	vin[1] = fabsf(temp - vin[1]);
	vin[2] = fabsf(temp - vin[2]);
	if (vin[1] < vin[2]) {
		if (vin[0] < vin[2]) {
			vin[0] = vin[2];
		}
	} else {
		if (vin[0] < vin[1]) {
			vin[0] = vin[1];
		}
	} //�������
	vin[0] = vin[0] *100 / temp;
	return vin[0];
}

//------------------------------------------------------------------------
//��	��: void att7022_IstartupSet () 
//��	��: 
//��	��: -
//��	��: -
//��	��: -
//��	��: ATT7022B �����������ã�Ib = 5A, ��������0.2%Ib
//------------------------------------------------------------------------
void att7022_IstartupSet(void) {
	float isv, temp;
	temp = IB_VO * ISTART_RATIO;
	att7022_WriteEnable();
	isv = temp * CONST_G * MAX_VALUE1;
	att7022_WriteReg(ATT7022_REG_Istartup, (uint32_t)isv);
	att7022_WriteDisable();
}


//------------------------------------------------------------------------
//У������
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//��	��: att7022_HFConstSet()
//��	��: 
//��	��: 
//��	��: 
//��	��: 
//��	��:�������峣��У���ѹ��������ã�
//			װ����Ҫ�ṩ��ѹ220����5A��
//------------------------------------------------------------------------
uint32_t att7022_HFConstSet(void) {
	float urms, irms, hfcost;
	urms = att7022_ReadReg(ATT7022_REG_URmsA); //��ȡ��ѹֵ 
	irms = att7022_ReadReg(ATT7022_REG_IRmsA); //��ȡ����ֵ 
	irms = irms / ICALI_MUL;
	hfcost = CONST_HF * CONST_G * CONST_G * urms * irms / (UCALI_CONST *ICALI_CONST * ATT7022_CONST_EC);
	att7022_WriteEnable();
	att7022_WriteReg(ATT7022_REG_HFConst, (uint32_t)hfcost); //����HFConst
	att7022_WriteDisable();
	return ((uint32_t)hfcost);
}

//------------------------------------------------------------------------
//��	��: att7022_PhaseCali2Seg () 
//��	��: 
//��	��: nPhase - ��У������λ
//			 cali_point - У����(0 - 4)
//��	��: 
//��	��: 
//��	��: ��λУ��(0.5L��У��), �˴�������У��
//------------------------------------------------------------------------
void att7022_PhaseCali2Seg(uint8_t nPhase) {
	uint32_t phv[2];
	phv[1] = att7022_PhaseCalibration(PHASE_A, 1); //7.5A��У��
	att7022_WriteEnable(); //ATT7022BУ��ʹ��
	att7022_WriteReg(ATT7022_REG_PhsregA0 + nPhase * 5, phv[1]);
	att7022_WriteReg(ATT7022_REG_PhsregA1 + nPhase * 5, phv[1]);
	att7022_WriteReg(ATT7022_REG_PhsregA2 + nPhase * 5, phv[1]);
	att7022_WriteReg(ATT7022_REG_PhsregA3 + nPhase * 5, phv[1]);
	att7022_WriteReg(ATT7022_REG_PhsregA4 + nPhase * 5, phv[1]);
	att7022_WriteDisable(); //ATT7022BУ����ֹ
}

//------------------------------------------------------------------------
//��	��: att7022_PhaseCalibration () 
//��	��: 
//��	��: nPhase - ��У������λ
//			cali_point - У����(0 - 4)
//��	��: -
//��	��: phase_v - ���ؼ��������λУ��ֵ(������) 
//��	��:��λУ��, �˴�������У��
//------------------------------------------------------------------------
uint32_t att7022_PhaseCalibration(uint8_t nPhase, uint8_t nCali) {
	uint32_t i;
	float fPhase = 0;
	switch (nCali) {
		case 0:
			att7022_WriteEnable();
			for (i = 0; i < 5; i++) {
				//�������У���Ĵ���ֵ
				att7022_WriteReg(ATT7022_REG_PhsregA0 + (nPhase *5) + i, 0);
			}
			for (i = 0; i < 4; i++) {
				//����Ĵ���ֵ
				att7022_WriteReg(ATT7022_REG_Irgion1 + i, 0);
			}
			att7022_WriteDisable();
			sys_Delay(50000);
			fPhase = att7022_FPhaseCaliData(nPhase, nCali);
			break;
		case 1:
		case 2:
		case 3:
		case 4:
			fPhase = att7022_FPhaseCaliData(nPhase, nCali);
			break;
		default:
			break;
	}
	return (uint32_t)fPhase;
}

//------------------------------------------------------------------------
//��	��: att7022_PhaseCaliData () 
//��	��: 
//��	��: nPhase - ��У������λ
//			cali_point - У����(0 - 4)
//��	��: -
//��	��: phase_v - ���ؼ��������λУ��ֵ(˫���ȸ�����) 
//��	��: ��λУ�����ݼ���
//------------------------------------------------------------------------

float att7022_FPhaseCaliData(uint8_t nPhase, uint8_t cali_point) {
	float phase_v = 0, att7022_pvalue, seta, err, pcali_value, eck;
	if (nPhase > 2) {
		return __FALSE;
	}
	eck = 3200.0f / (float)ATT7022_CONST_EC; //�������ϵ��
	pcali_value = 0; //phiconst_tab[cali_point];				//����У������й����ʳ���
	att7022_pvalue = att7022_ReadReg(ATT7022_REG_PA + nPhase); //��ȡ�й�����ֵ
	if (att7022_pvalue > MAX_VALUE1) {
		att7022_pvalue -= MAX_VALUE2;
	}
	att7022_pvalue = (att7022_pvalue / 256) *eck; //ת���ɹ�����
	err = (att7022_pvalue - pcali_value) / pcali_value; //������
	if (err) {
		seta = acosf((1 + err) * 0.5);
		seta -= PI / 3;
		if (seta < 0) {
			phase_v = MAX_VALUE2 + seta * MAX_VALUE1;
		} else {
			phase_v = seta * MAX_VALUE1;
		}
	}
	return phase_v;
}

//------------------------------------------------------------------------
//��	��: att7022_IregionSet () 
//��	��: 
//��	��: currunt_value - ����ֵ(һ��������)
//			 field_num - �����(0 - 3)
//��	��: -
//��	��: __TRUE - ���óɹ�, __FALSE - ����ʧ��
//��	��: ��λ�����������Iregion����, 
//			 ���Էֳ�5�ν�����λ����,
//			 ��Ҫ ��ÿ������ĵ���ֵ��������, 
//			 �������field_numָ��  
//------------------------------------------------------------------------
uint8_t att7022_IregionSet(float current_value, uint8_t field_num) {
	float Iregion;
	if (field_num > 3) {
		return __FALSE;
	}
	att7022_WriteEnable();
	Iregion = CONST_G * current_value * MAX_VALUE1;
	att7022_WriteReg(ATT7022_REG_Irgion1 + field_num, (uint32_t)Iregion);
	att7022_WriteDisable();
	return __TRUE;
}


//------------------------------------------------------------------------
//��	��: att7022_UIP_GainCalibration () 
//��	��: 
//��	��: -
//��	��: -
//��	��: -
//��	��: У��U��I��P����
//------------------------------------------------------------------------
void att7022_UIP_GainCalibration(void) {
	uint32_t nGain;
#ifdef ATT7022B_ADC7_CALI
	//AD7 У��
	att7022_AD_Channel_Ua();
	nGain = att7022_UgainCalibration(PHASE_ADC7);
	ATT7022_REG_Set(TERMINAL, 0x1020, (uint8_t*) &nGain, 0);
	nGain = att7022_UgainCalibration(PHASE_A);
	ATT7022_REG_Set(TERMINAL, 0x1004, (uint8_t*) &nGain, 0);
	nGain = att7022_PgainCalibration(PHASE_A);
	ATT7022_REG_Set(TERMINAL, 0x1026, (uint8_t*) &nGain, 0);
	att7022_AD_Channel_Ub();
	nGain = att7022_UgainCalibration(PHASE_ADC7);
	ATT7022_REG_Set(TERMINAL, 0x1021, (uint8_t*) &nGain, 0);
	nGain = att7022_UgainCalibration(PHASE_B);
	ATT7022_REG_Set(TERMINAL, 0x1005, (uint8_t*) &nGain, 0);
	nGain = att7022_PgainCalibration(PHASE_B);
	ATT7022_REG_Set(TERMINAL, 0x1027, (uint8_t*) &nGain, 0);
	att7022_AD_Channel_Uc();
	nGain = att7022_UgainCalibration(PHASE_ADC7);
	ATT7022_REG_Set(TERMINAL, 0x1022, (uint8_t*) &nGain, 0);
	nGain = att7022_UgainCalibration(PHASE_C);
	ATT7022_REG_Set(TERMINAL, 0x1006, (uint8_t*) &nGain, 0);
	nGain = att7022_PgainCalibration(PHASE_C);
	ATT7022_REG_Set(TERMINAL, 0x1028, (uint8_t*) &nGain, 0);
	att7022_AD_Channel_Ia();
	nGain = att7022_IgainCalibration(PHASE_ADC7);
	ATT7022_REG_Set(TERMINAL, 0x1023, (uint8_t*) &nGain, 0);
	nGain = att7022_IgainCalibration(PHASE_A);
	ATT7022_REG_Set(TERMINAL, 0x1010, (uint8_t*) &nGain, 0);
	nGain = att7022_PgainCalibration(PHASE_A);
	ATT7022_REG_Set(TERMINAL, 0x1029, (uint8_t*) &nGain, 0);
	att7022_AD_Channel_Ib();
	nGain = att7022_IgainCalibration(PHASE_ADC7);
	ATT7022_REG_Set(TERMINAL, 0x1024, (uint8_t*) &nGain, 0);
	nGain = att7022_IgainCalibration(PHASE_B);
	ATT7022_REG_Set(TERMINAL, 0x1011, (uint8_t*) &nGain, 0);
	nGain = att7022_PgainCalibration(PHASE_B);
	ATT7022_REG_Set(TERMINAL, 0x102a, (uint8_t*) &nGain, 0);
	att7022_AD_Channel_Ic();
	nGain = att7022_IgainCalibration(PHASE_ADC7);
	ATT7022_REG_Set(TERMINAL, 0x1025, (uint8_t*) &nGain, 0);
	nGain = att7022_IgainCalibration(PHASE_C);
	ATT7022_REG_Set(TERMINAL, 0x1012, (uint8_t*) &nGain, 0);
	nGain = att7022_PgainCalibration(PHASE_C);
	ATT7022_REG_Set(TERMINAL, 0x102b, (uint8_t*) &nGain, 0);
#endif 
	att7022_AD_Channel_None();
	//��ѹͨ��У��
	nGain = att7022_UgainCalibration(PHASE_A);
	ATT7022_REG_Set(TERMINAL, 0x1001, (uint8_t*) &nGain, 0);
	nGain = att7022_UgainCalibration(PHASE_B);
	ATT7022_REG_Set(TERMINAL, 0x1002, (uint8_t*) &nGain, 0);
	nGain = att7022_UgainCalibration(PHASE_C);
	ATT7022_REG_Set(TERMINAL, 0x1003, (uint8_t*) &nGain, 0);
	//����ͨ��У��
	nGain = att7022_IgainCalibration(PHASE_A);
	ATT7022_REG_Set(TERMINAL, 0x1007, (uint8_t*) &nGain, 0);
	nGain = att7022_IgainCalibration(PHASE_B);
	ATT7022_REG_Set(TERMINAL, 0x1008, (uint8_t*) &nGain, 0);
	nGain = att7022_IgainCalibration(PHASE_C);
	ATT7022_REG_Set(TERMINAL, 0x1009, (uint8_t*) &nGain, 0);
	//����У��
	nGain = att7022_PgainCalibration(PHASE_A);
	ATT7022_REG_Set(TERMINAL, 0x1013, (uint8_t*) &nGain, 0);
	nGain = att7022_PgainCalibration(PHASE_B);
	ATT7022_REG_Set(TERMINAL, 0x1014, (uint8_t*) &nGain, 0);
	nGain = att7022_PgainCalibration(PHASE_C);
	ATT7022_REG_Set(TERMINAL, 0x1015, (uint8_t*) &nGain, 1);
}



//------------------------------------------------------------------------
//��	��: att7022_PhaseABC_Calibration () 
//��	��: 
//��	��: -
//��	��: -
//��	��: -
//��	��: У����λ
//------------------------------------------------------------------------
void att7022_PhaseABC_Calibration(void) {
	float err;
	int *p1,  *p2;
	p1 = (int*) &err; //CommDataArea[7];
	p2 = (int*) &err; //CommDataArea[9];
	err = (float)((*p1) / (*p2));
}

//��ʱδʹ�õĺ���
/*
float att7022_GetAD7(uint32 fat) {
	return ((float)att7022_ReadReg(ATT7022_REG_RmsADC7) / 8192 * fat);
}

//------------------------------------------------------------------------
//��	��: att7022_IronlossCalc ()
//��	��: 
//��	��: -
//��	��: -
//��	��: ����ֵ
//��	��: ����ֵ����
//------------------------------------------------------------------------
float att7022_IronlossCalc(void) {
	float iron_loss, vin[3];
	uint32 i;
	//���������ѹֵ
	for (i = 0; i < 3; i++) {
		vin[i] = att7022_GetVoltage(PHASE_A + i) / 10;
	}
	iron_loss = vin[0] *vin[0] + vin[1] *vin[1] + vin[2] *vin[2];
	return iron_loss;
}

//------------------------------------------------------------------------
//��	��: att7022_CopperlossCalc ()
//��	��: 
//��	��: -
//��	��: -
//��	��: ͭ��ֵ
//��	��: ͭ��ֵ����
//------------------------------------------------------------------------
float att7022_CopperlossCalc(void) {
	float copper_loss, iin[3];
	uint32 i;
	//�����������ֵ
	for (i = 0; i < 3; i++) {
		iin[i] = att7022_GetCurrent(PHASE_A + i) / 1000;
	}
	copper_loss = iin[0] *iin[0] + iin[1] *iin[1] + iin[2] *iin[2];
	copper_loss = copper_loss * 10;
	return copper_loss;
}
*/

#endif

#endif

#endif

