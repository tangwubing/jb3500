#ifndef __GW3761_DATA_H__
#define __GW3761_DATA_H__

#ifdef __cplusplus
extern "C" {
#endif




//Public Typedefs
typedef struct {
	uint16_t	sn;
	uint8_t		port : 4,
				baud : 4;
	uint8_t		prtl;				//ͨ�Ź�Լ����
	uint8_t		madr[6];			//���ַ
	uint8_t		pw[6];				//������
	uint8_t		rate;				//������
	uint8_t		bits;				//�й�����ʾֵ����λ��С��λ����
	uint8_t		tadr[6];			//�ɼ��ն˵�ַ  
	uint8_t		stype : 4,			//�û�С���
				btype : 4;			//�û������
}t_gw3761_meterparam;


//External Functions



#ifdef __cplusplus
}
#endif

#endif








