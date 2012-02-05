#include "IR.h"
#include "temp.h"


void ir_send_byte(unsigned char byte)		  //����1�ֽ����ݴ���
{
	int i;
	TimerEnable(ir_timer_base, TIMER_A);
	ir_send_l;
	while (!(TimerIntStatus(ir_timer_base,0) & TIMER_TIMA_TIMEOUT));
	TimerIntClear(ir_timer_base,TIMER_TIMA_TIMEOUT);
	for(i=0;i<=7;i++)
	{
		if(byte&0x01==1)
		{
		ir_send_h;
		}
		else
		{
		ir_send_l;
		}
		byte>>=1;
	while (!(TimerIntStatus(ir_timer_base,0) & TIMER_TIMA_TIMEOUT));
	TimerIntClear(ir_timer_base,TIMER_TIMA_TIMEOUT);		
	}
	ir_send_h;
	while (!(TimerIntStatus(ir_timer_base,0) & TIMER_TIMA_TIMEOUT));
	TimerIntClear(ir_timer_base,TIMER_TIMA_TIMEOUT);
	TimerDisable(ir_timer_base, TIMER_A);

}

void ir_Send_data(void)	  //����֡���ݴ���
{
	unsigned char i;

	if (ir_debug)
	{
		ir_send_length=0x0E;
		ir_send_buf[0]=0x68;  								//��ʼλ
		ir_send_buf[1]=0x86;			//��ַ��
		ir_send_buf[2]=0x02;			//
		ir_send_buf[3]=0x16;			//
		ir_send_buf[4]=0x16;			//
		ir_send_buf[5]=0x68;			//
		ir_send_buf[6]=0x02;			//
		ir_send_buf[7]=0x85;								//��ʼλ
		ir_send_buf[8]=0x86;			//��ַ��
		ir_send_buf[9]=0x02;			//
		ir_send_buf[10]=0x16;			//
		ir_send_buf[11]=0x16;			//
		ir_send_buf[12]=0xbf;			//
		ir_send_buf[13]=0x16;			//
	}
	if(ir_send_length)
	{
		for(i=0;i<=(ir_send_length-1);i++ )
		{
			ir_status=0xe0;
			ir_send_byte(ir_send_buf[i]);
//			TimerDisable(ir_pwm_timer_base, TIMER_A);
//			SysCtlDelay(5 * (SysCtlClockGet()/3000));
		}
	}
//	ir_send_h;  //��Ϊ��ʱ��
//	TimerDisable(ir_timer_base, TIMER_A);
}

void ir_send(void)
{
	IntDisable(INT_GPIOE);
	SysCtlDelay(100 * (SysCtlClockGet()/3000));		//��ʱ�ȴ��������������״̬������ʱ���ͷ�ʽʱ�����ʱ��
	ir_Send_data();		//�������ͳ���
	ir_receive_counter=0x00;
	ir_send_flag=0x00;			//�رշ���������־λ
	ir_status=0x00;				//תΪ����״̬
//	SysCtlDelay(500 * (SysCtlClockGet()/3000));
	IntEnable(INT_GPIOE);
}

void ir_receive_byte(void)
{
	volatile long  rd_pin=0x00;
	unsigned char i,i2;	
  	TimerEnable(ir_timer_base, TIMER_A);			//������ʱ��
	for(i=0;i<=9;i++)
	{
		rd_pin = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_4) & GPIO_PIN_4;
		if(rd_pin == 0)
		{	 
			if( (i >= 1) && (i <= 8) )
			{
				ir_temp_byte >>=1;
				ir_temp_byte &= 0x7F;
			}
			ir_temp_data[i]=0x00;
		}
		else 
		{	 
			if( (i >= 1) && (i <= 8) )
			{
				ir_temp_byte >>=1;
				ir_temp_byte |= 0x80;
			}
			i2++;
			ir_temp_data[i]=0x01; 
		}
		while (!(TimerIntStatus(ir_timer_base,0) & TIMER_TIMA_TIMEOUT));
		TimerIntClear(ir_timer_base,TIMER_TIMA_TIMEOUT); 
	}
	TimerDisable(ir_timer_base, TIMER_A);			//������ʱ��
	i2%=2;
	i2=ir_temp_data[9];
}


void ir_receive(void)
{
	if(ir_receive_counter<ir_receive_length)
	{
		ir_receive_flag=0x01; 				//���ý��ձ�־λ
		ir_temp_byte=0x00;
		ir_receive_byte();
		ir_receive_buf[ir_receive_counter]=ir_temp_byte;
		ir_receive_counter++;
		ir_status=0x01;
	}
}


void ir_receive_int(void)		   //�����жϷ������
{
	long  rd_pin=0x00;
    unsigned long ul_int_staus=0x00;
    ul_int_staus = GPIOPinIntStatus(GPIO_PORTE_BASE, GPIO_PIN_4);	   	// ��ȡ�ж�Դ
	GPIOPinIntClear(GPIO_PORTE_BASE, ul_int_staus);	  					// ����ж�Դ
	SysCtlDelay(416 * (SysCtlClockGet()/3000000));	 					//�ж��Ƿ�Ϊ����
	rd_pin = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_4) & GPIO_PIN_4;	   	//��ȡ�������ŵ�λ
	if (rd_pin==0x00)
    {
		IntDisable(INT_GPIOE);
		ir_receive();
		IntEnable(INT_GPIOE);
	}
	
}

void ir_receive_deal(void)
{
		//֡���ȿ���
		if ((ir_receive_counter==0x07) && (ir_receive_flag != 0x00))
		{
			if(ir_receive_buf[ir_receive_counter]==0x11)
			{
				if(ir_receive_buf[ir_receive_counter-1]==0x01)
				{
					ir_receive_length=0x17;				//����У�����ʱ�Ľ��ճ���
				}
				else
				{
					ir_receive_length=0x0c;				//��У�����ʱ�Ľ��ճ���
				}
			}
			else if ((ir_receive_buf[ir_receive_counter]==0x05) && (ir_receive_buf[ir_receive_counter-1]==0x01))
			{
				ir_receive_length=0x0e;				   //���õ�ַʱ�Ľ��ճ���
			}
			else if ((ir_receive_buf[ir_receive_counter]==0x03)&& (ir_receive_buf[ir_receive_counter-1]==0x01))
			{
				ir_receive_length=0x10;				   //��������ʱ�Ľ��ճ���
			}
			else if ((ir_receive_buf[ir_receive_counter]==0x0a)&& (ir_receive_buf[ir_receive_counter-1]==0x01))
			{
				ir_receive_length=0x0c;				   //���ñ��ʱ�Ľ��ճ���
			}

		}
		ir_receive_flag=0x01;
		if ((ir_receive_counter>=ir_receive_length) || (ir_receive_flag==0x00))		//���մ�����߽������
		{
			IntDisable(INT_GPIOE);
			if(ir_receive_flag!=0x00)			//��������
			{
				ir_status=0x10;				//�÷��Ϳ��Ʊ�־λ
			}
			ir_receive_counter=0x00;		   	//��ս��ռ�����
			ir_receive_length=0x0b;		   	//�ָ�Ĭ�Ͻ���֡����
		}
}

void ir_start()
{

	if (ir_status != 0x00)
	{
		ir_receive_deal();
		if(ir_status >= 0x10)
		{
			ir_protocol();	//Э������ӳ���
			if(ir_status >= 0x10)
			{
				ir_send();
			}
		}
		ir_status=0x00;
	}

}


