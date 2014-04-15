/*---------------------------------------------------
	communication.c (v1.00)
	
	ͨ�ų���
---------------------------------------------------*/	

#include "main.h"
#include "port.h"

#include "communication.h"
#include "Delay.h"

/*-------------------------------------------------*/
tByte myTxRxData[7]={0x00,0x00,0x00,0x00,0x00,0x00,0x00};

extern tByte receive_LV_count;		//ÿ��timer1���ʱ�жϽ��������ΪLV���������1���Դ��������͵�ƽ��ʱ��
extern bit receive_wire_flag;		//����ͨ���ߵı�־λ��1�����ߵ�ƽ��0�����͵�ƽ��ÿ��timer1���ʱ���ж�P1.1һ�Ρ��Դ��������Ƿ�Ϊһ����ȷ���½���
extern tByte one_receive_byte;		//�������ݵ�һ���ֽڣ���������丳ֵ��received_data_buffer���Ӧ���ֽ�
extern tByte one_receive_byte_count;			//one_receive_byte��8λ���˼����������յ��ڼ�λ��ÿ�μ�����8��ʱ�����һ���ֽڽ�����ɡ�
extern tByte receive_HV_count;		//��ʱ��T1��û���źŵ�����ʱ�򣬶Ըߵ�ƽ������һ������ĳ��ֵ����one_receive_byte_count��0
extern tByte data_count;				//�������ݻ����λ�����������ڼ�����������
extern tByte received_data_buffer[7];		//�������ݻ���
extern bit receive_data_finished_flag;		//������һ��������ɺ󣬴˱�־λ��1

/*----------------------------------------------------
	initsignal()
	
	��ʼ���źų��򣬽��ջ��ڽӽ����źŵ�ʱ����Ҫ��
	һ�δ�����ʹ���ջ��򿪣�����һ�γ����п����޷�ʶ��
	�����������൱�ڷϴ��룬ֻռ��ͨ��ʱ�䡣
----------------------------------------------------*/

void initsignal()
{
	unsigned char k,k1;
	unsigned char mystartbuffer=0xaa;
	for(k1=0;k1<3;k1++)
	{
		for(k=0;k<8;k++)
		{
			if((mystartbuffer&0x80)==0x80)//Ϊ1
			{
				P10=0;
				Delay3(80);//��ʱ4.5ms���ϣ����ڶ�ʱ��ռ�����⣬ֻ����������ʱ��ʵ��
			}
			else//Ϊ0�����
			{
				P10=0;
				Delay3(80);//��ʱ2ms�����ڶ�ʱ��ռ�����⣬ֻ����������ʱ��ʵ��
			}
			P10=1;//��̬Ϊ�ߵ�ƽ
			mystartbuffer<<=1;
			Delay3(150);//��ʱҪ����2ms
		}
		mystartbuffer=0xaa;
		Delay3(80);
	}
	P10=1;
}

/*----------------------------------------------------
	initsignal2()
	
	��ʼ���źų��򣬽��ջ��ڽӽ����źŵ�ʱ����Ҫ��
	һ�δ�����ʹ���ջ��򿪣�����һ�γ����п����޷�ʶ��
	�����������൱�ڷϴ��룬ֻռ��ͨ��ʱ�䡣
----------------------------------------------------*/

void initsignal2()
{
	unsigned char k,k1;
	unsigned char mystartbuffer=0xaa;
	for(k1=0;k1<3;k1++)
	{
		for(k=0;k<8;k++)
		{
			if((mystartbuffer&0x80)==0x80)//Ϊ1
			{
				P10=0;
				Delay3(80);//��ʱ4.5ms���ϣ����ڶ�ʱ��ռ�����⣬ֻ����������ʱ��ʵ��
			}
			else//Ϊ0�����
			{
				P10=0;
				Delay3(80);//��ʱ2ms�����ڶ�ʱ��ռ�����⣬ֻ����������ʱ��ʵ��
			}
			P10=1;//��̬Ϊ�ߵ�ƽ
			mystartbuffer<<=1;
			Delay3(150);//��ʱҪ����2ms
		}
		mystartbuffer=0xaa;
		Delay3(80);
	}
	P10=1;
}

