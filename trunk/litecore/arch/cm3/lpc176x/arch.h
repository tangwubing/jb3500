#ifndef __ARCH_ARCH_H__
#define __ARCH_ARCH_H__

#ifdef __cplusplus
extern "C" {
#endif


//Header Files
#include <arch/cm3/lpc176x/LPC17xx.H>

#include <arch/cm3/lpc176x/emac.h>
#include <arch/cm3/lpc176x/gpio.h>
#include <arch/cm3/lpc176x/timer.h>
#include <arch/cm3/lpc176x/uart.h>

#include <arch/cm3/lpc176x/it.h>


#if MCU_FREQUENCY == MCU_SPEED_LOW
#error "not support low speed yet"
#elif MCU_FREQUENCY == MCU_SPEED_HALF
#error "not support half speed yet"
#else
#define MCU_CLOCK			__CORE_CLK
#endif

#define PERI_CLOCK			(__CORE_CLK / 4)






//External Functions
void arch_Init(void);
void arch_IdleEntry(void);
void arch_Reset(void);



#ifdef __cplusplus
}
#endif


#endif


