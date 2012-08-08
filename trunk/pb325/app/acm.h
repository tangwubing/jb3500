#ifndef __APP_ACM_H__
#define __APP_ACM_H__

#ifdef __cplusplus
extern "C" {
#endif

//Header Files
#include "para.h"
#include "data.h"

//Public Defines
#define ACM_MSAVE_VOL		0
#define ACM_MSAVE_CUR		6
#define ACM_MSAVE_PP		18
#define ACM_MSAVE_PQ		30
#define ACM_MSAVE_COS		42



//Public Typedefs
typedef struct {
	float	freq;	//
	float	u[3];	//
	float	i[4];	//
	float	pp[4];	//
	float	pq[4];	//
	float	ui[4];	//
	float	cos[4];	//
	float	ua[3];	//��ѹ�Ƕ�
	float	ia[3];	//�����Ƕ�
	float	ub;		//��ѹ��ƽ���
	float	ib;		//������ƽ���
	float	uib;	//���ز�ƽ���
}t_acm_rtdata;

typedef struct {
	float	base;
	float	xbrate[11];
}t_acm_xbdata;





//Public Variables
extern t_acm_rtdata acm_rtd;
extern t_acm_xbdata acm_uxb[3];
extern t_acm_xbdata acm_ixb[3];




//External Functions
void acm_Init(void);
void acm_Balance(t_acm_rtdata *pa);
void acm_JLRead(void);
void acm_XBRead(void);
void acm_MinSave(const uint8_t *pTime);
void acm_QuarterSave(const uint8_t *pTime);
int acm_IsReady(void);

void stat_Clear(void);
void stat_Handler(p_stat ps, t_acm_rtdata *pa, t_afn04_f26 *pF26, t_afn04_f28 *pF28, time_t tTime);




#ifdef __cplusplus
}
#endif

#endif

