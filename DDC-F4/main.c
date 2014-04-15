/*---------------------------------------------------
	main.c (v1.00)
	
	DDC-F program, for electrocar.
----------------------------------------------------*/

#include "main.h"
#include "port.h"

#include "AD.h"
#include "Timer.h"
#include "voice.h"
#include "pwm.h"
#include "Delay.h"
#include "communication.h"
#include "Other.h"

/*------- Public variable declarations --------------------------*/
extern bit fell_alarm_flag;				//���ر�����־
extern bit raised_alarm_flag;				//̧�𱨾���־
extern bit stolen_alarm_flag;				//����������־
extern tWord stolen_alarm_count;     	//��������ѭ������
extern bit transmit_comm1_flag;			//ÿ��һ��ʱ�佫����1������main�����з������ݣ��Ͳ���Ӱ��timer0�ļ�ʱ�������������0.
extern bit battery_check;					//��1ʱ��ִ��һ�ε���ת����ִ����󣬽�����0
extern bit Host_battery_high_flag;		//��1ʱ��ִ��һ��������ʾ����ʾ�������

/*------- Private variable declarations --------------------------*/
tWord ADC_check_result = 0;		//��ΪAD���ֵ
bit battery_HV_flag = 1;			//��ص�λ�ı�ǣ�1��ʾ���ڵ�������ģ�0��ʾ��û����

void main(void)
	{
	noVoice();
	InitTimer(50, 100);
//	myPwm();			//�������

	Moto_EN = 1;		//��ʼ�����ر����
	transmit_wire = 1;
	Voice_EN = 0;		  	//����ʱ�������Źر�
	
	ADC_check_result = GetADCResult(6);	//�ϵ�ʱ����ص������һ��
	
	stolen_alarm_count = 0;			//�屨��������
	stolen_alarm_flag = 0;			//�屨����־

	Transmitter_EN = 1;		// turn off the transmitter
	Receiver_EN = 0;			// turn on the receiver
   Delay(3);
	battery_stolen_speech();
	while(1)
		{
		sEOS_Go_To_Sleep();
		}
	}

/*---------------------------------------------------
	end of file
----------------------------------------------------*/