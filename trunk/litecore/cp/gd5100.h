#ifndef __GD5100_H__
#define __GD5100_H__


//Public Defines

//ͨѶȨ��
#define GD5100_PWD_LOW					0x01
#define GD5100_PWD_HIGH					0x02
#define GD5100_PWD_ALL					(GD5100_PWD_LOW | GD5100_PWD_HIGH)


//�������
#define GD5100_ERR_NO_ERROR				0x00	//��ȷ,�޴���
#define GD5100_ERR_RELAY_FALE			0x01	//�м�����û�з���
#define GD5100_ERR_INVALUE				0x02	//�������ݷǷ�
#define GD5100_ERR_NO_PERMIT			0x03	//����Ȩ�޲���
#define GD5100_ERR_NO_DATA				0x04	//�޴�������
#define GD5100_ERR_TIME_INVALUE			0x05	//����ʱ��ʧЧ
#define GD5100_ERR_NO_ADDR				0x11	//Ŀ���ַ������
#define GD5100_ERR_SEND_FALE			0x12	//����ʧ��
#define GD5100_ERR_SMS_LONG				0x13	//����Ϣ̫֡��

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
	uint8_t		tmo;			//��ʱ����
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
	uint16_t	group;
 	uint16_t	rtua;
 	uint16_t	terid;
	gd5100_rmsg	rmsg;
}t_gd5100, gd5100[1];






//External Functions
void gd5100_Init(gd5100 p);
sys_res gd5100_TmsgSend(gd5100 p, uint_t nCode, buf b, uint_t nType);
sys_res gd5100_TmsgError(gd5100 p, uint_t nCode, uint_t nErr);
sys_res gd5100_Handler(gd5100 p);



#endif

