#ifndef __GW3762_H__
#define __GW3762_H__


#ifdef __cplusplus
extern "C" {
#endif

//Public Defines

//GW376.2 AFN Defines
#define GW3762_AFN_CONFIRM			0x00
#define GW3762_AFN_RESET			0x01
#define GW3762_AFN_TRANSMIT			0x02
#define GW3762_AFN_DATA_FETCH		0x03
#define GW3762_AFN_DATA_SET			0x05
#define GW3762_AFN_ROUTE_FETCH		0x10
#define GW3762_AFN_ROUTE_SET		0x11
#define GW3762_AFN_TRANSMIT_ROUTE	0x13
#define GW3762_AFN_AUTOREPORT		0xF0





//Public Typedefs
typedef __packed struct {
	uint8_t		fun : 6,
				prm : 1,
				dir : 1;
}t_gw3762_c;

typedef __packed struct {
	uint8_t		route : 1,
				addit : 1,
				module : 1,
				clash : 1,
				relay : 4;
	uint8_t		chlid : 4,
				eccid : 4;
	uint8_t		acklen;
	uint16_t	rate : 15,
				unit : 1;
	uint8_t		reserve;
}t_gw3762_rdown;


typedef __packed struct {
	uint8_t		route : 1,
				addit : 1,
				module : 1,
				clash : 1,
				relay : 4;
	uint8_t		chlid : 4,
				eccid : 4;
	uint8_t		pulse : 4,
				chltype : 4;
	uint8_t		cmdq: 4,
				ackq: 4;
	uint16_t	reserve;
}t_gw3762_rup;


//GDW3761����֡
typedef struct {
	t_gw3762_c		c;
	t_gw3762_rup	rup;
	uint8_t			afn;
	uint16_t		fn;
	uint8_t			madr[6];
	uint8_t			adr[6];
	buf 			data;
}t_gw3762_rmsg;

typedef struct {
	uint8_t			type;
	uint8_t			adr[6];
	chl 			chl;
	buf				rbuf;
	t_gw3762_rmsg	rmsg;
}t_gw3762;

typedef struct {
	uint8_t		manualType;
	uint8_t		ready;
	uint8_t		ret;
	uint8_t		meterNo; //������
	char		stateStr[32];
	char		resStr[32];
	uint8_t		ctrPara[32];
	//����ͨ�Ų���
	uint8_t		cn;
	uint8_t		pw;
	uint8_t		baud;
	uint8_t		wID[2];
	uint8_t		rType;
	uint8_t		rsv[2];
	//·������״̬
	uint8_t		rsBit;
	uint16_t	aNum;
	uint16_t	sNum;
	uint8_t 	rsState;

}t_gw3762_info;

//External Functions
void gw3762_Init(t_gw3762 *p);
sys_res gw3762_Analyze(t_gw3762 *p);
sys_res gw3762_Transmit2Module(t_gw3762 *p, uint_t nAfn, uint_t nDT, const void *pData, uint_t nLen);
sys_res gw3762_Transmit2Meter(t_gw3762 *p, uint_t nAfn, uint_t nDT, const void *pAdr, uint_t nRelay, const void *pRtAdr, const void *pData, uint_t nLen);

sys_res gw3762_HwReset(t_gw3762 *p, uint_t nTmo);
sys_res gw3762_InfoGet(t_gw3762 *p, uint_t nTmo);
sys_res gw3762_ModAdrSet(t_gw3762 *p, const void *pAdr, uint_t nTmo);
sys_res gw3762_SubAdrRead(t_gw3762 *p, uint_t nSn, uint8_t *pAdr, uint_t nTmo);
sys_res gw3762_SubAdrAdd(t_gw3762 *p, uint_t nSn, const void *pAdr, uint_t nTmo);
sys_res gw3762_SubAdrDelete(t_gw3762 *p, const void *pAdr, uint_t nTmo);



#ifdef __cplusplus
}
#endif


#endif
