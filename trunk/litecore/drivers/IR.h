//-----------------------------------------------------------------------------
//
//                           		���⴦�����         
//                                  ͷ�ļ�(*.h)
//
//
// �ļ���    : ir_test.h
// ����      : nmy
// ��������  : 2011-10-9
//
// ARM�ں�   : ARMv7M Cortex-M3
// ʹ��оƬ  : LM3S5965
// ��������  : KEIL
//
// �汾��¼  : V1.00  ������һ��   2011-10-9 15:30
//
//-----------------------------------------------------------------------------

// ʹ������������Ҫ��ͷ�ļ�
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"   // ��ϵͳ�����й�
#include "driverlib/gpio.h"     // ��GPIO�����й�
#include "driverlib/timer.h"    // ��timer�����й�

#define ir_timer_base TIMER1_BASE
#define ir_pwm_timer_base TIMER2_BASE
//#define ir_timer_base TIMER1_BASE
//#define ir_pwm_timer_base TIMER2_BASE

#define ir_debug 0x00
#define ir_test 0x01
#define ir_receive_flag_reset 0x00
#define ir_receive_flag_fail 0xf0
#define ir_timer_on TimerEnable(TIMER1_BASE, TIMER_A) 
#define ir_timer_off TimerDisable(TIMER1_BASE, TIMER_A)
//#define ir_pwm_on TimerEnable(TIMER2_BASE, TIMER_A) 
//#define ir_pwm_off TimerDisable(TIMER2_BASE, TIMER_A)
#define ir_send_l TimerEnable(TIMER2_BASE, TIMER_A) 
#define ir_send_h TimerDisable(TIMER2_BASE, TIMER_A)


static unsigned char ir_receive_buf[25];    		//������ջ�����,����Э��������23���ֽ�����
static unsigned char ir_receive_flag=0x01;			//�������״̬��־λ,0��ʾ����ʧ��
static unsigned char ir_receive_counter=0x00;		//������ռ�����
static unsigned char ir_receive_length=0x0B;		//������ճ��ȿ�����
static unsigned char ir_temp_data[12];				//�������11λ����+��żУ�黺��λ
static unsigned char ir_temp_byte;					//����ʱ��λ�������ݻ���
static unsigned char ir_status=0x00;				//����״̬��־λ,0x00-0x0f��ʾ����״̬��0x10-0xf0��ʾ����״̬
static unsigned char ir_send_buf[25];			   	//���ⷢ�ͻ�����,����Э�������23���ֽ�����
static unsigned char ir_send_flag=0x00;				   	//���ⷢ��������־λ��0��ʾ������
static unsigned char ir_send_length=0x00;			   	//���ⷢ��֡���ȣ��ֽڣ�



//struct meter_para
//{
// unsigned char factory_No[6];			//���ı��
// unsigned char prg_password[6];		//���ı������
// unsigned char pulse_constant[3];	//�������峣��
// unsigned char auto_bill_day[2];  //�Զ�������
//};
//
//typedef struct
//{               
//	unsigned char Program_permit         :1 ;		//Ϊ1����ǰ����������״̬		          
//	unsigned char Battery                :1 ;   //���Ƿѹ��ǣ�1ΪǷѹ 
//	unsigned char Battery_flag_write     :1 ;   //�ϵ��жϵ�غϸ�д��һ�δ�EEPROM����һ
//	unsigned char Receive_finish         :1 ; 	//485��ɽ���1֡����
//	unsigned char receive_first_68H	     :1 ; 	//485���յ���1��0x68����һ
//	unsigned char I_receive_finish       :1 ;   //������ɽ���1֡����                 		
//	unsigned char I_receive_first_68H 	 :1 ;   //������յ���1��0x68����һ
//	unsigned char I_transmit_one_byte 	 :1 ;   //����1�ֽڷ�����ϱ�־
//	unsigned char Quarter_second				 :1 ;		//�ķ�֮һ����					
//	unsigned char Broadcast              :1 ;		//�㲥Уʱ��ǣ�Ϊ0���Թ㲥Уʱ��Ϊ1��ֹ	
//	unsigned char Zero_hour_energy       :1 ;	 	//����Ƿ���ת�����ݱ��
//	unsigned char hour_energy  			     :1 ;		//�����Ƿ���ת�����ݱ��
//	unsigned char Month_energy           :1 ;   //�������Ƿ���ת�����ݱ��
//	unsigned char Switch_order           :1 ;		//Ϊ1ʱ������բ����δ��Ӧ	
//	unsigned char Switch_on					     :1 ;		//Ϊ1�����բ����
//	unsigned char Switch_off             :1 ;		//Ϊ1������բ����
//	unsigned char Detect_time_enough     :1 ;   //Ϊ1�����Ѽ�⵽�㹻����ʱ���ж��Ƿ���բ
//	unsigned char Power_reverse					 :1 ;		//���ܷ�����
//	unsigned char Less_half_IB           :1 ;   //Ϊ1�������С��IB
//	unsigned char More_than_255          :1 ;   //�ۼƴ�������255ʱ��1;
//	unsigned char New_pulse              :1 ;   //Ϊ1�����������������Ҫ���������С
//} METER_FLAG;
//
//
//
//
////unsigned char ir_rdata[11];								//���ⷢ��1�ֽ�ʱ�跢�͵�11λ  
//struct meter_para   meter_para;					//������
//METER_FLAG   BitFlag;										//λ���
//









