/*---------------------------------------------------
	Timer.c (v1.00)
	
	Timer functions
---------------------------------------------------*/	

#include "main.h"
#include "port.h"

#include "Timer.h"
#include "communication.h"
#include "Other.h"
#include "voice.h"
#include "AD.h"
#include "delay.h"

// ------ Public variable declarations -----------------------------
bit fell_alarm_flag = 0;			//���ر�����־
tByte fell_alarm_count = 0;		//���ر���ѭ������

bit raised_alarm_flag = 0;		//̧�𱨾���־
tByte raised_alarm_count = 0;	//̧�𱨾�ѭ������

bit stolen_alarm_flag = 0;		//����������־
tWord stolen_alarm_count = 0;	//��������ѭ������

bit transmit_comm1_flag = 0;		//ÿ��һ��ʱ�佫����1������main�����з������ݣ��Ͳ���Ӱ��timer0�ļ�ʱ�������������0.
bit battery_check = 0;				//��1ʱ��ִ��һ�ε���ת����ִ����󣬽�����0
bit Host_battery_high_flag = 0;		//��1ʱ��ִ��һ��������ʾ����ʾ�������

extern tWord ADC_check_result;		//��ΪAD���ֵ
extern bit battery_HV_flag;			//��ص�λ�ı�ǣ�1��ʾ���ڵ�������ģ�0��ʾ��û����


// ------ Private variable definitions -----------------------------
tByte timer0_8H, timer0_8L, timer1_8H, timer1_8L;	//��ʱ��0��1�Ķ�ʱ����

tWord timer0_count = 0;			//��������timer0�Ĵ�����û���һ�ξͼ�1���൱�ڼ�ʱ
tByte leave_count = 0;				//ÿ��3s��1���������Ӧ����ȷ����������
tByte received_data_buffer[7] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00}; 	//�������ݻ���
bit receive_data_finished_flag = 0;		//������һ��������ɺ󣬴˱�־λ��1
tByte data_count = 0;				//�������ݻ����λ�����������ڼ�����������
tByte one_receive_byte = 0;		//�������ݵ�һ���ֽڣ���������丳ֵ��received_data_buffer���Ӧ���ֽ�
tByte one_receive_byte_count = 0;			//one_receive_byte��8λ���˼����������յ��ڼ�λ��ÿ�μ�����8��ʱ�����һ���ֽڽ�����ɡ�
bit receive_wire_flag = 1;		//����ͨ���ߵı�־λ��1�����ߵ�ƽ��0�����͵�ƽ��ÿ��timer1���ʱ���ж�P1.1һ�Ρ��Դ��������Ƿ�Ϊһ����ȷ���½���
tByte receive_HV_count = 0;		//��ʱ��T1��û���źŵ�����ʱ�򣬶Ըߵ�ƽ������һ������ĳ��ֵ����one_receive_byte_count��0
tByte receive_LV_count = 0;		//ÿ��timer1���ʱ�жϽ��������ΪLV���������1���Դ��������͵�ƽ��ʱ��
bit battery_stolen_EN = 0;       // ��Ϊ�����ӵ���ر����źź󣬱�����ʹ��
tByte battery_stolen_count = 0;

/*------------------------------------------------------------------
	timerT0()
	��ʱ��0ÿ�������ִ�еĲ���
--------------------------------------------------------------------*/

