#ifndef __APP_METER_H__
#define __APP_METER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "para.h"
#include "route.h"


//Public Defines
#define ECL_PORT_ACM				1
#define ECL_PORT_RS485				2
#define ECL_PORT_CASCADE			30
#define ECL_PORT_PLC				31

#define ECL_PRTL_DLT645_97			1
#define ECL_PRTL_ACM				2
#define ECL_PRTL_DLT645_07			30


//Modem Type Defines
#define GW3762_T_XIAOCHENG			0
#define GW3762_T_EASTSOFT_RT		1
#define GW3762_T_EASTSOFT_38		2


//Public Typedefs
typedef const struct {
	uint8_t		flag;
	uint8_t		type;
	uint16_t	di97;
	uint32_t	di07;
}t_ecl_rtdi;

typedef struct {
	uint8_t			ste;
	uint8_t			tmo;
	uint8_t			retry;
	uint8_t			rtid;
	uint8_t			cycle;
	uint8_t			time[3];
	uint16_t		sn;
	uint32_t		di;
	void *			chl;
	t_afn04_f10 	f10;
}t_ecl_task;



//External Functions
void tsk_Meter(void *args);

sys_res ecl_485_RealRead(buf b, uint_t nBaud, uint_t nTmo);




#ifdef __cplusplus
}
#endif

#endif

