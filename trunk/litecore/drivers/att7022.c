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

static uint32_t att7022_UgainCalibration(p_att7022 p, uint_t nPhase)
{
	float urms, k, gain = 0;

	//��ȡ�����ѹֵ 
	urms = att7022_Read(p, ATT7022_REG_URmsA + nPhase);
	if (urms) {
		 //����ʵ�ʲ���Ĺ�����
		urms = urms / SYS_KVALUE;
		//�˴���UCALI_CONSTΪ��׼����Ugain 
		k = UCALI_CONST / urms - 1;
		gain = k * MAX_VALUE1;
		if (k < 0)
			gain = MAX_VALUE2 + gain;
	}
	return (uint32_t)(gain + 0.5f);
} 

static uint32_t att7022_IgainCalibration(p_att7022 p, uint_t nPhase)
{
	float irms, k, gain = 0;

	//��ȡ�����ѹֵ 
	irms = att7022_Read(p, ATT7022_REG_IRmsA + nPhase);
	if (irms) {
		irms = irms / SYS_KVALUE; 
		//�˴���ICALI_CONSTΪ��׼����Igain	
		k = (ICALI_CONST * ICALI_MUL) / irms - 1;	
		gain = k * MAX_VALUE1;
		if (k < 0)
			gain = MAX_VALUE2 + gain;
	}
	return (uint32_t)(gain + 0.5f);
}

static uint32_t att7022_PgainCalibration(p_att7022 p, uint_t nPhase)
{
	float pvalue, err, pgain = 0;

	//�����������Ĺ���
	pvalue = att7022_Read(p, ATT7022_REG_PA + nPhase);	
	if (pvalue > MAX_VALUE1)
		pvalue -= MAX_VALUE2;
	//ת���ɹ�����
	pvalue = (pvalue / 256.0f) * (3200.0f / (float)ATT7022_CONST_EC);		
	//������
	err = (pvalue - (float)PCALI_CONST) / (float)PCALI_CONST;
	if (err) {
		err = -err / (1 + err);
		pgain = err * MAX_VALUE1;
		if (err < 0 )
			pgain = MAX_VALUE2 + pgain;			//����Pgain
	}
	return (uint32_t)(pgain + 0.5f);
}

static uint32_t att7022_FPhaseCaliData(p_att7022 p, uint_t nPhase) 
{
	float phase_v = 0, att7022_pvalue, seta, err, pcali_value = 0;
	uint32_t nTmp = 0;

	pcali_value = PCALI_CONST * 0.5f;					//����У������й����ʳ���
	nTmp = att7022_Read(p, ATT7022_REG_PA + nPhase);	//��ȡ�й�����ֵ
 	if (nTmp > MAX_VALUE1)
 		nTmp -= MAX_VALUE2;
	att7022_pvalue = ((float)nTmp / 256.0f) * (3200.0f / (float)ATT7022_CONST_EC); //ת���ɹ�����
	err = (att7022_pvalue - pcali_value) / pcali_value; //������
	if (err) {
		seta = acosf((1 + err) * 0.5f);
		seta -= PI / 3.0f;
		phase_v = seta * MAX_VALUE1;
		if (seta < 0)
			phase_v = MAX_VALUE2 + phase_v;
	}
	return (uint32_t)(phase_v + 0.5f);
}







//External Functions
void att7022_Init()
{
	p_gpio_def pDef;

	for (pDef = tbl_bspAtt7022[0]; pDef < tbl_bspAtt7022[1]; pDef++)
		sys_GpioConf(pDef);
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

uint32_t att7022_Read(p_att7022 p, uint_t nReg)
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

	nData = att7022_Read(p, ATT7022_REG_Freq);
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

	nVoltage = att7022_Read(p, ATT7022_REG_URmsA + nPhase);
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

	nCurrent = att7022_Read(p, ATT7022_REG_IRmsA + nPhase);
	fResult = (float)nCurrent / (ICALI_MUL * ATT7022_DATA_DEC);
	return fResult;
}


