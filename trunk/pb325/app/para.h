#ifndef __APP_PARA_H__
#define __APP_PARA_H__

#ifdef __cplusplus
extern "C" {
#endif



//Public Defines
#define ECL_SN_MAX					4
#define ECL_TN_MAX					ECL_SN_MAX


//HC595 OutPut
#define LED_RUN(x)      gpio_Set(0, x)			//�͵�ƽ��Ч
#define LCD_BL(x)       gpio_Set(2, (x) ^ 1)	//�͵�ƽ��Ч
#define BEEP(x)         gpio_Set(3, x)			//�ߵ�ƽ��Ч


//Public Typedefs
//�ն�����ͨѶ�ڲ���
typedef __packed struct {
	uint8_t		rts;
	uint8_t		delay;
	uint16_t	tmo;
	uint8_t		con;
	uint8_t		span;
}t_afn04_f1;

//��վIP��ַ�Ͷ˿�
typedef __packed struct {
	uint8_t		ip1[4];
	uint16_t	port1;
	uint8_t		ip2[4];
	uint16_t	port2;
	char		apn[16];
}t_afn04_f3;

//�ն��¼����ò���
typedef __packed struct {
	uint64_t	valid;
	uint64_t	imp;
}t_afn04_f9;

//���������
typedef __packed struct {
	uint16_t	tn;			//�����������
	uint8_t		port : 5,	//ͨ�����ʼ��˿ں� D7��D5�����ʾ���ܱ���������װ�����ն˵�ͨ�Ų����ʣ� 1��7���α�ʾ600��1200��2400��4800��7200��9600��19200��0����ʾ�������û�ʹ��Ĭ�ϵģ����籾ͨ�Ŷ˿ںŵĽӿڼ�ͨ������Ϊ2ʱ�������˿����Դ��нӿ���խ����ѹ�ز�ͨ��ģ�������ӣ��μ�������5.9.2.4.2��������ݣ�������ֵ�����壬��Ϊ0��ʵ��ʹ�õ�ͨ�����ʰ�������5.5.1.3.32������ִ�С�
				baud : 3;	//D4��D0�����ʾ���ܱ���������װ�����ն���������Ӧ���ն�ͨ�Ŷ˿ںţ���ֵ��Χ1��31������ֵ��Ч��
	uint8_t		prtl; 		//ͨ�Ź�Լ����ͨ��Э�����ͣ���ֵ��Χ0��255������0����ʾ�ն�����Ա���ŵĵ��ܱ�/��������װ�ý��г���1��DL/T 645-1997.��2����������װ��ͨ��Э�飻30��DL/T 645-2007��31��"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э�飻���������á�
	uint8_t		madr[6];	//���ַ��ֵ��Χ0��999999999999��
	uint8_t		pwd[6];		//�������ն�����ܱ�ͨ�ŵ����롣
	uint8_t		rate;		//�����ʸ���
	uint8_t		energy;		//�й�����ʾֵ����λ��С��λ������ D3��D2�������ʾͨ�Ž���ĵ��ܱ���й�����ʾֵ������λ��������ֵ��Χ0��3���α�ʾ4��7λ����
							//D1��D0�������ʾͨ�Ž���ĵ��ܱ���й�����ʾֵ��С��λ��������ֵ��Χ0��3���α�ʾ1��4λС����
	uint8_t		tadr[6];	//��Ӧ�ı���ն˵�ַ  
	uint8_t		usertype;	//�û�����ż��û�С���   �߸�4λ����𣬵�4λΪС��� 0ȱʡ1������ 2���� 
							// D7��D4�������ʾ�����ܱ��������û�����ţ���ֵ��Χ0��15�����α�ʾ16���û�����š�
							// D3��D0�������ʾ�����ܱ��������û�С��ţ���ֵ��Χ0��15�����α�ʾ16��1���2������������ã�Ӧ���ն�֧�ֵ�1���2���������÷�Χ�ڣ�
}t_afn04_f10;

//�ܼ������ò���
typedef __packed struct {
	uint8_t		qty;
	uint8_t		tn[64];
}t_afn04_f14;

//���ʲ���
typedef __packed struct {
	uint8_t		qty;
	uint32_t	rate[16];
}t_afn04_f22;

//�������������
typedef __packed struct {
	uint16_t	PT;
	uint16_t	CT;
	uint8_t		RatedU[2];
	uint8_t		RatedI;
	uint8_t		RatedCP[3];
	uint8_t		LineStyle : 2,
				SinglePhasic : 2,
				reserve : 4;
}t_afn04_f25;

//�ն˳������в���
typedef __packed struct {
	uint16_t	flag;
	uint32_t	date;
	uint8_t		time[2];
	uint8_t		span;
	uint8_t		checktime[3];
	uint8_t		qty;
	uint32_t	timeinfo[24];
}t_afn04_f33;

//�ն˵�ַ
typedef __packed struct {
	uint16_t	mfcode;
	uint16_t	area;
	uint16_t	addr;
}t_afn04_f85;







//External Functions
int icp_ParaRead(uint_t nAfn, uint_t nFn, uint_t nPn, void *pBuf, uint_t nLen);
int icp_ParaWrite(uint_t nAfn, uint_t nFn, uint_t nPn, const void *pBuf, uint_t nLen);
int icp_MeterRead(uint_t nSn, t_afn04_f10 *p);
void icp_MeterWrite(uint_t nSn, t_afn04_f10 *p);
int icp_Meter4Tn(uint_t nTn, t_afn04_f10 *p);
void icp_RunTimeWrite(time_t tTime);
int icp_RunTimeRead(time_t *pTime);
void icp_Clear(void);
void icp_Init(void);
void icp_UdiskLoad(void);


#ifdef __cplusplus
}
#endif

#endif


