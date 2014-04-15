//�綯��
#include"N79E81x.h"
#include<intrins.h>
#include"AD.h"
#include"UART.h"
#include"T0.h"
#include"voice.h"
#include"pwm.h"
#include"T1.h"

//����ͨ������
#define CmdStart 0x00 //��������
#define CmdStop 0x01  //�ػ�����
#define ComMode_1 0xc1 //ͨ��ģʽ1 
#define ComMode_2 0xc2 //ͨ��ģʽ2
#define ComMode_3 0xc3 //ͨ��ģʽ3
#define ComMode_4 0xc4 //̧��ָ��
#define ComMode_5 0xc5//����ָ��
#define ComMode_6 0xc6	//�������ָ��
#define Succeed 0xce  //ͨ�ųɹ�
#define Wrong 0xff    //ͨ��ʧ��
#define CmdHead 0xc8
#define CmdHead1 0x33 //����֡���ײ�1, 00110011,11
#define CmdHead2 0xcc //����֡���ײ�2,11001100,00
#define CmdHead3 0x3c //����֡���ײ�3,11000011,01
#define CmdHead4 0xcc //����֡���ײ�4,11001100,00
#define MyAddress 0xe0
#define MyAddress1 0x33 //������ַ1, 00110011,11
#define MyAddress2 0x3c //������ַ2, 11000011,01
#define MyAddress3 0xcc //������ַ3,11001100,00
#define MyAddress4 0xcc //������ַ4,11001100,00

//���ñ������İ�ť��1��ʱ��һ�µ�����0��ʱ��Ϊ��̬
sbit det_battery=P2^4;			

//���ó��ָʾ
//sbit det_charge=P0^2;
//�����ķ��䲿�ֵĿ��ƶ˿�
//sbit PWMout=P0^1;//������ķ�������ڣ�ʹ������PWM
sbit ModeControl_1=P2^6;//�����ģʽ����,0��Ϊ30Mģʽ��1��Ϊ300Mģʽ
sbit tran_en=P2^7;//��������أ�1��Ϊ���ˣ�0��Ϊ����

//���ᴫ����
sbit ReceWave=P0^7;//���ᴫ���������
sbit SensorControl=P2^5;//���ᴫ�������ƶ�

//�����,  ƽʱֵΪ00��������10��������01��Ȼ�󶼻ָ���00
sbit MagentControl_1=P2^2;
sbit MagentControl_2=P2^3;

//ʰ�������� AD��6��ͨ��Ϊʰ��������������
//sbit VoiceControl=P2^4;//ʰ�������ƶ�

sbit upSignal=P0^4;//̧���ź�
sbit downSignal=P0^3;//�����ź�

//��ؿ��� 	AD��1��ͨ��Ϊ��صĵ�������
sbit BatteryControl=P1^2;

unsigned char count=0;//���ݽ��ղ��ֵļ�����

unsigned int lastAddr=0;//��һ�ν��յ��ı���ĵ�ַ

unsigned char time0Count_1=0;//��Ϊ���ᴫ������������֮���ʱ������ʱ
unsigned char time0Count_2=0;//��Ϊ���ᴫ�����ļ�ʱ
unsigned char time0Count_6=0;
unsigned int time0Count_3=0;//��Ϊ����ÿ������������Ϣ����ʱ��
unsigned char time0Count_4=0;//��Ϊ̧�������ʱ������ʱ
unsigned char time0Count_5=0;//��Ϊ���������ʱ������ʱ

bit SensorFlag=0; //���ᴫ�����ĵ͵�ƽ��־λ
unsigned char  SensorCount=0; //��Ϊ���ᴫ��������ļ���

unsigned char TestFlag=0;//1��2��3�ֱ�Ϊÿ�ν��յ��������������ݺ�ļ������ڴ��ڵĳɹ�ָ�����ִ�н�ȥ����Ĳ���
                //�������3�ζ�û�й��㣬��˵�����ڳ���
unsigned char ModeFlag=1;//ģʽѡ��λ��1����ģʽ1,2����ģʽ2,3��Ϊģʽ3

bit alarmFlag=0;//���������Ŀ�����־
bit alarmFlag2=0;//����������־2
//unsigned int alarmCount=0;//���������Ĵ���

bit threeFlag=0;//��·ѭ�����ر�־

bit power1Flag=0;
bit power2Flag=0;
bit power3Flag=0;
bit power4Flag=0;

