#ifndef __HT1621_H__
#define __HT1621_H__


#ifdef __cplusplus
extern "C" {
#endif



// LCD���ڶ˿ڼ�����
#define GPIO_LCD_SYSCTL_PERIPH  SYSCTL_PERIPH_GPIOE

#define GPIO_LCD_PORT_BASE      GPIO_PORTE_BASE

#define HT1621_CS			GPIO_PIN_0

#define HT1621_RD			GPIO_PIN_1

#define HT1621_WR			GPIO_PIN_2

#define HT1621_DATA			GPIO_PIN_3

#define high_val			0xFF

//CS����1
#define HT1621_CS_H GPIOPinWrite(GPIO_LCD_PORT_BASE,HT1621_CS,high_val)

//CS������
#define HT1621_CS_L GPIOPinWrite(GPIO_LCD_PORT_BASE,HT1621_CS,~high_val)

//RD����1
#define HT1621_RD_H GPIOPinWrite(GPIO_LCD_PORT_BASE,HT1621_RD,high_val)

//RD������
#define HT1621_RD_L GPIOPinWrite(GPIO_LCD_PORT_BASE,HT1621_RD,~high_val)
 
//WR����1
#define HT1621_WR_H GPIOPinWrite(GPIO_LCD_PORT_BASE,HT1621_WR,high_val)

//WR����1
#define HT1621_WR_L GPIOPinWrite(GPIO_LCD_PORT_BASE,HT1621_WR,~high_val)

//DATA������
#define HT1621_DATA_H GPIOPinWrite(GPIO_LCD_PORT_BASE,HT1621_DATA,high_val)

//DATA������
#define HT1621_DATA_L GPIOPinWrite(GPIO_LCD_PORT_BASE,HT1621_DATA,~high_val)

//��DATA�˵�����״̬
#define HT1621_DATA_READ GPIOPinRead(GPIO_LCD_PORT_BASE, HT1621_DATA)

//����DATA�˵�Ϊ���
#define HT1621_DATA_OUTPUT GPIOPinTypeGPIOOutput(GPIO_LCD_PORT_BASE,HT1621_DATA )

//����DATA�˵�Ϊ����
#define HT1621_DATA_INPUT 	GPIOPinTypeGPIOInput(GPIO_LCD_PORT_BASE,HT1621_DATA);

//����DATA�˵�������
#define DATA_INPUT_CONFIG 	GPIOPadConfigSet(GPIO_LCD_PORT_BASE, HT1621_DATA, GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);;

//�������ŵ�Ϊ���
#define HT1621_OUTPUT(x) 	GPIOPinTypeGPIOOutput(GPIO_LCD_PORT_BASE,x )

//������ʱ
#define Ctl_Delay(x) 	SysCtlDelay(x * (SysCtlClockGet()/16000000))





//External Functions
void ht1621_Init(void);
void ht1621_Write(uint_t nIcon, uint_t nMask);

#ifdef __cplusplus
}
#endif

#endif

