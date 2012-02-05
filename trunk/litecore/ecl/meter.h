#ifndef __METER_H__
#define __METER_H__





typedef struct {
	uint8_t	ver;
	uint8_t	type;
	uint8_t	adr[8];
	uint16_t	ct;
	uint16_t	pt;
	union {
		t_md645_para md645;
		t_mcarr_para mcarr;
	}para;
}meter_para[1];


typedef struct {










}meter_data[1];



typedef struct {
	t_uart_para	x485;
	uint8_t		port;
	uint8_t 		aUser[6];
	uint8_t		aPwd[6];
}t_md645_para;

typedef struct {
/*	//�̶��м����
	uint16 arrFixRelay[4];
	//�Զ��м�Լ������
	uint8 arrMIU[6];		//�ɼ��ն˵�ַ
	uint16 nLine;			//��·���
	uint16 nMeterBox;		//������
	uint16 nMultiModel;	//���ģ����
	uint8 nPhase;			//Լ�����������
	//�ز�������λ
	uint8 nFirstPhase;
*/
}t_mcarr_para;





#endif