bit voiceFlag=0;
bit downUpFlag=0;  //���غ�̧�����־

bit downFlag=0;//���صı�־
unsigned char downcount=0;
bit upFlag=0;//̧��ı�־
unsigned char upcount=0;
bit downFlagSend=0;//���ط��͵ı�־
bit upFlagSend=0;//̧���͵ı�־

//һ��ͷ�ֽڣ�һ����ַ�ֽڣ�һ�������ֽڣ����������ַ�ֽڣ���������
unsigned char myTxRxData[7]={0x00,0x00,0x00,0x00,0x00,0x00,0x00};//��������ͨ�����ݵĻ�����
unsigned char myTxRxData2[7]={0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned int Check=0;//�������
//unsigned char Check1=0;//��ΪAD���ֵ

bit receiveFlag=0;//���յ����ݱ�־
bit commuFlag=0;//����ͨ�ű�־

unsigned char DataBetween=0;//��Ϊ�������ݵ��м����
unsigned char RecData=0;//���յ�������
unsigned char DataTime=0;//��Ϊ���յ����ݵ���λ��������
bit ComFlag=1;//�������ص�һ����־
//unsigned int newAddr=0;
unsigned char T1highcount=0;	   //��ʱ��T1��û���źŵ�����ʱ�򣬶Ըߵ�ƽ������һ������ĳ��ֵ����Datatime��0

//���ſ��ؿ��ƣ�1Ϊ�򿪹��ţ�0Ϊ�رչ���
sbit PAshutdown=P1^4;

//��������ת�����
unsigned char magnetflag=0;

//����һ������������ʾ�źŽ��պ󣬶೤ʱ��ʹ���ջ��򿪣�������SwitchControl�ĸߵ�ƽʱ�䡣
//unsigned int SwitchControlcount=0;

//���ͱ����źű�־λ
//unsigned char commode2_flag=0; //���ͱ���2�ı�־λ
//unsigned char commode3_flag=0; //���ͱ���3�ı�־λ

bit ADCcheck=0;			//��1ʱ��ִ��һ�ε���ת����ִ����󣬽�����0
bit sendcomm1=0;		//��1ʱ��ִ��һ�η��ͱ���1��ִ����󣬽�����0
bit sendcomm3=0;		//��1ʱ��ִ��һ�η��ͱ���3��ִ����󣬽�����0
bit sendcomm4=0;		//��1ʱ��ִ��һ�η��ͱ���4��ִ����󣬽�����0
bit sendcomm5=0;		//��1ʱ��ִ��һ�η��ͱ���5��ִ����󣬽�����0
bit magcon=0;			//��1ʱ��ִ��һ�ε�������ϲ�����ִ����󣬽�����0
bit magcon2=0;			//��1ʱ��ִ��һ�ε�����򿪲�����ִ����󣬽�����0
bit sendspeech1=0;		//��1ʱ��ִ��һ�α��������������棩��ִ����󣬽�����0
unsigned char speech1_count=0;
bit sendspeech2=0;		//��1ʱ��ִ��һ�α�������������+��������ִ����󣬽�����0
bit sendspeech3=0;		//��1ʱ��ִ��һ�α��������������棩��ִ����󣬽�����0
unsigned int speech3_count=0;
bit sendspeech4=0;		//��1ʱ��ִ��һ�α�������������+��������ִ����󣬽�����0
bit sendspeech5=0;
bit sendspeech6=0;
bit sendspeech7=0;
unsigned char speech7_count=0;
bit sendspeech8=0;
unsigned char speech8_count=0;

bit stolenflag=0;		//������־λ
unsigned int stolen_count=0;	//����������ʱ��
unsigned char stolen_flag=0;	//��⴫������ʼ��־

unsigned char turnonflag=0;		//�綯�������رձ�־λ��1��ʾ�綯�������ˣ�0��ʾ�綯���ر���
unsigned char turnon_speech_flag=0;		//����������־λ�������任������

//��������
//void ComMode_1_Data(unsigned int sendAddr);	//����ģʽ1����
void ComMode_1_Data(void);	//����ģʽ1����
void ComMode_2_Data(void);//����ģʽ2����
void ComMode_3_Data(void);//����ģʽ3����
void ComMode_4_Data(void);//����̧�����
void ComMode_5_Data(void);//���͵��ر���
void ComMode_6_Data(void);//���ͳ����������
void verifybattery(void);

//void codeData(unsigned char *doData,unsigned char len);		//���� ,��ƽ1��Ϊ0011����ƽ0��Ϊ1100
//void transCode(unsigned char *doData,unsigned char len);//���룬�����յ������ݻ�ԭ

//t=1ʱ���ӳ�100us����
void Delay3(unsigned int t)
{
	unsigned int i,j;
	for(i=0;i<t;i++)		
	for(j=0;j<19;j++);
}
void Delay4(unsigned int t)
{
	unsigned int i,j;
	for(i=0;i<t;i++)		
	for(j=0;j<26;j++);
}

//init signal�����ͱ����ź�ǰ����ʼ�źţ����ڽ����ջ����Զ������
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
//	Delay3(80);
}

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
//	Delay3(80);
}

