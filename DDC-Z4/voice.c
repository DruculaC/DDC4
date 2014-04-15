/*---------------------------------------------------
	voice.c (v1.00)
	
	include the voice function
---------------------------------------------------*/

#include "main.h"
#include "port.h"

#include "voice.h"
#include "Delay.h"
#include "communication.h"

/*------ private variable --------------------------*/
tByte key_rotate_on_speech_number = 1;
   
/*--------------------------------------------------
	SC_Speech()
	
	control the voice IC, need which seg, so send how many pulses.
---------------------------------------------------*/

void SC_Speech(tByte cnt)
	{
	tByte i;
	SC_RST = 0;
	delay_ms(100);
	SC_RST = 1;
	delay_ms(150);
	for(i=0; i < cnt; i++)
		{
		SC_DATA = 1; 
		delay_us(20);
		SC_DATA = 0; 
		delay_us(20);
		}
	}

/*--------------------------------------------------
	SC_Speech2()
	
	control the voice IC, need which seg, so send how many pulses.

void SC_Speech2(tByte cnt)
{
	tByte i;
	SC_RST=0;
	delay_ms(40);
	SC_RST=1;
	delay_ms(40);
	for(i=0;i < cnt;i++)
	{
		SC_DATA=1; // ���������
		delay_us(350);
		SC_DATA=0; // ���������
		delay_us(350);
	}
}
---------------------------------------------------*/

/*-------------------------------------------------
	noVoice()
	
	���ϵ��ʱ����ã�������������
--------------------------------------------------*/

void noVoice()
	{
	P14=0;
	SC_DATA = 0;
	SC_RST = 0;
	delay_us(350); // �ϵ縴λ��ֹ���ŷ���
	SC_RST = 1;
	delay_us(350);
	P14 = 1;
	}

/*----------------------------------------------------
	key_rotate_on_speech()
	speech for key rotated on
-----------------------------------------------------*/
void key_rotate_on_speech(void)
	{
	switch(key_rotate_on_speech_number)
		{
		case 1:
			{
			voice_EN = 1;
			SC_Speech(14);  
			Delay(30);
			SC_Speech(18);  
			Delay(30);
			SC_Speech(11);  
			Delay(50);
			voice_EN = 0;
         key_rotate_on_speech_number = 2;
			}
		break;
		
		case 2:
			{
			voice_EN = 1;
			SC_Speech(14);  
			Delay(25);
			SC_Speech(12);  
			Delay(50);
			voice_EN = 0;
         key_rotate_on_speech_number = 1;			
			}
		break;
		}
	}
	
/*-----------------------------------------------------
	key_rotate_off_speech()
	speech for key rotated off
void key_rotate_off_speech(void)
	{
	voice_EN = 1;
	SC_Speech(13);
	Delay(40);
	voice_EN = 0;
	}
------------------------------------------------------*/

/*----------------------------------------------------
	motorBAT_low_speech()
	��ƿ����������С�޶�ֵ��������ʾ��
-----------------------------------------------------*/
void motorBAT_low_speech(void)
	{
	voice_EN = 1;
	SC_Speech(6);
	Delay(60);
	voice_EN = 0;
	}

/*----------------------------------------------------
	host_touch_speech()
	��һ��������������һ����ʾ����
--------------------------------------------------------*/
void host_touch_speech(void)
	{
	voice_EN = 1;
	SC_Speech(2);  
	Delay(60);
	voice_EN = 0;
	}

/*-----------------------------------------------------
	host_2ndtouch_speech()
	�ڶ��δ����������ٱ�һ����ʾ����
-------------------------------------------------------*/
void host_2ndtouch_speech(void)
	{
	voice_EN = 1;
	SC_Speech(27);  
	Delay(80);
	voice_EN = 0;
	}
	
/*----------------------------------------------------
	stolen_alarm_speech()
	�ж�Ϊ������ʱ�򣬷�������������һ��
----------------------------------------------------*/
void stolen_alarm_speech1(void)
	{
	voice_EN = 1;
	SC_Speech(3); 
	Delay(160);
	voice_EN = 0; 
	}

/*----------------------------------------------------
	stolen_alarm_speech()
	�ж�Ϊ������ʱ�򣬷������������ڶ���
----------------------------------------------------*/
void stolen_alarm_speech2(void)
	{
	voice_EN = 1;
	SC_Speech(22); 
	Delay(160);
	voice_EN = 0;
	Delay(20);
	}

/*----------------------------------------------------
	slave_nearby_speech()
	�����ӵ������źź󣬱�������֪�������ڸ�������ʱ
	��һ��������
-----------------------------------------------------*/
void slave_nearby_speech(void)
	{
	voice_EN=1;
	SC_Speech(23); 
	Delay(50);
	voice_EN=0;
	}

/*----------------------------------------------------
	slave_away_speech()
	�����Ӳ��������źź󣬱���������Ϊ�����뿪�ˣ���ʱ
	��һ������
-----------------------------------------------------*/
void slave_away_speech(void)
	{
	voice_EN = 1;
	// ����������
	SC_Speech(24);  
	Delay(50);
	// ף��ƽ������
	SC_Speech(13);
	Delay(50);
	voice_EN = 0;
	}

/*----------------------------------------------------
	battery_stolen_speech()
	������⵽��ƽ���ڵ�ʱ�򣬲���������
-----------------------------------------------------*/
void battery_stolen_speech(void)
	{
	voice_EN = 1;
	SC_Speech(19);  
	Delay(80);
	voice_EN = 0;
	}

/*---------------------------------------------------
	end of file
----------------------------------------------------*/