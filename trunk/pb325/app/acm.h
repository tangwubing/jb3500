#ifndef __APP_ACM_H__
#define __APP_ACM_H__

#ifdef __cplusplus
extern "C" {
#endif


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
void acm_JLRead(void);
void acm_XBRead(void);
int acm_IsReady(void);


#ifdef __cplusplus
}
#endif

#endif

