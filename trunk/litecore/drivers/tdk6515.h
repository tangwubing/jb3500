#ifndef __TDK6515_H__
#define __TDK6515_H__


#ifdef __cplusplus
extern "C" {
#endif




//Public Typedefs
typedef struct {
	uint_t		ste;
	p_dev_uart	uart;
}t_tdk6515;




// =============================================================
// �Ӽ���IC�ж�ȡ��ԭʼֵ,���ջ�ת������Ӧ�Ľ���
// =============================================================
typedef struct {
    sint32_t	temp;		// 1. �¶� S32	0.1��
    uint32_t	freq;		// 2. Ƶ�� U32	0.01HZ
    float		enp[4];		// 3. ����ABC�й�����  Float
    float		enq[4];     // 7. ����ABC�޹�����  Float
    float		ahour[4];	// 11.ABCN�ల��ƽ��Сʱ Float
    float		vhour[3];	// 15.ABC�����ƽ��Сʱ  Float
    uint32_t	caangle;	// 18.CA���ѹ������   U32		0.1DEC
    uint32_t	cbangle;	// 19.CB���ѹ������	U32		0.1DEC
    uint32_t	reserve;	// 20
    uint32_t	vover0num;	// 21.��ѹ�������	U32
    float		p[4];		// 22.��ABC���й�����    Float
    float		q[4];		// 26.��ABC���޹�����	Float
    float		vi[4];		// 30.��ABC���ڹ���		Float
    float		viangle[4];	// 34.��ABC��ѹ�������	Float
    float		cos[4];	// 38.��ABC��������	Float
    float		i[4];	// 42.ABCN������Чֵ	Float
    float		v[3];	// 46.ABC���ѹ��Чֵ	Float
    float		ptneed;	// 49.�й���������		Float
    float		qtneed;	// 50.�޹���������		Float
    float		ppneed;	// 51.�й���������		Float
    float		qpneed;	// 52.�޹���������		Float
    uint_t   workr;	// 53.��������״̬�Ĵ���	SW
    uint_t   powerdir;	// 54.���ʷ���Ĵ���		SW
    uint_t   netstatus;	// 55.��������״̬��		SW
    uint_t   connstatus;// 56.��������״̬��		SW
    uint_t   adjustsum;	// 57. У������У��ͼĴ���		U32
    uint_t   lasttx;	// 58. ��һ��TX����ֵ�Ĵ���		
    uint_t   uab;	// 59. AB�ߵ�ѹ��Чֵ
    uint_t   ubc;	// 60. BC�ߵ�ѹ��Чֵ
    uint_t   uac;	// 61. AC�ߵ�ѹ��Чֵ
    uint_t   ppulse;	// 62. �й����������
    uint_t   qpulse;	// 63. �޹����������
    uint_t   pulse1;	// 64. Զ�����������1
    uint_t   pulse2;	// 65. Զ�����������2
    uint_t   pulse3;	// 66. Զ�����������3
    uint_t   pulse4;	// 67. Զ�����������4
    uint_t   pulse5;	// 68. Զ�����������5
    uint_t   ua1min;	// 69. A���ѹһ����ƽ��ֵ
    uint_t   ub1min;	// 70. B���ѹһ����ƽ��ֵ
    uint_t   uc1min;	// 71. C���ѹһ����ƽ��ֵ
}t_tdk6515_rtdata;
	

// =============================================================
// �Ӽ���IC�ж�ȡ��ԭʼֵ,����ת�����˸�������������
// =============================================================
typedef struct {
	uint_t	curxbno;	// 72.��ǰг������ͨ����
	float	xbrate[21];	// 73.��ǰͨ����2-21��г��������
	float	xbbase;	// 94. ��ǰг��ͨ��������Чֵ
	uint_t	xbbasefreq;// 95. ��ǰг������Ƶ��
}t_tdk6515_xbdata;




//External Functions
sys_res tdk6515_IsJLReady(void);
sys_res tdk6515_IsXBReady(void);


#ifdef __cplusplus
}
#endif

#endif