void timer0() interrupt interrupt_timer_0_overflow
	{
	// ��װ�ڶ�ʱ����ʱ������
	TH0 = timer0_8H;
	TL0 = timer0_8L;

	// ����һ��ÿ3s�Ĳ���
	if(++timer0_count >= 60)		
		{
      // ÿ��3s��һ�ε�����⣬��������صĵ�����ʾ
		ADC_check_result = GetADCResult(6);
		

		
		// �жϵ�ص�ѹ�����С��3.4V��ԭ�ȱ����ߵ�ѹʱ���򱨵������㡣�����ѹ����4.2V����ԭ�ȱ�����������ʱ������ʾ��ѹ����
		if((battery_HV_flag == 1)&&(ADC_check_result <= 0x333))
			{
			// ��λ��0����ʾ��������
			battery_HV_flag = 0;
			Battery_low_alarm_speech();		
			}
		else if((battery_HV_flag == 0)&&(ADC_check_result >= 0x35a))
			{
			// ��λ��1����ʾ��������
			battery_HV_flag = 1;
			Battery_high_alarm_speech();
			}	

		// ��������0
		timer0_count = 0;
		
		if((battery_stolen_EN == 1)&&(battery_stolen_count < 3))
			{
			battery_stolen_speech();
			Moto_Vibration();         
			if(++battery_stolen_count >= 3)
				{
				battery_stolen_EN = 0;
				battery_stolen_count = 0;
				}
			}
			
		// ̧���źű�����ÿ�α������0������ٴνӵ����������һ����˵������ÿ��̧��ֻ��4�顣
		if((raised_alarm_flag == 1)&&(raised_alarm_count < 3))	
			{
			Alarm_raised_speech();		
			Moto_Vibration();         
			if(++raised_alarm_count >= 3)
				{
				raised_alarm_flag = 0;
				raised_alarm_count = 0;
				}			
			}

		// �����źű�����ÿ�α������0������ٴνӵ����������һ����˵������ÿ�ε���ֻ��4�顣
		if((fell_alarm_flag == 1)&&(fell_alarm_count < 3))
			{
			Alarm_fell_speech();		  
			Moto_Vibration();         
			if(++fell_alarm_count >= 3)
				{
				fell_alarm_flag = 0;
				fell_alarm_count = 0;
				}
			}
		}

 	// ������������
	if(stolen_alarm_flag == 1)		
		{
		// ������������������ÿ��һ�μ�1���������2�Σ��������1����һ��ʱ��󣬽�flag��count��0.�Դ�����֤����ÿ�α������ڶ�ʱ����
		// �ٴ��յ������źţ��Ͳ��ᱨ���ˡ�
		if(++stolen_alarm_count < 3)
			{			
			// ������ʾ�������
			Alarm_stolen_speech();		
			Moto_Vibration();          
			}
		else
			{
			if(stolen_alarm_count >= 1200)
				{
				stolen_alarm_count = 0;
				stolen_alarm_flag = 0;
				}
			}
		}	


	// ��������������ʾ
	if(Host_battery_high_flag == 1)
		{
		Host_battery_high_alarm_speech();		
		Host_battery_high_flag = 0;
		}		
	}

/*------------------------------------------------------------------
	timerT1()
	��ʱ��1ÿ�������ִ�еĲ���
--------------------------------------------------------------------*/
	
