/*---------------------------------------------------
	main.c (v1.00)
	
	DDC-Z program, for electrocar.
----------------------------------------------------*/

//�綯��
#include "Main.h"
#include "port.h"

#include "AD.h"
#include "voice.h"
#include "pwm.h"
#include "Timer.h"
#include "Delay.h"
#include "communication.h"
#include "Battery.h"
#include "Other.h"
#include "operation.h"
                                        
/*------- Public variable declarations --------------------------*/
extern bit stolen_alarm_flag;		//��������������־��1��ʱ���ʾ����		
extern bit host_stolen_speech_EN;		//������������ʹ�ܣ�1��ʱ����main��������ʾ
extern bit battery_check;		         //��1ʱ��ִ��һ�ε���ת����ִ����󣬽�����0
extern bit position_sensor_EN;  			//λ�ô�������������̧�𴫸����ܿ��أ�1��ʱ�򣬼��������������
extern bit slave_away_speech_EN;      	//�жϸ����뿪��������ʾ����main�������
extern bit magnet_CW_EN;					//�����˳ʱ��ת��ʹ�ܣ�ת��һ�κ�λΪ0
extern bit comm_whole_control;				//ͨ���ܿ��أ�0�ر�ͨ�ţ�1��ͨ��
extern tWord host_stolen_speech_count;    //�ж�����������������ʾ�Ĵ���
extern bit magnet_ACW_EN;						//�������ʱ��ת��ʹ�ܣ�ת��һ�κ�λΪ0
extern bit slave_nearby_speech_EN;       //�жϸ���������������ʾ����main�������
extern bit host_stolen_alarm1_EN;      //�ж�Ϊ����������ĵ�һ������ʹ��
extern bit host_stolen_alarm2_EN;      //�ж�Ϊ����������ĵڶ�������ʹ��
extern tByte host_stolen_alarm1_count;		//�ж�Ϊ����������ĵ�һ����������
extern tByte host_stolen_alarm2_count;		//�ж�Ϊ����������ĵڶ�����������
extern bit fell_flag;						//�ж��������º���1
extern tByte slave_away;					//ģʽѡ��λ��1����ģʽ1,2����ģʽ2,3��Ϊģʽ3
extern tByte leave_count;					//�����뿪ʱ�ļ�����ֻҪ����3sӦ����ȷ������0	
extern bit raised_flag;					//�ж�������̧�����1
extern tByte sensor_2ndstage_LV_time; 	//����������ڶ��׶μ��ʱ������͵�ƽ��ʱ��
extern tByte sensor_2ndstage_count;		//����������ڶ��׶μ��ʱ������͵�ƽ��ʱ��
extern tByte host_2ndtouch_speech_count;		//�������ڶ��δ�����������ʾ�Ĵ���
extern tWord sensor_3rdstage_time;			//��������������׶ε�ʱ�䣬
extern tByte sensor_3rdstage_effcount;		//��������������׶κ���Ч���������ļ���
extern tByte sensor_3rdstage_count;			//��������������׶κ󣬵͵�ƽ�ļ���
extern tWord sensor_3rdstage_interval;		//�������ڵ����׶��У�ÿ����Ч�͵�ƽ����֮���ʱ�����������ڼ�ĵ͵�ƽ����Ϊ��Ч��
extern bit host_touch_speech_EN;				//��һ�δ���������ʹ��
extern bit host_2ndtouch_speech_EN;			//�ڶ��δ���������ʹ��
extern bit raised_fell_flag;					//���ػ���̧�𴥷��󣬴˱�־λ��1
extern tByte sensor_trigger_count;			//��������������
extern tWord sensor_2ndstage_time;			//����������ڶ��׶κ�����ʱ��ļ���
extern tByte sensor_1ststage_count;			//��������һ�׶��жϵ͵�ƽ�ļ���
extern tByte nearby_away_interval;		//�����뿪�Ϳ���ʱ��������ʾ�Ϳ�������ʱ����
extern bit magnet_ACW_flag;
extern bit slave_nearby_EN;			// ��Ϊslave�����������ʹ��
extern tByte key_rotated_on_flag;			//�綯�������رձ�־λ��1��ʾ�綯�������ˣ�0��ʾ�綯���ر���
extern tByte slave_nearby_operation_count;	// ��Ϊslave����������Ĵ���
extern tWord ADC_check_result;		//��ΪAD���ֵ

/*------- Private variable declarations --------------------------*/

void main()
	{
	InitTimer(1,100);
	
	sensor_EN = 0;
 	position_sensor_EN = 0;

	noVoice();
	myPwm();	//�������

	voice_EN=0;		//�����Źر�

	raised_sensor_detect=1;
	fell_sensor_detect=1;

	P10=1;
	
	magnet_ACW_flag=0;
   horizontal_sensor = 1;
	// open communication
	comm_whole_control = 1; 
	
	// turn off transmitter, turn on receiver
	transmiter_EN = 1;
	receiver_EN = 0;


	nearby_away_interval = 6;			//��ʼ��ʹ�������������ػ��ļ���Ϊ����ִ�е�״̬
	
	// initialize the magnet, 
	MagentControl_1 = 1;
	MagentControl_2 = 1;
	
	transmiter_power = 1; 
   
	// lock the external motor, ��ֹ����û��ȫ�򿪵�ʱ�򣬳��ּӵ絼�����������ĸ�����ײ�� 
	motor_lock = 1;
	
	// detect the battery voltage
	ADC_check_result = GetADCResult(6);

	wire_broken = 0;

	while(1)
		{
		sEOS_Go_To_Sleep();
		}
	}



/*---------------------------------------------------
	end of file
----------------------------------------------------*/