void main()
{
	SensorControl=0;		  //�ϵ�رմ�����

	noVoice();
	InitT0();
	InitT1();
	
	PAshutdown=0;		//�����Źر�

	upSignal=1;
	downSignal=1;
	ET0=1;	//������ʱ��0�ж�
	ET1=1; //������ʱ��1�ж�
	PT1=1;//��ʱ��1���ж����ȼ����
	EA=1;
	P10=1;
//	det_charge=1;

	BatteryControl=0;	//�����ϵ��ʱ����0�������Գ�磬�����û�г����������Ϊ�͵�ƽ
	myPwm();	//�������

	ModeControl_1=0; //�����ģʽ���ƶ�,����ʱΪ30Mģʽ
	
//	MagentControl_1=0;//�رմ���
//	MagentControl_2=1;
//	Delay(50);
//	MagentControl_1=0;//������̬Ϊ����ģʽ
//	MagentControl_2=0;
	magnetflag=0;

	commuFlag=1; //����ͨ��
	tran_en=0;   //�ر����߷����

	downUpFlag=1;
	det_battery=1;

	while(1)
	{
		if((det_battery==1)&&(turnonflag==0))		   //����ת��Կ��ʱ��ִ��һ�ε���ת��
		{
		 	Delay(30);
			if(det_battery==1)
			{
				verifybattery();
				PAshutdown=1;
				SC_Speech(7);  
				Delay(160);
				PAshutdown=0;
				
/*				if(turnon_speech_flag==0)
				{
					PAshutdown=1;
					SC_Speech(8);  
					Delay(80);
					PAshutdown=0;
					turnon_speech_flag=1;
				}
				else
				{
					PAshutdown=1;
					SC_Speech(7);  
					Delay(240);
					PAshutdown=0;
					turnon_speech_flag=0;
				}
*/
				turnonflag=1;
			}
		}
		
		if((det_battery==0)&&(turnonflag==1))
		{
		 	Delay(30);
			if(det_battery==0)
			{
				verifybattery();
				if((Check<0x300))						  //С��38V������С��5������
				{
					PAshutdown=1;
					SC_Speech(10);  //����3.6v����������ʾ
					Delay(100);
					PAshutdown=0;
				}
				PAshutdown=1;
				SC_Speech(9);
				Delay(130);
				PAshutdown=0;
				turnonflag=0;
			}
		}

		if(ADCcheck==1)
		{
			Check=GetADCResult(6);	//��ص������
			ADCcheck=0;	
		}

		if(magcon==1)
		{
			if(magnetflag==1)
			{
				MagentControl_1=1;//�رմ���
				MagentControl_2=0;
				Delay(40);
				MagentControl_1=0;//������̬Ϊ����ģʽ
				MagentControl_2=0;
				magnetflag=0;
			}
			magcon=0;
		}

		if(magcon2==1)
		{
			if(magnetflag==0)
			{
				MagentControl_1=0;//��������
				MagentControl_2=1;
				Delay(40);
				MagentControl_1=0;//������̬Ϊ����ģʽ
				MagentControl_2=0;
				magnetflag=1;
			}
			magcon2=0;
		}
/*
		if(sendspeech1==1)
		{
			PAshutdown=1;
			SC_Speech(17);  //�ػ���������
			Delay(80);
			PAshutdown=0;
			sendspeech1=0;
		}
*/
/*
		if(sendspeech2==1)
		{
			PAshutdown=1;
			SC_Speech(18);  //�ػ���������
			Delay(100);
			SC_Speech(16);  //�ػ���������
			Delay(100);
			PAshutdown=0;
			sendspeech2=0;
		}
*/
		if((sendspeech3==1)&&(speech3_count<4))
		{
			if((upSignal==1)&&(downSignal==1))
			{
				PAshutdown=1;
				SC_Speech(22);  //�ػ���������
				ComMode_3_Data();
				Delay(100);
				SC_Speech(23);  //�ػ���������		
				ComMode_3_Data();
				Delay(60);
				PAshutdown=0;
			}
			speech3_count++;
			if(speech3_count==4)
			{
				speech3_count=0;
				sendspeech3=0;
				stolenflag=0;
			}
		}

/*		if(sendspeech3==1)
		{
			PAshutdown=1;
			SC_Speech(18);  //�ػ���������
			Delay(120);
			SC_Speech(19);  //�ػ���������
			Delay(140);
			SC_Speech(20);  //�ػ���������
			Delay(170);
			PAshutdown=0;
			sendspeech3=0;
		}
*/
		if((sendspeech7==1)&&(speech7_count<1))
		{
				sendspeech8=0;
				speech8_count=0;
				
				PAshutdown=1;
				SC_Speech(11);  //�ػ���������
				Delay(150);
				PAshutdown=0;
				speech7_count++;
		}

		if((sendspeech8==1)&&(speech8_count<1))
		{
				sendspeech7=0;
				speech7_count=0;
				
				PAshutdown=1;
				SC_Speech(12);  
				Delay(80);
				SC_Speech(13);
				Delay(80);
				PAshutdown=0;
				speech8_count++;
				SensorControl=1;	//�������ᴫ����
		}
	}
}

