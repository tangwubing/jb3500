#ifndef __ECL_ACM_H__
#define __ECL_ACM_H__


#ifdef __cplusplus
extern "C" {
#endif


//Public Typedefs
typedef struct {
	float freq;		//
	float vol[3];	//
	float cur[4];	//
	float pp[4];	//
	float pq[4];	//
	float ui[4];	//
	float cos[4];	//
	float au[3];	//��ѹ�Ƕ�
	float ai[3];	//�����Ƕ�
}t_acm_rtdata;



















#ifdef __cplusplus
}
#endif


#endif