/*----------------------------------------------------
	initsignal3()
	
	��ʼ���źų��򣬽��ջ��ڽӽ����źŵ�ʱ����Ҫ��
	һ�δ�����ʹ���ջ��򿪣�����һ�γ����п����޷�ʶ��
	�����������൱�ڷϴ��룬ֻռ��ͨ��ʱ�䡣
----------------------------------------------------*/

void initsignal3()
{
	unsigned char k,k1;
	unsigned char mystartbuffer=0xaa;
	for(k1=0;k1<3;k1++)
	{
		for(k=0;k<8;k++)
		{
			if((mystartbuffer&0x80)==0x80)//Ϊ1
			{
				P10=0;
				Delay3(80);	//��ʱ4.5ms���ϣ����ڶ�ʱ��ռ�����⣬ֻ����������ʱ��ʵ��
			}
			else
			{
				P10=0;
				Delay3(80);	//��ʱ2ms�����ڶ�ʱ��ռ�����⣬ֻ����������ʱ��ʵ��
			}
			P10=1;
			mystartbuffer<<=1;
			Delay3(150);//��ʱҪ����2ms
		}
		mystartbuffer=0xaa;
		Delay3(80);
	}
	P10=1;
}

/*------------------------------------------------------------------------
	

/*--------------------------------------------------
	ComMode_1_Data()
	
	�������յ�����1�źź󣬻ᷴ��һ������1�źŸ�����
	�Ա�ʾ�����ڸ���������
---------------------------------------------------*/

void ComMode_1_Data()	
{
	unsigned char i,n;
	receiver_EN = 1;
	transmiter_power = 1;				
	transmiter_EN = 0;
	myTxRxData[0]=CmdHead;
	myTxRxData[1]=MyAddress;
	myTxRxData[2]=ComMode_1;
/*	myTxRxData[3]=0x00;
	myTxRxData[4]=0x00;
	myTxRxData[5]=0x00;
	myTxRxData[6]=0x00;
*/
	initsignal2();

	for(i=0;i<3;i++)
	{
		for(n=0;n<8;n++)
		{
			if((myTxRxData[i]&0x80)==0x80)//Ϊ1
			{
				P10=0;
				Delay4(120);//��ʱ4.5ms���ϣ����ڶ�ʱ��ռ�����⣬ֻ����������ʱ��ʵ��
			}
			else//Ϊ0�����
			{
				P10=0;
				Delay4(80);//��ʱ2ms�����ڶ�ʱ��ռ�����⣬ֻ����������ʱ��ʵ��
			}
			P10=1;//��̬Ϊ�ߵ�ƽ
			myTxRxData[i]<<=1;
			Delay4(50);//��ʱҪ����2ms
		}
	}
	transmiter_EN = 1;
	receiver_EN = 0;
	transmiter_power = 1;				
}

/*--------------------------------------------------
	ComMode_2_Data()
	when battery is stolen, transmit this code to slave. 
---------------------------------------------------*/

void ComMode_2_Data()	
{
	unsigned char i,n;
	receiver_EN = 1;
	transmiter_power = 0;				
	transmiter_EN = 0;
	myTxRxData[0]=CmdHead;
	myTxRxData[1]=MyAddress;
	myTxRxData[2]=ComMode_2;
	initsignal2();

	for(i=0;i<3;i++)
	{
		for(n=0;n<8;n++)
		{
			if((myTxRxData[i]&0x80)==0x80)//Ϊ1
			{
				P10=0;
				Delay4(120);//��ʱ4.5ms���ϣ����ڶ�ʱ��ռ�����⣬ֻ����������ʱ��ʵ��
			}
			else//Ϊ0�����
			{
				P10=0;
				Delay4(80);//��ʱ2ms�����ڶ�ʱ��ռ�����⣬ֻ����������ʱ��ʵ��
			}
			P10=1;//��̬Ϊ�ߵ�ƽ
			myTxRxData[i]<<=1;
			Delay4(50);//��ʱҪ����2ms
		}
	}
	transmiter_EN = 1;
	transmiter_power = 1;				
	receiver_EN = 0;
}

