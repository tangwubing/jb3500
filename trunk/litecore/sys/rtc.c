#if RTC_TYPE == RTC_T_ISL1202X
#include <drivers/isl1202x.c>
#elif RTC_TYPE == RTC_T_DS323X
#include <drivers/ds323x.c>
#elif RTC_TYPE == RTC_T_R202X
#include <drivers/r202x.c>
#elif RTC_TYPE == RTC_T_RX802X
#include <drivers/rx802x.c>
#elif RTC_TYPE == RTC_T_R802XSA
#include <drivers/r802xsa.c>
#endif

//Private Defines
#define RTC_DEBUG_METHOD			0


//Internal Variables
static time_t rtc_tTime = 0;
static struct tm rtc_Tm;



//External Functions
void rtc_OsTick()
{

	rtc_tTime += 1;
	localtime_r(&rtc_tTime, &rtc_Tm);
}


void rtc_Maintain()
{

#if RTC_TYPE & RTC_T_I2C
	p_dev_i2c p;

	if ((p = i2c_Get(RTC_COMID, 5000)) != NULL) {
		rtc_i2c_GetTime(p, &rtc_tTime);
		i2c_Release(p);
	}
#elif RTC_TYPE == RTC_T_INT
	rtc_tTime = arch_RtcGet();
#endif
	localtime_r(&rtc_tTime, &rtc_Tm);
}


//-------------------------------------------------------------------------
//rtc_Init - ��ʼ��RTC
//
//@
//
//Note: 
//
//Return: SYS_R_OK on success,  errno otherwise
//-------------------------------------------------------------------------
sys_res rtc_Init()
{
	sys_res res = SYS_R_OK;
#if RTC_TYPE & RTC_T_I2C
	p_dev_i2c p;

	if ((p = i2c_Get(RTC_COMID, OS_TMO_FOREVER)) != NULL) {
		res = rtc_i2c_Init(p);
		i2c_Release(p);
	} else 
		res = SYS_R_TMO;
#elif RTC_TYPE == RTC_T_INT
	arch_RtcInit();
#endif
	rtc_Maintain();
	return res;
}


//-------------------------------------------------------------------------
//rtc_GetTimet - ���time_t�ṹʱ��
//
//@
//
//Note: 
//
//Return: time_t on success,  errno otherwise
//-------------------------------------------------------------------------
time_t rtc_GetTimet()
{

	return rtc_tTime;
}

//-------------------------------------------------------------------------
//rtc_GetTm - ���struct tm *�ṹʱ��
//
//@tmTime:��Ż�ȡ��struct tm�ṹ���ݵ�ָ��
//
//Note: 
//
//Return: SYS_R_OK on success,  errno otherwise
//-------------------------------------------------------------------------
sys_res rtc_GetTm(struct tm *ptmTime)
{
	time_t tTime;

	tTime = rtc_GetTimet();
	localtime_r(&tTime, ptmTime);
	return SYS_R_OK;
}


//-------------------------------------------------------------------------
//rtc_GetTm - ���struct tm *�ṹʱ��
//
//@tmTime:��Ż�ȡ��struct tm�ṹ���ݵ�ָ��
//
//Note: 
//
//Return: SYS_R_OK on success,  errno otherwise
//-------------------------------------------------------------------------
struct tm *rtc_pTm()
{

	return &rtc_Tm;
}


//-------------------------------------------------------------------------
//rtc_SetTimet - ͨ��time_t�ṹ����ʱ��
//
//@tTime:���õ�ʱ��tTime�ṹ
//
//Note: 
//
//Return: SYS_R_OK on success,  errno otherwise
//-------------------------------------------------------------------------
sys_res rtc_SetTimet(time_t tTime)
{
	sys_res res = SYS_R_OK;
#if RTC_TYPE & RTC_T_I2C
	p_dev_i2c p;

	if ((p = i2c_Get(RTC_COMID, OS_TMO_FOREVER)) != NULL) {
		res = rtc_i2c_SetTime(p, tTime);
		i2c_Release(p);
	} else 
		res = SYS_R_TMO;
#elif RTC_TYPE == RTC_T_INT
	arch_RtcSet(tTime);
#endif
	rtc_tTime = tTime;
	return res;
}

//-------------------------------------------------------------------------
//rtc_SetTm - ͨ��struct tm *�ṹ����ʱ��
//
//@tmTime:���struct tm�ṹ���ݵ�ָ��
//
//Note: 
//
//Return: SYS_R_OK on success,  errno otherwise
//-------------------------------------------------------------------------
sys_res rtc_SetTm(struct tm *ptmTime)
{
	time_t tTime;

	tTime = mktime(ptmTime);
	return rtc_SetTimet(tTime);
}



