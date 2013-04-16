#ifndef __ARCH_IT_H
#define __ARCH_IT_H


#ifdef __cplusplus
extern "C" {
#endif





/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define ARCH_EXTIRQ_QTY			1
#define ARCH_TIMER_QTY			1


/* Exported functions ------------------------------------------------------- */
int arch_ExtIrqRegister(uint_t nPort, uint_t nPin, uint_t nTriggerMode);
void arch_ExtIrqEnable(uint_t nPort, uint_t nPin, uint_t nMode);
void arch_ExtIrqDisable(uint_t nPort, uint_t nPin);


void WDT_IRQHandler(int vector);
void TIMER0_IRQHandler(int vector);
void TIMER1_IRQHandler(int vector);
void UART0_IRQHandler(int vector);
void UART1_IRQHandler(int vector);
void EINT0_IRQHandler(int vector);
void EINT1_IRQHandler(int vector);
void EINT2_IRQHandler(int vector);
void EINT3_IRQHandler(int vector);



#ifdef __cplusplus
}
#endif

#endif

