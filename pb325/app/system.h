#ifndef __APP_SYSTEM_H__
#define __APP_SYSTEM_H__


#ifdef __cplusplus
extern "C" {
#endif




//HC595 OutPut
#define LED_RUN(x)      gpio_Set(0, (x) ^ 1)	//低电平有效
#define LCD_BL(x)       gpio_Set(2, (x) ^ 1)	//低电平有效
#define BEEP(x)         gpio_Set(3, x)			//高电平有效

//System Status Defines
#define SYS_STATUS_LOGIN	1


//Public Variables
extern volatile uint_t g_sys_status;



//External Functions
int sys_IsUsbFormat(void);


#ifdef __cplusplus
}
#endif


#endif