void timeT1() interrupt 3 //��ʱ��1�жϽ�������
{

	TH1=timer1H;//��װ��
	TL1=timer1L;
	
	if(P11==0)//�������Ϊ�ߵ�ƽ,�е͵�ƽ˵�����ź�
	{
		DataBetween++;
		ComFlag=0;
		if(DataBetween==150)//�͵�ƽ���������ʱ��	
		{
			DataBetween=0;
		}
	}
	else//Ϊ�ߵ�ƽ��
	{
		if(ComFlag==0)//˵����һ���͵�ƽ
		{
			ComFlag=1;

			if((DataBetween>60)&&(DataBetween<=100))	//�͵�ƽ������ʱ��С��10ms����Ϊ0
			{
				RecData<<=1;
				RecData &= 0xfe;
				DataTime++;
				T1highcount=0;
			}
			else if((DataBetween>100))//�͵�ƽ������ʱ�����4.5ms����Ϊ1
			{
				RecData<<=1;
				RecData |= 0x01;
				DataTime++;
				T1highcount=0;
			}
			else
			{
				T1highcount++;	
			}

			DataBetween=0;
		}

		else
		{
			T1highcount++;
			if(T1highcount>=120)
			{
				DataTime=0;
				ComFlag=1;
				count=0;
			}		
		}
	}

	if(DataTime==8)//˵��һ���ֽڵ������Ѿ�������ȫ
	{
		DataTime=0;
		myTxRxData2[count]=RecData;
		if(count==0&&myTxRxData2[0]==CmdHead)
		{
			count=1;
		}
		else if(count==1&&myTxRxData2[1]==MyAddress)
		{
			count=2;
		}
		else if(count==2)
		{
			receiveFlag=1;
			count=0;
		}
		else 
		{
			count=0;
		}
/*
		else if(count>=2&&count<=5)
		{
			count++;
		}
		else if(count==6)
		{
		    receiveFlag=1;
			count=0;
		}
		else 
		{
			count=0;
		}
*/
	}

	if(receiveFlag==1)
	{
		receiveFlag=0;
		switch(myTxRxData2[2]) 		//������֡���������д���
		{
			case ComMode_1:  		//�������͹�����ֻ��ģʽ1��˵�������������ģ����ݲ���Ϊ����ĵ�һ�͵ڶ����ֽڣ�Ϊ������ڵ��������Ŀ�ʼ�ֽڵ��Ǹ���ַ��Ȼ���������֡�������������ݷ��ͳ�ȥ
			{
//				sendcomm1=1;
				stolenflag=0;
				
				ComMode_1_Data(); 	//�򸽻����ͱ���3

				sendspeech7=1;		//����1��һ������

//				sendspeech8=0;
//				speech8_count=0;

				alarmFlag=0;		//�ر�����־λ
//				alarmCount=0;		//����������������
				SensorControl=0;	//���ᴫ����
				downUpFlag=0; 		//�ص��ء�̧����
				downFlag=0;
				upFlag=0;
				magcon2=1;			//�򿪵����
				
				SensorCount=0;
				time0Count_2=0;
				stolen_count=0;
				stolen_flag=0;
				sendspeech1=0;
				sendspeech2=0;
				sendspeech3=0;
				speech3_count=0;

				TestFlag=0;	
				if(ModeFlag==3||ModeFlag==2)
				{
					ModeFlag=1;
				}
			}
			break;
		}
	}
}