/*----------------------------------------------------------
	ComMode_3_Data()
	
	���������źţ��ø���������ʾ���綯��������
----------------------------------------------------------*/

void ComMode_3_Data()
{
//	unsigned int j;
	unsigned char i,n;
	receiver_EN = 1;
	transmiter_power = 0;
	transmiter_EN = 0;
	myTxRxData[0]=CmdHead;
	myTxRxData[1]=MyAddress;
	myTxRxData[2]=ComMode_3;
/*	myTxRxData[3]=0x00;
	myTxRxData[4]=0x00;
	myTxRxData[5]=0x00;
	myTxRxData[6]=0x00;
*/
	initsignal();

	for(i=0;i<3;i++)
	{
		for(n=0;n<8;n++)
		{
			if((myTxRxData[i]&0x80)==0x80)//Ϊ1
			{
				P10=0;
				Delay4(120);//��ʱ4.5ms���ϣ����ڶ�ʱ��ռ�����⣬ֻ����������ʱ��ʵ��
			}
			else//Ϊ0�����
			{
				P10=0;
				Delay4(80);//��ʱ2ms�����ڶ�ʱ��ռ�����⣬ֻ����������ʱ��ʵ��
			}
			P10=1;//��̬Ϊ�ߵ�ƽ
			myTxRxData[i]<<=1;
			Delay4(50);//��ʱҪ����2ms
		}
	}
	transmiter_EN = 1;
	transmiter_power = 1;
	receiver_EN = 0;
}

/*--------------------------------------------------------
	ComMode_4_Data()
	
	���źŸ�����������������ʾ���綯����̧��
---------------------------------------------------------*/

void ComMode_4_Data()//����̧�����
{
	unsigned char i,n;
	receiver_EN = 1;
	transmiter_power = 0;
	transmiter_EN = 0;
	myTxRxData[0]=CmdHead;
	myTxRxData[1]=MyAddress;
	myTxRxData[2]=ComMode_4;
/*	myTxRxData[3]=0x00;
	myTxRxData[4]=0x00;
	myTxRxData[5]=0x00;
	myTxRxData[6]=0x00;
*/
	initsignal3();

	for(i=0;i<3;i++)
	{
		for(n=0;n<8;n++)
		{
			if((myTxRxData[i]&0x80)==0x80)//Ϊ1
			{
				P10=0;
				Delay4(120);//��ʱ4.5ms���ϣ����ڶ�ʱ��ռ�����⣬ֻ����������ʱ��ʵ��
			}
			else//Ϊ0�����
			{
				P10=0;
				Delay4(80);//��ʱ2ms�����ڶ�ʱ��ռ�����⣬ֻ����������ʱ��ʵ��
			}
			P10=1;//��̬Ϊ�ߵ�ƽ
			myTxRxData[i]<<=1;
			Delay4(50);//��ʱҪ����2ms
		}
	}
	transmiter_EN = 1;
	transmiter_power = 1;
	receiver_EN = 0;
}

/*-----------------------------------------------------------
	ComMode_5_Data()
	
	���źŸ�����������������ʾ���綯�������ˡ�
------------------------------------------------------------*/

