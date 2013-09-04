#ifndef __ATT7022_H__
#define __ATT7022_H__


#ifdef __cplusplus
extern "C" {
#endif








//------------------------------------------------------------------------
//ATT7022B calibration register define(write only)
//------------------------------------------------------------------------
#define ATT7022_REG_Irgion1			0x02		//��λ������������1
#define	ATT7022_REG_Irgion2			0x03		//��λ������������2
#define	ATT7022_REG_Irgion3			0x04		//��λ������������3
#define	ATT7022_REG_Irgion4			0x05		//��λ������������4

#define	ATT7022_REG_PgainA0			0x06		//A�๦������0
#define	ATT7022_REG_PgainB0			0x07		//B�๦������0
#define	ATT7022_REG_PgainC0			0x08		//C�๦������0
#define	ATT7022_REG_PgainA1			0x09		//A�๦������1
#define	ATT7022_REG_PgainB1			0x0a		//B�๦������1
#define	ATT7022_REG_PgainC1			0x0b		//C�๦������1

#define	ATT7022_REG_PhsregA0		0x0c		//A������0��λУ��
#define	ATT7022_REG_PhsregA1		0x0d		//A������1��λУ��
#define	ATT7022_REG_PhsregA2		0x0e		//A������2��λУ��
#define	ATT7022_REG_PhsregA3		0x0f		//A������3��λУ��
#define	ATT7022_REG_PhsregA4		0x10		//A������4��λУ��

#define	ATT7022_REG_PhsregB0		0x11		//B������0��λУ��
#define	ATT7022_REG_PhsregB1		0x12		//B������1��λУ��
#define	ATT7022_REG_PhsregB2		0x13		//B������2��λУ��
#define	ATT7022_REG_PhsregB3		0x14		//B������3��λУ��
#define	ATT7022_REG_PhsregB4		0x15		//B������4��λУ��

#define	ATT7022_REG_PhsregC0		0x16		//C������0��λУ��
#define	ATT7022_REG_PhsregC1		0x17		//C������1��λУ��
#define	ATT7022_REG_PhsregC2		0x18		//C������2��λУ�� 
#define	ATT7022_REG_PhsregC3		0x19		//C������3��λУ��
#define	ATT7022_REG_PhsregC4		0x1a		//C������4��λУ��

#define	ATT7022_REG_UgainA			0x1b		//A���ѹ��������
#define	ATT7022_REG_UgainB			0x1c		//B���ѹ��������
#define	ATT7022_REG_UgainC			0x1d		//C���ѹ��������

#define	ATT7022_REG_Irechg			0x1e		//�Ȳ����������
#define	ATT7022_REG_Istartup		0x1f		//����������ֵ����
#define	ATT7022_REG_HFConst			0x20		//��Ƶ�����������

#define	ATT7022_REG_IgainA			0x26		//A�������������
#define	ATT7022_REG_IgainB			0x27		//B�������������
#define	ATT7022_REG_IgainC			0x28		//C�������������

#define	ATT7022_REG_FailVoltage		0x29		//ʧѹ��ֵ����
#define	ATT7022_REG_EAddMode		0x2a		//���������ۼ�ģʽѡ��
#define	ATT7022_REG_GainAdc7		0x2b		//����·ADC��ЧֵУ��
#define	ATT7022_REG_GCtrlT7Adc		0x2c		//�¶�/����·ADCѡ�����
#define	ATT7022_REG_EnlineFreq		0x2d		//��������ʹ�ܿ���
#define	ATT7022_REG_EnUAngle		0x2e		//��ѹ�нǲ���ʹ�ܿ���
#define	ATT7022_REG_SelectPQSU		0x2f		//������ѹ�������ѡ��
#define	ATT7022_REG_EnDtIorder		0x30		//����������ʹ�ܿ���
#define	ATT7022_REG_LineFreqPg		0x31		//��������У��
#define	ATT7022_REG_EnHarmonic		0x3C		//����������г�������л�ѡ��
#define	ATT7022_REG_UADCPga			0x3f		//��ѹͨ��ADC����ѡ��

//------------------------------------------------------------------------
//ATT7022B data register define(read only)
//------------------------------------------------------------------------
#define	ATT7022_REG_PA				0x01		//A���й�����
#define	ATT7022_REG_PB				0x02		//B���й�����
#define	ATT7022_REG_PC				0x03		//C���й�����
#define	ATT7022_REG_PT				0x04		//�����й�����

#define	ATT7022_REG_QA				0x05		//A���޹�����
#define	ATT7022_REG_QB				0x06		//B���޹�����
#define	ATT7022_REG_QC				0x07		//C���޹�����
#define	ATT7022_REG_QT				0x08		//�����޹�����

#define	ATT7022_REG_SA				0x09		//A�����ڹ���
#define	ATT7022_REG_SB				0x0a		//B�����ڹ���
#define	ATT7022_REG_SC				0x0b		//C�����ڹ���
#define	ATT7022_REG_ST				0x0c		//�������ڹ���

#define	ATT7022_REG_URmsA			0x0d		//A���ѹ��Чֵ
#define	ATT7022_REG_URmsB			0x0e		//B���ѹ��Чֵ
#define	ATT7022_REG_URmsC			0x0f		//C���ѹ��Чֵ

#define	ATT7022_REG_IRmsA			0x10		//A�������Чֵ
#define	ATT7022_REG_IRmsB			0x11		//B�������Чֵ
#define	ATT7022_REG_IRmsC			0x12		//C�������Чֵ
#define	ATT7022_REG_IRmsT			0x13		//ABC�����ʸ���͵���Чֵ

#define	ATT7022_REG_PfA				0x14		//A�๦������
#define	ATT7022_REG_PfB				0x15		//B�๦������
#define	ATT7022_REG_PfC				0x16		//C�๦������
#define	ATT7022_REG_PfT				0x17		//���๦������

#define	ATT7022_REG_PgA				0x18		//A��������ѹ�����
#define	ATT7022_REG_PgB				0x19		//B��������ѹ�����		
#define	ATT7022_REG_PgC				0x1a		//C��������ѹ�����
#define	ATT7022_REG_PgT				0x1b		//����������ѹ�����	

#define	ATT7022_REG_Freq			0x1c		//��Ƶ��

#define	ATT7022_REG_EpA				0x1e		//A���й�����
#define	ATT7022_REG_EpB				0x1f		//B���й�����
#define	ATT7022_REG_EpC				0x20		//C���й�����
#define	ATT7022_REG_EpT				0x21		//�����й�����

#define	ATT7022_REG_EqA				0x22		//A���޹�����
#define	ATT7022_REG_EqB				0x23		//B���޹�����
#define	ATT7022_REG_EqC				0x24		//C���޹�����
#define	ATT7022_REG_EqT				0x25		//�����޹�����

#define	ATT7022_REG_RSPIData		0x28		//�ϴ�SPI����������
#define	ATT7022_REG_RmsADC7			0x29		//����·ADC�����źŵ���Чֵ
#define	ATT7022_REG_TempD			0x2a		//�¶ȴ��������

#define	ATT7022_REG_URmst			0x2b		//ABC��ѹʸ���͵���Чֵ
#define ATT7022_REG_gainADC7		0x2b		//ADC7����Ĵ���

#define	ATT7022_REG_SFlag			0x2c		//ϵͳ״̬��־
#define	ATT7022_REG_WSPIData1		0x2d		//��һ��д��SPI������
#define	ATT7022_REG_WSPIData2		0x2e		//

#define	ATT7022_REG_EFlag			0x30		//���ܼĴ����Ĺ���״̬

#define	ATT7022_REG_EpA2			0x31		//A���й�����
#define	ATT7022_REG_EpB2			0x32		//B���й�����
#define	ATT7022_REG_EpC2			0x33		//C���й�����
#define	ATT7022_REG_EpT2			0x34		//�����й�����

#define	ATT7022_REG_EqA2			0x35		//A���й�����
#define	ATT7022_REG_EqB2			0x35		//B���й�����
#define	ATT7022_REG_EqC2			0x35		//C���й�����
#define	ATT7022_REG_EqT2			0x35		//�����й�����


#define	ATT7022_REG_LEFlag			0x3c		//�������ܼĴ�������״̬
#define	ATT7022_REG_PFlag			0x3d		//�й����޹����ʷ���(����Ϊ0, ����Ϊ1)

#define	ATT7022_REG_ChkSum1			0x3e		//У������У��Ĵ���

#define	ATT7022_REG_InstADC7		0x3f		//��7·ADC�����������

#define	ATT7022_REG_PosEpA			0x40		//A�������й�����
#define	ATT7022_REG_PosEpB			0x41		//B�������й�����
#define	ATT7022_REG_PosEpC			0x42		//C�������й�����
#define	ATT7022_REG_PosEpT			0x43		//���������й�����

#define	ATT7022_REG_NegEpA			0x44		//A�ฺ���й�����
#define	ATT7022_REG_NegEpB			0x45		//B�ฺ���й�����
#define	ATT7022_REG_NegEpC			0x46		//C�ฺ���й�����
#define	ATT7022_REG_NegEpT			0x47		//���ฺ���й�����

#define	ATT7022_REG_PosEqA			0x48		//A�������޹�����
#define	ATT7022_REG_PosEqB			0x49		//B�������޹�����
#define	ATT7022_REG_PosEqC			0x4a		//C�������޹�����
#define	ATT7022_REG_PosEqT			0x4b		//���������޹�����

#define	ATT7022_REG_NegEqA			0x4c		//A�ฺ���޹�����
#define	ATT7022_REG_NegEqB			0x4d		//B�ฺ���޹�����
#define	ATT7022_REG_NegEqC			0x4e		//C�ฺ���޹�����
#define	ATT7022_REG_NegEqT			0x4f		//���ฺ���޹�����

#define	ATT7022_REG_LineEpA			0x50		//A������й�����
#define	ATT7022_REG_LineEpB			0x51		//A������й�����
#define	ATT7022_REG_LineEpC			0x52		//A������й�����
#define	ATT7022_REG_LineEpT			0x53		//��������й�����

#define	ATT7022_REG_LineEqA			0x54		//A������޹�����
#define	ATT7022_REG_LineEqB			0x55		//A������޹�����
#define	ATT7022_REG_LineEqC			0x56		//A������޹�����
#define	ATT7022_REG_LineEqT			0x57		//��������޹�����

#define	ATT7022_REG_YUaUb			0x5c		//Ua��Ub�ĵ�ѹ�н�
#define	ATT7022_REG_YUaUc			0x5d		//Ua��Uc�ĵ�ѹ�н�	
#define	ATT7022_REG_YUbUc			0x5e		//Ub��Uc�ĵ�ѹ�н�


#define	ATT7022_REG_PosEpA2			0x60		//A�������й�����(��ȡһ�α����)
#define	ATT7022_REG_PosEpB2			0x61		//A�������й�����(��ȡһ�α����)
#define	ATT7022_REG_PosEpC2			0x62		//A�������й�����(��ȡһ�α����)
#define	ATT7022_REG_PosEpT2			0x63		//A�������й�����(��ȡһ�α����)

#define	ATT7022_REG_NegEpA2			0x64		//A�ฺ���й�����(��ȡһ�α����)
#define	ATT7022_REG_NegEpB2			0x65		//B�ฺ���й�����(��ȡһ�α����)
#define	ATT7022_REG_NegEpC2			0x66		//C�ฺ���й�����(��ȡһ�α����)
#define	ATT7022_REG_NegEpT2			0x67		//���ฺ���й�����(��ȡһ�α����)

#define	ATT7022_REG_PosEqA2			0x68		//A�������޹�����(��ȡһ�α����)
#define	ATT7022_REG_PosEqB2			0x69		//B�������޹�����(��ȡһ�α����)
#define	ATT7022_REG_PosEqC2			0x6a		//C�������޹�����(��ȡһ�α����)
#define	ATT7022_REG_PosEqT2			0x6b		//���������޹�����(��ȡһ�α����)

#define	ATT7022_REG_NegEqA2			0x6c		//A�ฺ���޹�����(��ȡһ�α����)
#define	ATT7022_REG_NegEqB2			0x6d		//B�ฺ���޹�����(��ȡһ�α����)
#define	ATT7022_REG_NegEqC2			0x6e		//C�ฺ���޹�����(��ȡһ�α����)
#define	ATT7022_REG_NegEqT2			0x6f		//���ฺ���޹�����(��ȡһ�α����)

//------------------------------------------------------------------------
//ATT7022B command set
//------------------------------------------------------------------------
#define ATT7022_CALIDATA_CLR		0xC3		//У�������������
#define ATT7022_CALIDATA_READ		0xC6		//��У����������
#define ATT7022_CALIDATA_WEN		0xC9		//У������дʹ��
#define ATT7022_SOFTRST_CMD			0xD3		//�����λ����

//------------------------------------------------------------------------
//ATT7022B calibration register define(write only)
//------------------------------------------------------------------------
#define ATT7022_REG_Irgion1			0x02		//��λ������������1
#define ATT7022_REG_Irgion2			0x03		//��λ������������2
#define ATT7022_REG_Irgion3			0x04		//��λ������������3
#define ATT7022_REG_Irgion4			0x05		//��λ������������4

#define ATT7022_REG_PhsregA0		0x0c		//A������0��λУ��
#define ATT7022_REG_PhsregA1		0x0d		//A������1��λУ��
#define ATT7022_REG_PhsregA2		0x0e		//A������2��λУ��
#define ATT7022_REG_PhsregA3		0x0f		//A������3��λУ��
#define ATT7022_REG_PhsregA4		0x10		//A������4��λУ��

#define ATT7022_REG_PhsregB0		0x11		//B������0��λУ��
#define ATT7022_REG_PhsregB1		0x12		//B������1��λУ��
#define ATT7022_REG_PhsregB2		0x13		//B������2��λУ��
#define ATT7022_REG_PhsregB3		0x14		//B������3��λУ��
#define ATT7022_REG_PhsregB4		0x15		//B������4��λУ��


#define ATT7022_REG_PhsregC0		0x16		//C������0��λУ��
#define ATT7022_REG_PhsregC1		0x17		//C������1��λУ��
#define ATT7022_REG_PhsregC2		0x18		//C������2��λУ�� 
#define ATT7022_REG_PhsregC3		0x19		//C������3��λУ��
#define ATT7022_REG_PhsregC4		0x1a		//C������4��λУ��

#define ATT7022_REG_Irechg			0x1e		//�Ȳ����������
#define ATT7022_REG_Istartup		0x1f		//����������ֵ����
#define ATT7022_REG_HFConst			0x20		//��Ƶ�����������

#define ATT7022_REG_FailVoltage		0x29		//ʧѹ��ֵ����
#define ATT7022_REG_EAddMode		0x2a		//���������ۼ�ģʽѡ��
#define ATT7022_REG_GainAdc7		0x2b		//����·ADC��ЧֵУ��
#define ATT7022_REG_GCtrlT7Adc		0x2c		//�¶�/����·ADCѡ�����
#define ATT7022_REG_EnlineFreq		0x2d		//��������ʹ�ܿ���
#define ATT7022_REG_EnUAngle		0x2e		//��ѹ�нǲ���ʹ�ܿ���
#define ATT7022_REG_SelectPQSU		0x2f		//������ѹ�������ѡ��
#define ATT7022_REG_EnDtIorder		0x30		//����������ʹ�ܿ���
#define ATT7022_REG_LineFreqPg		0x31		//��������У��
#define	ATT7022_REG_EnHarmonic		0x3C		//����������г�������л�ѡ��
#define ATT7022_REG_UADCPga			0x3f		//��ѹͨ��ADC����ѡ��

//------------------------------------------------------------------------
//ATT7022B data register define(read only)
//------------------------------------------------------------------------
#define ATT7022_REG_SA				0x09		//A�����ڹ���
#define ATT7022_REG_SB				0x0a		//B�����ڹ���
#define ATT7022_REG_SC				0x0b		//C�����ڹ���
#define ATT7022_REG_ST				0x0c		//�������ڹ���

#define ATT7022_REG_PfA				0x14		//A�๦������
#define ATT7022_REG_PfB				0x15		//B�๦������
#define	ATT7022_REG_PfC				0x16		//C�๦������
#define	ATT7022_REG_PfT				0x17        //���๦������
#define	ATT7022_REG_PgA				0x18		//A��������ѹ�����
#define	ATT7022_REG_PgB				0x19		//B��������ѹ�����		
#define	ATT7022_REG_PgC				0x1a		//C��������ѹ�����
#define	ATT7022_REG_PgT				0x1b		//����������ѹ�����	

#define	ATT7022_REG_Freq			0x1c		//��Ƶ��

#define	ATT7022_REG_EpA				0x1e		//A���й�����
#define	ATT7022_REG_EpB				0x1f		//B���й�����
#define	ATT7022_REG_EpC				0x20		//C���й�����
#define	ATT7022_REG_EpT				0x21		//�����й�����

#define	ATT7022_REG_EqA				0x22		//A���޹�����
#define	ATT7022_REG_EqB				0x23		//B���޹�����
#define	ATT7022_REG_EqC				0x24		//C���޹�����
#define	ATT7022_REG_EqT				0x25		//�����޹�����

#define	ATT7022_REG_RSPIData		0x28		//�ϴ�SPI����������
#define	ATT7022_REG_RmsADC7			0x29		//����·ADC�����źŵ���Чֵ
#define	ATT7022_REG_TempD			0x2a		//�¶ȴ��������

#define	ATT7022_REG_URmst			0x2b		//ABC��ѹʸ���͵���Чֵ
#define ATT7022_REG_gainADC7		0x2b		//ADC7����Ĵ���

#define	ATT7022_REG_SFlag			0x2c		//ϵͳ״̬��־
#define	ATT7022_REG_WSPIData1		0x2d		//��һ��д��SPI������
#define	ATT7022_REG_WSPIData2		0x2e		//

#define	ATT7022_REG_EFlag			0x30		//���ܼĴ����Ĺ���״̬

#define	ATT7022_REG_EpA2			0x31		//A���й�����
#define	ATT7022_REG_EpB2			0x32		//B���й�����
#define	ATT7022_REG_EpC2			0x33		//C���й�����
#define	ATT7022_REG_EpT2			0x34		//�����й�����

#define	ATT7022_REG_EqA2			0x35		//A���й�����
#define	ATT7022_REG_EqB2			0x35		//B���й�����
#define	ATT7022_REG_EqC2			0x35		//C���й�����
#define	ATT7022_REG_EqT2			0x35		//�����й�����


#define	ATT7022_REG_LEFlag			0x3c		//�������ܼĴ�������״̬
#define	ATT7022_REG_PFlag			0x3d		//�й����޹����ʷ���(����Ϊ0, ����Ϊ1)

#define	ATT7022_REG_ChkSum1			0x3e		//У������У��Ĵ���

#define	ATT7022_REG_InstADC7		0x3f		//��7·ADC�����������

#define	ATT7022_REG_PosEpA			0x40		//A�������й�����
#define	ATT7022_REG_PosEpB			0x41		//B�������й�����
#define	ATT7022_REG_PosEpC			0x42		//C�������й�����
#define	ATT7022_REG_PosEpT			0x43		//���������й�����

#define	ATT7022_REG_NegEpA			0x44		//A�ฺ���й�����
#define	ATT7022_REG_NegEpB			0x45		//B�ฺ���й�����
#define	ATT7022_REG_NegEpC			0x46		//C�ฺ���й�����
#define	ATT7022_REG_NegEpT			0x47		//���ฺ���й�����

#define	ATT7022_REG_PosEqA			0x48		//A�������޹�����
#define	ATT7022_REG_PosEqB			0x49		//B�������޹�����
#define	ATT7022_REG_PosEqC			0x4a		//C�������޹�����
#define	ATT7022_REG_PosEqT			0x4b		//���������޹�����

#define	ATT7022_REG_NegEqA			0x4c		//A�ฺ���޹�����
#define	ATT7022_REG_NegEqB			0x4d		//B�ฺ���޹�����
#define	ATT7022_REG_NegEqC			0x4e		//C�ฺ���޹�����
#define	ATT7022_REG_NegEqT			0x4f		//���ฺ���޹�����

#define	ATT7022_REG_LineEpA			0x50		//A������й�����
#define	ATT7022_REG_LineEpB			0x51		//A������й�����
#define	ATT7022_REG_LineEpC			0x52		//A������й�����
#define	ATT7022_REG_LineEpT			0x53		//��������й�����

#define	ATT7022_REG_LineEqA			0x54		//A������޹�����
#define	ATT7022_REG_LineEqB			0x55		//A������޹�����
#define	ATT7022_REG_LineEqC			0x56		//A������޹�����
#define	ATT7022_REG_LineEqT			0x57		//��������޹�����

#define	ATT7022_REG_YUaUb			0x5c		//Ua��Ub�ĵ�ѹ�н�
#define	ATT7022_REG_YUaUc			0x5d		//Ua��Uc�ĵ�ѹ�н�	
#define	ATT7022_REG_YUbUc			0x5e		//Ub��Uc�ĵ�ѹ�н�
#define	ATT7022_REG_ChkSum2			0x5f		//У������У��Ĵ���


#define	ATT7022_REG_PosEpA2			0x60		//A�������й�����(��ȡһ�α����)
#define	ATT7022_REG_PosEpB2			0x61		//A�������й�����(��ȡһ�α����)
#define	ATT7022_REG_PosEpC2			0x62		//A�������й�����(��ȡһ�α����)
#define	ATT7022_REG_PosEpT2			0x63		//A�������й�����(��ȡһ�α����)

#define	ATT7022_REG_NegEpA2			0x64		//A�ฺ���й�����(��ȡһ�α����)
#define	ATT7022_REG_NegEpB2			0x65		//B�ฺ���й�����(��ȡһ�α����)
#define	ATT7022_REG_NegEpC2			0x66		//C�ฺ���й�����(��ȡһ�α����)
#define	ATT7022_REG_NegEpT2			0x67		//���ฺ���й�����(��ȡһ�α����)

#define	ATT7022_REG_PosEqA2			0x68		//A�������޹�����(��ȡһ�α����)
#define	ATT7022_REG_PosEqB2			0x69		//B�������޹�����(��ȡһ�α����)
#define	ATT7022_REG_PosEqC2			0x6a		//C�������޹�����(��ȡһ�α����)
#define	ATT7022_REG_PosEqT2			0x6b		//���������޹�����(��ȡһ�α����)

#define	ATT7022_REG_NegEqA2			0x6c		//A�ฺ���޹�����(��ȡһ�α����)
#define	ATT7022_REG_NegEqB2			0x6d		//B�ฺ���޹�����(��ȡһ�α����)
#define	ATT7022_REG_NegEqC2			0x6e		//C�ฺ���޹�����(��ȡһ�α����)
#define	ATT7022_REG_NegEqT2			0x6f		//���ฺ���޹�����(��ȡһ�α����)

//------------------------------------------------------------------------
//ATT7022B calibration register define(write only)
//------------------------------------------------------------------------

#define	REG_FailVoltage				0x29		//ʧѹ��ֵ����
#define	REG_EAddMode				0x2a		//���������ۼ�ģʽѡ��
#define	REG_GainAdc7				0x2b		//����·ADC��ЧֵУ��
#define	REG_GCtrlT7Adc				0x2c		//�¶�/����·ADCѡ�����
#define	REG_EnlineFreq				0x2d		//��������ʹ�ܿ���
#define	REG_EnUAngle				0x2e		//��ѹ�нǲ���ʹ�ܿ���
#define	REG_SelectPQSU				0x2f		//������ѹ�������ѡ��
#define	REG_EnDtIorder				0x30		//����������ʹ�ܿ���
#define	REG_LineFreqPg				0x31		//��������У��
#define	REG_UADCPga					0x3f		//��ѹͨ��ADC����ѡ��


//------------------------------------------------------------------------
//ATT7022B command set
//------------------------------------------------------------------------
#define	CLEAR_CALIDATA				0xC3		//У�������������
#define	READ_CALIDATA     			0xC6		//��У����������
#define	CALIDATA_WEN				0xC9		//У������дʹ��
#define	SOFTRST_CMD					0xd3		//�����λ����

#define	PHASE_A						0
#define	PHASE_B						1
#define	PHASE_C						2
#define	PHASE_T						3
#define PHASE_ADC7					16

//��λ����У������
#define	PHI_FIELD1					0.1f		//��λ����У������1(100mA����)
#define	PHI_FIELD2					0.5f		//��λ����У������2(500mA����)
#define	PHI_FIELD3					1.0f		//��λ����У������3(1A����)
#define	PHI_FIELD4					2.0f		//��λ����У������4(2A����)

#define	MAX_VALUE1					8388608.0f	// 2 ^ 23 ֵ
#define	SYS_KVALUE					8192.0f		// 2 ^ 13 ֵ
#define	MAX_VALUE2					16777216L	// 2 ^ 24 ֵ
#define	CONST_G						0.648f

#define ISTART_RATIO				0.001f				//����������ֵ(Ib * 0.002)
#define IB_VO						0.4f				//Ibʱ�������ĵ�ѹ����ź�(0.4V)
#define UCALI_CONST					220.0f				//��ѹУ������(220V��У��)
#define ICALI_CONST					ATT7022_CONST_IB	//����У������(5A��У��)Ibֵ
#define ICALI_MUL					1.0f				//����У��ʱIb���ı���

#define PCALI_CONST					(UCALI_CONST * ICALI_CONST)//1100				//����У������(220V/5A��У��, ��������Ϊ1)




//Public Typedefs
typedef struct {
	uint_t		ste;
	p_dev_spi	spi;
}t_att7022, *p_att7022;

typedef struct {
	  uint32_t Res1;
      uint32_t Res2;
      uint32_t Iregion1;		//��λ������������1
      uint32_t Iregion2;		//��λ������������2
      uint32_t Iregion3;		//��λ������������3
      uint32_t Iregion4;		//��λ������������4
      uint32_t Pgain0[3];		//A�๦������0
      uint32_t Pgain1[3];		//A�๦������1
      uint32_t PhsregA[5];		//A������0��λУ��   
      uint32_t PhsregB[5];		//B������0��λУ��   
      uint32_t PhsregC[5];		//C������0��λУ��   
      uint32_t Ugain[3];		//A���ѹУ��
      uint32_t Irechg;			//�Ȳ����������
      uint32_t Istarstup;		//�𶯵�����ֵ����   
      uint32_t HFConst;			//��Ƶ�����������
      uint32_t Res3;
      uint32_t Res4;
      uint32_t Res5;
      uint32_t Res6;
      uint32_t Res7;
      uint32_t Igain[3];		//A�����У��
      uint32_t FailVoltage;		// ʧѹ��ֵ����
      uint32_t EAddMode;		//���������ۼ�ģʽѡ��      
      uint32_t GainAdc7;		//����·ADC��ЧֵУ��    
      uint32_t GCtrlT7Adc;		//�¶�/����·ADCѡ�����       
      uint32_t EnLineFreq;		//��������ʹ�ܿ���       
      uint32_t EnUAngle;		//��ѹ�нǲ���ʹ�ܿ���     
      uint32_t SlectPQSU;		//������ѹ����ʹ�ܿ���     
      uint32_t ENDtIorder;		//����������ʹ�ܿ���       
      uint32_t LineFreqPg;		//��������У��        
      uint32_t Res8;
      uint32_t Res9;
      uint32_t Res10;
      uint32_t Res11;
      uint32_t Res12;
      uint32_t Res13;
      uint32_t Res14;
      uint32_t Res15;
      uint32_t Res16;
      uint32_t Res17;
      uint32_t EnHarm;			//����������г�������л�ѡ��   
      uint32_t Res18;
      uint32_t HFDouble;		//���峣���ӱ�ѡ��       
      uint32_t UADCPga;			//��ѹͨ��ADC����ѡ��       
      uint32_t nCRC;				//�������ݵ�У���
      uint32_t CalitFlage;		//У���ʶ 0��û��У�� 1��У���� 2�����У��      
}t_att7022_cali, *p_att7022_cali;











//External Functions
void att7022_Init(void);
sys_res att7022_Reset(p_att7022 p, p_att7022_cali pCali);
float att7022_GetFreq(p_att7022 p);
uint32_t att7022_GetPosEP(p_att7022 p, uint_t nPhase);
uint32_t att7022_GetNegEP(p_att7022 p, uint_t nPhase);
uint32_t att7022_GetPhaseEQ(p_att7022 p, uint_t nQuad, uint_t nDir, uint_t nPhase);
float att7022_GetVoltage(p_att7022 p, uint_t nPhase);
float att7022_GetCurrent(p_att7022 p, uint_t nPhase);
float att7022_GetPower(p_att7022 p, uint_t nPhase, uint_t nType);
uint32_t att7022_GetPowerDir(p_att7022 p);
float att7022_GetSV(p_att7022 p, uint_t nPhase);
float att7022_GetPFV(p_att7022 p, uint_t nPhase);
float att7022_GetPAG(p_att7022 p, uint_t nPhase);
float att7022_GetPVAG(p_att7022 p, uint_t nPhase);
uint16_t att7022_GetQuanrant(p_att7022 p, uint_t nPhase);
sys_res att7022_GetHarmonic(p_att7022 p, uint_t Ch, sint16_t* pbuf);
void att7022_UIP_gainCalibration(p_att7022 p, p_att7022_cali pCali);
void att7022_Phase_GainCalibration(p_att7022 p, p_att7022_cali pCali);
uint32_t att7022_Status(p_att7022 p);
uint32_t att7022_CheckSum1(p_att7022 p);
uint32_t att7022_CheckSum2(p_att7022 p);

sys_res att7022_WriteReg(p_att7022 p, uint_t nReg, uint32_t nData);
uint32_t att7022_ReadReg(p_att7022 p, uint_t nReg);
uint32_t att7022_UgainCalibration(p_att7022 p, uint8_t nPhase );
uint32_t att7022_PhaseCalibration(p_att7022 p,uint8_t nPhase, uint8_t cali_point);
float att7022_FPhaseCaliData(p_att7022 p,uint8_t nPhase, uint8_t cali_point);

#if 0
int att7022_ADC_ChannelSelect(uint32_t nChl);
float att7022_GetSV(uint8_t nPhase);
float att7022_GetPFV(uint8_t nPhase);
float att7022_GetPAG(uint8_t nPhase);
float att7022_GetPVAG(uint8_t nPhase);
float att7022_GetVoltage(uint8_t nPhase);
float att7022_GetPower(uint8_t nPhase, uint32_t nType);
void att7022_GetPosEP(uint8_t nPhase);
void att7022_GetNegEP(uint8_t nPhase);
void att7022_GetPhaseEQ(uint8_t nQuad, uint8_t nDir, uint8_t nPhase);
void att7022_GetEQ(uint8_t nQuad);
float att7022_GetCurrent(uint8_t nPhase);
float att7022_GetTemp(uint8_t cali);
uint32_t att7022_GetFlag(void);
uint8_t att7022_GetPowerDir(void);
float att7022_IBlanceCalc(void);
float att7022_VBlanceCalc(void);
void att7022_IstartupSet(void);
uint32_t att7022_PhaseCalibration(uint8_t nPhase, uint8_t cali_point);
float att7022_FPhaseCaliData(uint8_t nPhase, uint8_t cali_point);
uint8_t att7022_IregionSet(float current_value, uint8_t field_num);
uint32_t att7022_PgainCalibration(uint8_t nPhase);
uint32_t att7022_IgainCalibration(uint8_t nPhase);
uint32_t att7022_UgainCalibration(uint8_t nPhase);
void att7022_PhaseCali1Seg(float fCali);
void att7022_PhaseCali2Seg(uint8_t nPhase);
uint32_t att7022_PhaseCaliData(float err);
void att7022_UIP_GainCalibration(void);
void att7022_PhaseABC_Calibration(void);
void att7022_GainAD7(uint8_t channel);
S16 att7022_Getadc7(void);
uint16_t att7022_GetQuanrant(uint8_t Phase);
#endif

//��ʱδʹ�õĺ���
/*
float att7022_GetAD7(uint32 fat);
float att7022_IronlossCalc(void);
float att7022_CopperlossCalc(void);
*/


#ifdef __cplusplus
}
#endif

#endif

