#ifndef __GW3761_H__
#define __GW3761_H__


#ifdef __cplusplus
extern "C" {
#endif





//Public Defines
//����2009ͨѶ��Լ
#define GW3761_T_GWJC2009			0
//PB325���ͨѶ��Լ
#define GW3761_T_PB325				1
//��������2004ͨѶ��Լ
#define GW3761_T_GWFK2004			4
//��������2005ͨѶ��Լ
#define GW3761_T_GWFK2005			5
//���츺��2006ͨѶ��Լ
#define GW3761_T_CQFK2006			6


//��������2005ͨѶ��Լ
#define GW3761_T_NWFK2005			16
//��������2005ͨѶ��Լ
#define GW3761_T_NWFK2008			17



#if GW3761_TYPE == GW3761_T_GWJC2009
#define GW3761_PROTOCOL_ID			2
#define GW3761_PW_SIZE				16
#define GW3761_GROUPADR_QTY			8
#define GW3761_ECREPORT_ENABLE		0
#endif

#if GW3761_TYPE == GW3761_T_PB325
#define GW3761_PROTOCOL_ID			2
#define GW3761_PW_SIZE				16
#define GW3761_GROUPADR_QTY			8
#define GW3761_ECREPORT_ENABLE		1
#endif

#if GW3761_TYPE == GW3761_T_GWFK2004
#define GW3761_PROTOCOL_ID			1
#define GW3761_PW_SIZE				2
#define GW3761_GROUPADR_QTY			8
#define GW3761_ECREPORT_ENABLE		0
#endif

#if GW3761_TYPE == GW3761_T_GWFK2005
#define GW3761_PROTOCOL_ID			1
#define GW3761_PW_SIZE				16
#define GW3761_GROUPADR_QTY			8
#define GW3761_ECREPORT_ENABLE		0
#endif

#if GW3761_TYPE == GW3761_T_CQFK2006
#define GW3761_PROTOCOL_ID			1
#define GW3761_PW_SIZE				2
#define GW3761_GROUPADR_QTY			16
#define GW3761_ECREPORT_ENABLE		0
#endif

#if GW3761_TYPE == GW3761_T_NWFK2005
#define GW3761_PROTOCOL_ID			1
#define GW3761_PW_SIZE				2
#define GW3761_GROUPADR_QTY			8
#define GW3761_ECREPORT_ENABLE		0
#endif

#if GW3761_TYPE == GW3761_T_NWFK2008
#define GW3761_PROTOCOL_ID			1
#define GW3761_PW_SIZE				16
#define GW3761_GROUPADR_QTY			8
#define GW3761_ECREPORT_ENABLE		0
#endif




//���ͷ�����
#define GW3761_DIR_RECV             0   //��վ����
#define GW3761_DIR_SEND             1   //�ն˷���

//AFN
#define GW3761_AFN_CONFIRM			0x00
#define GW3761_AFN_LINKCHECK		0x02


//FUN
#define GW3761_FUN_RESET			1
#define GW3761_FUN_DATA_USER		4
#define GW3761_FUN_LINKCHECK		9
#define GW3761_FUN_DATA_L1			10
#define GW3761_FUN_DATA_L2			11

#define GW3761_FUN_CONFIRM			0
#define GW3761_FUN_RESPONSE			8
#define GW3761_FUN_NODATA			9
#define GW3761_FUN_LINKSTATE		11

//��Ч����ֵ
#define GW3761_DATA_INVALID			0xEEEEEEEE

//��Ϣ�ദ�����Ͷ���
#define GW3761_DT_T_DATA_CURRENT	0x01
#define GW3761_DT_T_DATA_HOUR		0x05
#define GW3761_DT_T_DATA_CURVE		0x0C
#define GW3761_DT_T_DATA_DAY		0x0D
#define GW3761_DT_T_DATA_MDAY		0x0E
#define GW3761_DT_T_DATA_MONTH		0x0F

#define GW3761_DT_T_PARAM_TER		0x10
#define GW3761_DT_T_PARAM_METER		0x11
#define GW3761_DT_T_PARAM_PULSE		0x12
#define GW3761_DT_T_PARAM_ANA		0x13
#define GW3761_DT_T_PARAM_GROUP		0x14
#define GW3761_DT_T_PARAM_DIFF		0x15
#define GW3761_DT_T_PARAM_FREEZE	0x16
#define GW3761_DT_T_PARAM_PORT		0x17

//��Ϣ�����Ͷ���
#define GW3761_DA_T_TER				0
#define GW3761_DA_T_TN				1
#define GW3761_DA_T_GROUP			2
#define GW3761_DA_T_ANA				3

//��Լ�������Ͷ���
#define GW3761_DATA_T_01			1
#define GW3761_DATA_T_02			2
#define GW3761_DATA_T_03			3
#define GW3761_DATA_T_04			4
#define GW3761_DATA_T_05			5
#define GW3761_DATA_T_06			6
#define GW3761_DATA_T_07			7
#define GW3761_DATA_T_08			8
#define GW3761_DATA_T_09			9
#define GW3761_DATA_T_10			10
#define GW3761_DATA_T_11			11
#define GW3761_DATA_T_12			12
#define GW3761_DATA_T_13			13
#define GW3761_DATA_T_14			14
#define GW3761_DATA_T_15			15
#define GW3761_DATA_T_16			16
#define GW3761_DATA_T_17			17
#define GW3761_DATA_T_18			18
#define GW3761_DATA_T_19			19
#define GW3761_DATA_T_20			20
#define GW3761_DATA_T_21			21
#define GW3761_DATA_T_22			22
#define GW3761_DATA_T_23			23
#define GW3761_DATA_T_24			24
#define GW3761_DATA_T_25			25
#define GW3761_DATA_T_26			26
#define GW3761_DATA_T_27			27

#define GW3761_DATA_T_BIN			40
#define GW3761_DATA_T_TIME			41
#define GW3761_DATA_T_PERCENT		42
#define GW3761_DATA_T_OTHER			43
#define GW3761_DATA_T_CONTROL		44
#define GW3761_DATA_T_CONST			45
#define GW3761_DATA_T_RDONLY		46
#define GW3761_DATA_T_RATE			47
#define GW3761_DATA_T_DYNAMIC		60
#define GW3761_DATA_T_INDEX			61
#define GW3761_DATA_T_REPEAT		62
#define GW3761_DATA_T_FREEZEPARAM	63

#define GW3761_DATA_T_DIVIDE		0x40
#define GW3761_DATA_T_MULTI			0x80



//Public Typedefs
//֡������
typedef __packed struct {
	uint8_t	fun : 4,
			fcv : 1,
			fcb_acd : 1,
			prm : 1,
			dir : 1;
}t_gw3761_c;

//֡������
typedef __packed struct {
	uint8_t	seq : 4,
			con : 1,
			fin : 1,
			fir : 1,
			tpv : 1;
}t_gw3761_seq;

//ʱ���ǩ��
typedef __packed struct {
	uint8_t	pfc;
	uint8_t	tp[4];
	uint8_t	dly;
}t_gw3761_tp;

//GDW3761����֡
typedef struct {
 	uint16_t		a1;			//����������
 	uint16_t		a2;			//�ն˵�ַ
	uint8_t			group : 1,
					msa : 7;
	t_gw3761_c		c;
	uint8_t			afn;
	t_gw3761_seq	seq;
	t_gw3761_tp		tp;
	uint8_t			pw[GW3761_PW_SIZE];
	buf				data;
}t_gw3761_rmsg;

//GDW3761��Լ�ṹ
typedef struct {
	t_dlrcp		parent;
 	uint16_t	rtua;
 	uint16_t	terid;
	t_gw3761_rmsg rmsg;
}t_gw3761, *p_gw3761;



//GDW3761���ݽṹ
typedef const struct {
	uint8_t		type; 		//��������
	uint8_t		size; 		//����Ϣ��Ԫ���ݴ�С
	uint16_t	para; 		//���Ӳ���
	uint16_t	reg; 		//��Ӧ�Ĵ������
}t_gw3761_item, *p_gw3761_item;

typedef const struct {
	uint8_t		dttype : 5, //��Ϣ�ദ������
				datype : 3;	//��Ϣ������
	uint8_t		qty; 		//����������
	uint16_t	dtid;			//��Ϣ����
	p_gw3761_item item; //��ӦDT1��ĵ�ַ
}t_gw3761_dt, *p_gw3761_dt;



//External Functions
void gw3761_Init(p_gw3761 p);
sys_res gw3761_Transmit(p_gw3761 p, p_gw3761 pD);
int gw3761_RecvCheck(p_gw3761 p);
sys_res gw3761_Handler(p_gw3761 p);

void gw3761_Response(p_gw3761 p);

sys_res gw3761_TmsgSend(p_gw3761 p, uint_t nFun, uint_t nAfn, buf b, uint_t nType);
sys_res gw3761_TmsgConfirm(p_gw3761 p);
sys_res gw3761_TmsgReject(p_gw3761 p);

uint_t gw3761_ConvertDa2DA(uint_t nDa);
uint_t gw3761_ConvertFn2DT(uint_t nFN);
uint_t gw3761_ConvertDt2Fn(uint_t nDT);
uint_t gw3761_ConvertDa2Map(uint_t nDA, void *pData);
void gw3761_ConvertData(void *p, uint32_t nData, uint_t nDec, uint32_t nMark, uint_t nSignBit, uint_t nSize, uint_t nSign);
void gw3761_ConvertData_Time(uint8_t *p, time_t tTime, uint_t nType);
void gw3761_ConvertData_01(void *p, time_t tTime);
void gw3761_ConvertData_03(void *p, uint32_t nData, uint_t nSign);
void gw3761_ConvertData_05(void *p, uint32_t nData, uint_t nSign);
void gw3761_ConvertData_05_Percent(void *p, uint32_t nData, uint_t nSign);
void gw3761_ConvertData_06(void *p, uint32_t nData, uint_t nSign);
void gw3761_ConvertData_07(void *p, uint32_t nData);
void gw3761_ConvertData_09(void *p, uint32_t nData, uint_t nSign);
void gw3761_ConvertData_11(void *p, uint32_t nData);
void gw3761_ConvertData_13(void *p, uint32_t nData);
void gw3761_ConvertData_14(uint8_t *p, float nData);
void gw3761_ConvertData_15(void *p, time_t tTime);
void gw3761_ConvertData_17(void *p, time_t tTime);
void gw3761_ConvertData_18(void *p, time_t tTime);
void gw3761_ConvertData_22(void *p, uint32_t nData);
void gw3761_ConvertData_23(void *p, uint32_t nData);
void gw3761_ConvertData_25(void *p, uint32_t nData, uint_t nSign);
void gw3761_ConvertData_26(void *p, uint32_t nData);
void gw3761_ConvertData_THD(uint_t nDa, uint_t nPulse, void *pBuf);





#ifdef __cplusplus
}
#endif


#endif


