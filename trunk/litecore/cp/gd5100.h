#ifndef __GD5100_H__
#define __GD5100_H__


//Public Defines

//���ͷ�����
#define GD5100_CDIR_RECV				0x00	//��վ����
#define GD5100_CDIR_SEND				0x01	//�ն˷���


//�������
#define GD5100_ERR_NO_ERROR				0x00	//��ȷ,�޴���
#define GD5100_ERR_RELAY_FALE			0x01	//�м�����û�з���
#define GD5100_ERR_INVALID				0x02	//�������ݷǷ�
#define GD5100_ERR_NO_PERMIT			0x03	//����Ȩ�޲���
#define GD5100_ERR_NO_DATA				0x04	//�޴�������
#define GD5100_ERR_TIME_INVALID			0x05	//����ʱ��ʧЧ
#define GD5100_ERR_NO_ADDR				0x11	//Ŀ���ַ������
#define GD5100_ERR_SEND_FAIL			0x12	//����ʧ��
#define GD5100_ERR_SMS_LONG				0x13	//����Ϣ̫֡��

#define GD5100_DATA_INVALID				0xFFFFFFFF



//Public Typedefs

//���ձ��Ľṹ
typedef struct {
 	uint16_t	rtua;			//����������
 	uint16_t	terid;			//�ն˵�ַ
 	uint16_t	ste : 6,		//״̬
 				fseq : 7,		//֡���
 				iseq : 3;		//֡�����
 	uint8_t		code : 6,		//������
 				abn : 1,		//�쳣��־
 				dir : 1;		//���ͷ���
 	buf			data;			//��������
}gd5100_rmsg[1];


//���ͱ��Ľṹ
typedef struct {
	uint8_t	ste : 4,
			retry : 4;
	uint8_t	tmo;
	buf		data;
}t_gd5100_tmsg, *gd5100_tmsg;


//��Լ�ṹ
typedef struct {
	t_dlrcp		parent;
	uint8_t		ste;
	uint8_t		pwd[3];
 	uint16_t	rtua;
 	uint16_t	terid;
	gd5100_rmsg	rmsg;
}t_gd5100, *p_gd5100;






//External Functions
void gd5100_Init(p_gd5100 p);
sys_res gd5100_TmsgSend(p_gd5100 p, uint_t nCode, buf b, uint_t nType);
sys_res gd5100_TmsgError(p_gd5100 p, uint_t nCode, uint_t nErr);
sys_res gd5100_Transmit(p_gd5100 p, p_gd5100 pD);
int gd5100_RecvCheck(p_gd5100 p);
sys_res gd5100_Handler(p_gd5100 p);

void gd5100_Response(p_gd5100 p);



#endif

