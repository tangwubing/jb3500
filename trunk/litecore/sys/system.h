#ifndef __SYSTEM_H__
#define __SYSTEM_H__


#ifdef __cplusplus
extern "C" {
#endif






#include <sys/dev.h>

#include <sys/battery.h>
#include <sys/gpio.h>
#include <sys/i2c.h>
#include <sys/iap.h>
#include <sys/interrupt.h>
#include <sys/rtc.h>
#include <sys/spi.h>
#include <sys/wdg.h>

#if OS_TYPE
#include <sys/uart.h>
#endif


//External Functions
void sys_Init(void);
void sys_Start(void);
void sys_Reset(void);
void sys_Delay(volatile uint_t n);


#ifdef __cplusplus
}
#endif

#endif