void time0() interrupt 1	//��Ϊ����ϵͳ�Լ���ʱ��
{
	TH0=timer0H;//��װ��
	TL0=timer0L;
	time0Count_3++;

	if(time0Count_3>=2000)//����ÿ3s����һ�ε����ݵ�ʱ���־
	{
		if(commuFlag==1)//˵��������ͨ��
		{
			TestFlag++;

			if(TestFlag>=4&&ModeFlag==1)//˵��û�н��յ������Ѿ���3���ˣ������Ѿ�����3M�����ھ�Ҫ�Ӵ��ʣ��л���ģʽ2,30M�ٿ��ܲ��ܽ��յ�����
			{
				TestFlag=5;
				if(ModeFlag==1)
				{
					magcon=1;		 	//���������
					sendspeech8=1;		//�������뿪����
					
//					sendspeech7=0;
//					speech7_count=0;

//					SensorControl=1;	//�������ᴫ����
					downUpFlag=1;		//�������ء�̧���־
					ModeFlag=2;

					SensorCount=0;
					time0Count_2=0;
				}	
 			}
 		}
		time0Count_3=0;
		
//		ComMode_1_Data();

		ADCcheck=1;		
		
		if((downFlag==1)&&(downcount<5))  //���غ�����Ӧ�Ķ���
		{
//			sendcomm5=1;
			ComMode_5_Data(); //�򸽻����ͱ���3
			downcount++;
		}
		if((upFlag==1)&&(upcount<5))		//̧�������Ӧ����
		{
//			sendcomm4=1;
			ComMode_4_Data(); //�򸽻����ͱ���3
			upcount++;
		}

/*
	if((stolenflag==1)&&(speech3_count<4))
		{
			ComMode_3_Data();	
		}
*/
	}

	if(SensorControl==1)	//������ᴫ�����Ƿ�򿪣����һ�û�б���
	{
		if((ReceWave==1)&&(stolenflag==0))
		{
			time0Count_2++;
			if(time0Count_2>=8)				 //ÿ1ms���һ�θߵ�ƽ�����������6ms�ĸ߶�ƽ��˵����������һ��
			{
				time0Count_2=0;
				SensorCount++;
				speech1_count=0;
				stolen_flag=1;
			}
		}
		else
		{
			time0Count_2=0;
		}
		
		if(stolen_flag==1)
		{
			stolen_count++;
			if(stolen_count>=6000)
			{
				SensorCount=0;
				time0Count_2=0;
				stolen_count=0;
				stolen_flag=0;
				sendspeech1=0;
				speech1_count=0;
				sendspeech2=0;
			}
		}
	}

	if(SensorCount==1)
	{
//		sendspeech1=1;
		if((speech1_count<1)&&(sendspeech3!=1))
		{
			if((downSignal==1)&&(upSignal==1))
			{
				PAshutdown=1;
				SC_Speech(17);  //�ػ���������
				Delay(80);
				PAshutdown=0;
				speech1_count++;
			}
		}
	}
	else
	{
		if(stolen_count>=3000)
		{
			if(ReceWave==1)
			{
				time0Count_6++;
				if(time0Count_6>=6)	
				{
					sendspeech3=1;
					speech3_count=0;
					stolenflag=1;
					time0Count_6=0;	
				}
			}
			else
			{
				time0Count_6=0;
			}
		}						 
	}

//	��⵹�غ�̧����Ĵ���
	if(downUpFlag==1)//������̧�𵹵ؼ��
	{
		if(upSignal==0)//˵����̧���źŲ����ǵ�һ�Σ���ʼ��ʱ
		{
			time0Count_4++;
			if(time0Count_4==10)//˵���Ѿ�����0.5S
			{
				upFlag=1;//��̧���־
				downFlag=0;
				alarmFlag=0;
			}		
		}
		else
		{
			upFlag=0;
			upcount=0;
			time0Count_4=0;
		}

		if(downSignal==0)//˵����̧���źţ���ʼ��ʱ
		{
			time0Count_5++;
			if(time0Count_5==10)//˵���Ѿ�����0.5S
			{
				downFlag=1;//��̧���־
				upFlag=0;
				alarmFlag=0;
			}		
		}
		else
		{
			downFlag=0;
			downcount=0;
			time0Count_5=0;
		}
	}
}


