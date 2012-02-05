#ifndef __SC16IS7X_H__
#define __SC16IS7X_H__


#ifdef __cplusplus
extern "C" {
#endif










//External Functions
sys_res sc16is7x_Init(void);
sys_res sc16is7x_Reset(void);
sys_res sc16is7x_UartConfig(uint_t nChl, t_uart_para *pPara);
sys_res sc16is7x_UartSend(uint_t nChl, void *pData, uint_t nLen);
void sc16is7x_ItHandler(void);
int sc16is7x_GpioRead(uint_t nPin);
void sc16is7x_GpioSet(uint_t nPin, uint_t nHL);


#ifdef __cplusplus
}
#endif

#endif

