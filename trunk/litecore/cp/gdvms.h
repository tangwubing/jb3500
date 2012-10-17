#ifndef __GDVMS_H__
#define __GDVMS_H__


//Public Defines
#define GDVMS_ADR_SIZE					6

//���ͷ�����
#define GDVMS_CDIR_RECV					0x00	//��վ����
#define GDVMS_CDIR_SEND					0x01	//�ն˷���


//�������
#define GDVMS_ERR_NO_ERROR				0x00	//��ȷ,�޴���
#define GDVMS_ERR_RELAY_FALE			0x01	//�м�����û�з���
#define GDVMS_ERR_INVALID				0x02	//�������ݷǷ�
#define GDVMS_ERR_NO_PERMIT				0x03	//����Ȩ�޲���
#define GDVMS_ERR_NO_DATA				0x04	//�޴�������
#define GDVMS_ERR_TIME_INVALID			0x05	//����ʱ��ʧЧ
#define GDVMS_ERR_NO_ADDR				0x11	//Ŀ���ַ������
#define GDVMS_ERR_SEND_FAIL				0x12	//����ʧ��
#define GDVMS_ERR_SMS_LONG				0x13	//����Ϣ̫֡��

#define GDVMS_DATA_INVALID				0xFFFFFFFF



//Public Typedefs
//��Լ�ṹ
typedef struct {
	t_dlrcp		parent;
	uint8_t		ste;
	uint8_t		adr[GDVMS_ADR_SIZE];
	uint8_t		fseq;			//֡���
	uint8_t		code : 6,		//������
				abn : 1,		//�쳣��־
				dir : 1;		//���ͷ���
	buf			data;			//��������
}t_gdvms, *p_gdvms;






//External Functions
void gdvms_Init(p_gdvms p);
sys_res gdvms_TmsgSend(p_gdvms p, uint_t nCode, buf b, uint_t nType);
sys_res gdvms_TmsgError(p_gdvms p, uint_t nCode, uint_t nErr);
sys_res gdvms_Transmit(p_gdvms p, p_gdvms pD);
sys_res gdvms_Handler(p_gdvms p);

void gdvms_Response(p_gdvms p);



#endif