float att7022_GetPower(p_att7022 p, uint_t nReg, uint_t nPhase)
{
	float fResult,eck;
	
	eck = 3200.0f / (float)ATT7022_CONST_EC; //�������ϵ��
	fResult = att7022_Read(p, nReg + nPhase);
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

uint_t att7022_GetFlag(p_att7022 p) 
{
	
	return att7022_Read(p,ATT7022_REG_SFlag); //��ȡ״̬�Ĵ��� 
}
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
uint_t att7022_GetPowerDir(p_att7022 p)
{

	//Bit0-3 �ֱ��ʾA B  C  ������й����ʵķ���		0 ��ʾΪ��	1 ��ʾΪ�� 
	//Bit4-7 �ֱ��ʾA B  C  ������޹����ʵķ��� 	0 ��ʾΪ��	1 ��ʾΪ�� 
	return att7022_Read(p, ATT7022_REG_PFlag); //��ȡ���ʷ���Ĵ���
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

	sv = att7022_Read(p,ATT7022_REG_SA + nPhase);
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

	nData = att7022_Read(p, ATT7022_REG_PfA + nPhase);
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
	
	sita = att7022_Read(p,ATT7022_REG_PgA + nPhase);
	if (sita > MAX_VALUE1)
		sita -= MAX_VALUE2;
	return (sita * 360 / MAX_VALUE1 / PI);
}

//-------------------------------------------------------------------------
//��ѹ���
//-------------------------------------------------------------------------
float att7022_GetPVAG(p_att7022 p, uint_t nPhase) 
{

	return ((float)att7022_Read(p, ATT7022_REG_YUaUb + nPhase) / 8192.0f);
}

uint_t att7022_GetQuanrant(p_att7022 p, uint_t Phase) 
{
	uint32_t pflag;
	uint32_t p_direction, q_direction;

	pflag = att7022_Read(p, ATT7022_REG_PFlag); //�ȶ�ȡ���ʷ���Ĵ���(����Ϊ0,����Ϊ1)
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
		if(att7022_Read(p, 0x7E) >= 240)//��һ��д���ݵ�λ��
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
//��	��: BSP_ATT7022B_UIP_gainCalibration () 
//��	��: 
//��	��: -
//��	��: -
//��	��: -
//��	��: У��U��I��P����
//------------------------------------------------------------------------
void att7022_CaliUIP(p_att7022 p, p_att7022_cali pCali)
{
	uint_t i, j;
	uint32_t phv;

	for (i = 0; i < 3; i++) {
		//��ѹͨ��У��
		phv = 0;
		for (j = 0; j < 8; j++) {
			phv += att7022_UgainCalibration(p, PHASE_A + i);
			os_thd_Sleep(250);
		}
		phv /= 8;
		pCali->Ugain[i] = phv;
		
		//����ͨ��У��
		phv = 0;
		for (j = 0; j < 8; j++) {
			phv += att7022_IgainCalibration(p, PHASE_A + i);
			os_thd_Sleep(250);
		}
		phv /= 8;
		pCali->Igain[i] = phv;   
		
		//����У��
		phv = 0;
		for (j = 0; j < 8; j++) {
			phv += att7022_PgainCalibration(p, PHASE_A + i);
			os_thd_Sleep(250);
		}
		phv /= 8;
		pCali->Pgain0[i] = phv;
		pCali->Pgain1[i] = phv;
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
void att7022_CaliPhase(p_att7022 p, p_att7022_cali pCali)
{
	uint_t i;
	uint32_t phv;

	phv = 0;
	for (i = 0; i < 8; i++) {
		phv += att7022_FPhaseCaliData(p, PHASE_A); //1.5A��У��
		os_thd_Sleep(250);
	}
	phv /= 8;
#if 1
	phv *= 9;
	phv /= 8;
#endif
	if (phv < 0x18000)
		phv = 0x18000;
	for (i = 0; i < 5; i++) {
		pCali->PhsregA[i] = phv;   
	}
	phv = 0;
	for (i = 0; i < 8; i++) {
		phv += att7022_FPhaseCaliData(p, PHASE_B); //1.5A��У��
		os_thd_Sleep(250);
	}
	phv /= 8;
#if 0
	phv *= 9;
	phv /= 8;
#endif
	if (phv < 0x18000)
		phv = 0x18000;
	for (i = 0; i < 5; i++) {
		pCali->PhsregB[i] = phv;  
	}
	phv = 0;
	for (i = 0; i < 8; i++) {
		phv += att7022_FPhaseCaliData(p, PHASE_C); //1.5A��У��
		os_thd_Sleep(250);
	}
	phv /= 8;
#if 1
	phv *= 10;
	phv /= 9;
#endif
	if (phv < 0x18000)
		phv = 0x18000;
	for (i = 0; i < 5; i++) {
		pCali->PhsregC[i] = phv;  
	}
}



#endif