void ComMode_5_Data()//���͵��ر���
{
	unsigned char i,n;
	receiver_EN = 1;
	transmiter_power = 0;//�л�Ϊ300M����
	transmiter_EN = 0;      //�����߷����
	myTxRxData[0]=CmdHead;
	myTxRxData[1]=MyAddress;
	myTxRxData[2]=ComMode_5;
/*	myTxRxData[3]=0x00;
	myTxRxData[4]=0x00;
	myTxRxData[5]=0x00;
	myTxRxData[6]=0x00;
*/
	initsignal3();

	for(i=0;i<3;i++)
	{
		for(n=0;n<8;n++)
		{
			if((myTxRxData[i]&0x80)==0x80)//Ϊ1
			{
				P10=0;
				Delay4(120);//��ʱ4.5ms���ϣ����ڶ�ʱ��ռ�����⣬ֻ����������ʱ��ʵ��
			}
			else//Ϊ0�����
			{
				P10=0;
				Delay4(80);//��ʱ2ms�����ڶ�ʱ��ռ�����⣬ֻ����������ʱ��ʵ��
			}
			P10=1;//��̬Ϊ�ߵ�ƽ
			myTxRxData[i]<<=1;
			Delay4(50);//��ʱҪ����2ms
		}
	}
	transmiter_EN = 1;
	transmiter_power = 1;
	receiver_EN = 0;
}

/*-----------------------------------------------------------------------------
	receive_byte()
	receive a byte program
-----------------------------------------------------------------------------*/
void receive_byte(void)
	{
	// P11 constantly HV, if detected a LV, judge it.
	if(P11 == 0)
		{
		// count the LV time, if more than 12ms, reset it.
		if(++receive_LV_count == 120)
			{
			receive_LV_count = 0;
			}
		receive_wire_flag = 0;
		}
	// if P11 return to HV
	else
		{
		// and already have LV before, so we think it maybe a bit signal
		if(receive_wire_flag == 0)
			{
			// set the flag, to judge a bit only one time
			receive_wire_flag = 1;

			// judge the LV time, if 3.5ms < time < 8ms, we think it is a "0". time<3.5ms means a noise.
			if((receive_LV_count > 35)&&(receive_LV_count <= 80))	
				{
				// save "0" to one byte
				one_receive_byte <<= 1;
				one_receive_byte &= 0xfe;
				one_receive_byte_count++;
				receive_HV_count = 0;
				}
			// time > 8ms, means a "1"
			else if((receive_LV_count > 80))
				{
				// save "1" to one byte
				one_receive_byte <<= 1;
				one_receive_byte |= 0x01;
				one_receive_byte_count++;
				receive_HV_count = 0;
				}
			
			else
				{
				// increase the count for HV
				receive_HV_count++;	
				}
         // reset LV count
			receive_LV_count = 0;
			}
		else
			{
			// judge whether HV count > 6ms, if yes, means a restart
			if(++receive_HV_count >= 60)
				{
				one_receive_byte_count = 0;
				receive_wire_flag = 1;
				data_count = 0;
				}		
			}
		}
	}

/*-----------------------------------------------------------------------------
	receive_word()
	receive a word program
-----------------------------------------------------------------------------*/
void receive_word(void)
	{
	// one byte have 8 bit, once receive a bit, the count increase, once it is 8, it means a byte received.
	if(one_receive_byte_count == 8)
		{
		one_receive_byte_count = 0;
		// assign one byte to buffer[i] 
		received_data_buffer[data_count] = one_receive_byte;
		
		// judge whether buffer[0] is CmdHead
		if((data_count == 0) && (received_data_buffer[0] == CmdHead))
			{
			data_count = 1;
			}
		// judge whether buffer[1] is MyAddress
		else if((data_count == 1) && (received_data_buffer[1] == MyAddress))
			{
			data_count = 2;
			}
		else if(data_count == 2)
			{
			receive_data_finished_flag = 1;
			data_count = 0;
			}
		else 
			{
			data_count = 0;
			}
		}
	}
	
/*---------------------------------------------------
	end of file
----------------------------------------------------*/