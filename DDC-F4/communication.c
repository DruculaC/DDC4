/*---------------------------------------------------
	communication.c (v1.00)
	
	ͨ�ų���
---------------------------------------------------*/	

#include "main.h"
#include "port.h"

#include "communication.h"
#include "Delay.h"

tByte myTxRxData[7]={0x00,0x00,0x00,0x00,0x00,0x00,0x00};

/*--------------------------------------------------
	ComMode_1_Data()
	
	���ͱ���1�������������᷵��һ������1�źţ��������
	�յ��ˣ��򸽻�֪�������ڸ����ˡ�
---------------------------------------------------*/
void ComMode_1_Data()			
	{
	tByte i,n;
   Receiver_EN = 1;			// turn off the receiver
	Delay(2);

	Transmitter_EN = 0;		// turn on the transmitter

	myTxRxData[0]=CmdHead;
	myTxRxData[1]=MyAddress;
	myTxRxData[2]=ComMode_1;

	initsignal();

	for(i=0;i<3;i++)
		{
		for(n=0;n<8;n++)
			{
			if((myTxRxData[i]&0x80)==0x80)
				{
				transmit_wire=0;
				Delay3(120);	
				}
			else	
				{
				transmit_wire=0;
				Delay3(60);
				}
			transmit_wire=1;
			myTxRxData[i]<<=1;
			Delay3(50);
			}
		}
	Transmitter_EN = 1;		// turn off the transmitter
	Delay(2);

	Receiver_EN = 0;			// turn on the receiver

	}

/*----------------------------------------------------
	initsignal()
	
	��ʼ���źų��򣬽��ջ��ڽӽ����źŵ�ʱ����Ҫ��
	һ�δ�����ʹ���ջ��򿪣�����һ�γ����п����޷�ʶ��
	�����������൱�ڷϴ��룬ֻռ��ͨ��ʱ�䡣
----------------------------------------------------*/

void initsignal()				
	{
	tByte k,k1;
	tByte mystartbuffer=0xaa;
	for(k1=0;k1<3;k1++)
		{
		for(k=0;k<8;k++)
			{
			if((mystartbuffer&0x80)==0x80)
				{
				transmit_wire=0;
				Delay3(80);	
				}
			else
				{
				transmit_wire=0;
				Delay3(80);	
				}
			transmit_wire=1;
			mystartbuffer<<=1;
			Delay3(150);	
			}
		mystartbuffer=0xaa;
		Delay3(80);
		}
	transmit_wire=1;
	}

/*---------------------------------------------------
	end of file
----------------------------------------------------*/