void timerT1() interrupt interrupt_timer_1_overflow 			
	{
	// ��װ�ڶ�ʱ��1������
	TH1 = timer1_8H;				
	TL1 = timer1_8L;

	// �жϽ������Ƿ�Ϊ0�������0�����ʾ�������źŹ�����
	if(receive_wire == 0)					
		{
		// �����������0���򽫴˱�־λ��0����ʾ����Ϊ0����
		receive_wire_flag=0;
		
		// ���յ��͵�ƽ��ʱ�����������8ms�����¼���
		if(++receive_LV_count==120)		
			{
			receive_LV_count=0;
			}
		}
	else
	{
		if(receive_wire_flag==0)//˵����һ���͵�ƽ
		{
			receive_wire_flag=1;
//			one_receive_byte<<=1;

			if((receive_LV_count>35)&&(receive_LV_count<=80))//�͵�ƽ������ʱ��С��3ms����Ϊ0
			{
				one_receive_byte<<=1;
				one_receive_byte &= 0xfe;
				one_receive_byte_count++;
				receive_HV_count=0;
			}
			if(receive_LV_count>80)//�͵�ƽ������ʱ�����4.5ms����Ϊ1
			{
				one_receive_byte<<=1;
				one_receive_byte |= 0x01;
				one_receive_byte_count++;
				receive_HV_count=0;
			}
			else
			{
				receive_HV_count++;	
			}

			receive_LV_count=0;
		}
		else
		{
			receive_HV_count++;
			if(receive_HV_count>=60)
			{
				one_receive_byte_count=0;
				receive_wire_flag=1;
				data_count=0;
			}		
		}
	}

	if(one_receive_byte_count==8)//˵��һ���ֽڵ������Ѿ�������ȫ
	{
		one_receive_byte_count=0;
		received_data_buffer[data_count]=one_receive_byte;
		if(data_count==0&&received_data_buffer[0]==CmdHead)
		{
			data_count=1;
		}
		else if(data_count==1&&received_data_buffer[1]==MyAddress)
		{
			data_count=2;
		}
		else if(data_count==2)
		{
			receive_data_finished_flag=1;
			data_count=0;
		}
		else 
		{
			data_count=0;
		}
	}

	if(receive_data_finished_flag==1)	//˵�����յ������ݣ���ʼ����
	{
		receive_data_finished_flag=0;	//����ձ�־
		switch(received_data_buffer[2])//����ָ��
		{
			case ComMode_1://���յ������������͹����ı���1���źţ�˵��������3M�ڣ���������
				{	
				ComMode_1_Data();
				}
			break;
			
			case ComMode_2:
				{
				battery_stolen_EN = 1;
				}
		   break;
			
			case ComMode_3:
				{
				stolen_alarm_flag = 1;

				raised_alarm_count=0;//�屨��������
				raised_alarm_flag=0;//�屨����־
				fell_alarm_count=0;//�屨��������
				fell_alarm_flag=0;//�屨����־
				}
			break;
		
			case ComMode_4://����̧���ź�ʹ��
			{
				raised_alarm_flag=1;//̧�𱨾�

				stolen_alarm_count=0;//�屨��������
				stolen_alarm_flag=0;//�屨����־
				fell_alarm_count=0;//�屨��������
				fell_alarm_flag=0;//�屨����־
			}
			break;

			case ComMode_5://���������ź�ʹ��
			{
				fell_alarm_flag=1;	//���ر���

				stolen_alarm_count=0;//�屨��������
				stolen_alarm_flag=0;//�屨����־
				raised_alarm_count=0;//�屨��������
				raised_alarm_flag=0;//�屨����־
			}
			break;

			case ComMode_6:
				{
				Host_battery_high_flag = 1;
				}
			break;
		}
	}
}

/*--------------------------------------------------
	InitTimer()
	
	��ʼ����ʱ��T0��T1
---------------------------------------------------*/

void InitTimer(const tByte Tick_ms_T0, Tick_us_T1)
	{
	tLong Inc_T0, Inc_T1;
	tWord timer0_16, timer1_16;
	
	//����Timer0�ļĴ���ֵ
	Inc_T0 = (tLong)Tick_ms_T0 * (OSC_FREQ/1000) / (tLong)OSC_PER_INST;
	timer0_16 = (tWord) (65536UL - Inc_T0);
	timer0_8H = (tByte) (timer0_16 / 256);
	timer0_8L = (tByte) (timer0_16 % 256);
	
	//����Timer1�ļĴ���ֵ
	Inc_T1 = (tLong)Tick_us_T1 * (OSC_FREQ/1000000) / (tLong)OSC_PER_INST;
	timer1_16 = (tWord) (65536UL - Inc_T1);
	timer1_8H = (tByte) (timer1_16 / 256);
	timer1_8L = (tByte) (timer1_16 % 256);
	
	TMOD = 0x11;
	TH0 = timer0_8H;
	TL0 = timer0_8L;
	TH1 = timer1_8H;
	TL1 = timer1_8L;

	ET0 = 1;
	TR0 = 1;
	ET1 = 1;
	TR1 = 1;
	PT1 = 1;			
	EA = 1;
	}

/*---------------------------------------------------------------------
	sEos_Go_To_Sleep()
	��MCU��������״̬
----------------------------------------------------------------------*/
void sEOS_Go_To_Sleep(void)
   {
   PCON |= 0x01;    // Enter idle mode (generic 8051 version)
   }

/*---------------------------------------------------
	end of file
----------------------------------------------------*/