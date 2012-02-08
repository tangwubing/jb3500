#ifndef __APP_SYSTEM_H__
#define __APP_SYSTEM_H__


#ifdef __cplusplus
extern "C" {
#endif




//HC595 OutPut
#define LED_RUN(x)      gpio_Set(0, (x) ^ 1)	//�͵�ƽ��Ч
#define LCD_BL(x)       gpio_Set(2, (x) ^ 1)	//�͵�ƽ��Ч
#define BEEP(x)         gpio_Set(3, x)			//�ߵ�ƽ��Ч


//Public Variables
extern volatile uint_t g_sys_status;



#ifdef __cplusplus
}
#endif


#endif