void ComMode_1_Data()		//���ͱ���1
{
	unsigned char i,n;
	ModeControl_1=0;//30M���书��				
	tran_en=1;
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
	tran_en=0;
}

void ComMode_3_Data()//���ͱ���3
{
//	unsigned int j;
	unsigned char i,n;
	ModeControl_1=1;//�л�Ϊ300M����
	tran_en=1;
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
				Delay3(120);//��ʱ4.5ms���ϣ����ڶ�ʱ��ռ�����⣬ֻ����������ʱ��ʵ��
			}
			else//Ϊ0�����
			{
				P10=0;
				Delay3(80);//��ʱ2ms�����ڶ�ʱ��ռ�����⣬ֻ����������ʱ��ʵ��
			}
			P10=1;//��̬Ϊ�ߵ�ƽ
			myTxRxData[i]<<=1;
			Delay3(50);//��ʱҪ����2ms
		}
	}
	tran_en=0;
}

void ComMode_4_Data()//����̧�����
{
	unsigned char i,n;
	ModeControl_1=0;//�л�Ϊ300M����
	tran_en=1;
	myTxRxData[0]=CmdHead;
	myTxRxData[1]=MyAddress;
	myTxRxData[2]=ComMode_4;
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
				Delay3(120);//��ʱ4.5ms���ϣ����ڶ�ʱ��ռ�����⣬ֻ����������ʱ��ʵ��
			}
			else//Ϊ0�����
			{
				P10=0;
				Delay3(80);//��ʱ2ms�����ڶ�ʱ��ռ�����⣬ֻ����������ʱ��ʵ��
			}
			P10=1;//��̬Ϊ�ߵ�ƽ
			myTxRxData[i]<<=1;
			Delay3(50);//��ʱҪ����2ms
		}
	}
	tran_en=0;
}

void ComMode_5_Data()//���͵��ر���
{
	unsigned char i,n;
	ModeControl_1=0;//�л�Ϊ300M����
	tran_en=1;      //�����߷����
	myTxRxData[0]=CmdHead;
	myTxRxData[1]=MyAddress;
	myTxRxData[2]=ComMode_5;
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
				Delay3(120);//��ʱ4.5ms���ϣ����ڶ�ʱ��ռ�����⣬ֻ����������ʱ��ʵ��
			}
			else//Ϊ0�����
			{
				P10=0;
				Delay3(80);//��ʱ2ms�����ڶ�ʱ��ռ�����⣬ֻ����������ʱ��ʵ��
			}
			P10=1;//��̬Ϊ�ߵ�ƽ
			myTxRxData[i]<<=1;
			Delay3(50);//��ʱҪ����2ms
		}
	}
	tran_en=0;
}

void verifybattery()
{									  
	if((Check>=0x3a3))//���ñȽϵ�ѹ���˴���4.96VΪ��׼������47.4V��4.5V��
	{
		PAshutdown=1;
		SC_Speech(6);  	//4V����������ʾ
		Delay(130);
		PAshutdown=0;
	}
    else if((Check<0x3a0)&&(Check>=0x37c))		//С��47.4������45��4.31V��
	{
		PAshutdown=1;
		SC_Speech(5);  //3.8V����������ʾ
		Delay(130);
		PAshutdown=0;
	}
	else if((Check<0x378)&&(Check>=0x359))		 //С��45������43��4.14V��
	{
		PAshutdown=1;
		SC_Speech(4);  //3.6V����������ʾ
		Delay(130);
		PAshutdown=0;
	}
	else if((Check<0x355)&&(Check>=0x327))		 //С��43������40��3.9V��
	{
		PAshutdown=1;
		SC_Speech(3);  //����3.6v����������ʾ
		Delay(130);
		PAshutdown=0;
	}
	else if((Check<0x323)&&(Check>=0x304))		  //С��40������38��3.73V��
	{
		PAshutdown=1;
		SC_Speech(2);  //����3.6v����������ʾ
		Delay(130);
		PAshutdown=0;
	}
}