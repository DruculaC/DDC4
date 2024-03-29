/*---------------------------------------------------
	Timer.c (v1.00)
	
	Timer functions
---------------------------------------------------*/	

#include "main.h"
#include "port.h"

#include "Timer.h"
#include "communication.h"
#include "voice.h"
#include "Delay.h"
#include "Battery.h"
#include "AD.h"
#include "Other.h"
#include "operation.h"

// ------ Public variable declarations -----------------------------
bit stolen_alarm_flag = 0;					// when host been touch 3 times, this flag 1 before alarm voice present, not to detect sensor for 1st voice alarm.
bit host_stolen_speech_EN = 0;			// when host been touch 3 times, enable 3rd alarm voice.
bit battery_check=0;							// 2置1时，执行一次电量转换，执行完后，将其置0
bit position_sensor_EN=0;  		//位置传感器，即倒地抬起传感器总开关，1的时候，检测这两个传感器
bit slave_away_speech_EN=0;      //判断附机离开后，语音提示，在main里面操作
bit magnet_CW_EN = 0;					//电磁铁顺时针转动使能，转动一次后复位为0
bit magnet_ACW_EN=0;						//电磁铁逆时针转动使能，转动一次后复位为0
bit comm_whole_control=0;				//通信总开关，0关闭通信，1打开通信
tWord host_stolen_speech_count=0;   //判断主机被盗后，语音提示的次数
bit slave_nearby_speech_EN=0;       //判断附近靠近后，语音提示，在main里面操作
bit host_stolen_alarm1_EN = 0;      //判断为被盗报警后的第一段语音使能
bit host_stolen_alarm2_EN = 0;      //判断为被盗报警后的第二段语音使能
tByte host_stolen_alarm1_count = 0;		//判断为被盗报警后的第一段语音次数
tByte host_stolen_alarm2_count = 0;		//判断为被盗报警后的第二段语音次数
bit fell_flag=0;						//判断主机倒下后，置1
bit slave_away = 1;					// flag for slave, 1 means away, 0 means nearby
tByte leave_count=0;					// increment every 2s, reset for every success communication
bit raised_flag=0;					//判断主机被抬起后，置1
tByte sensor_2ndstage_LV_time=0; 	//传感器进入第二阶段检测时，计算低电平的时间
tByte sensor_2ndstage_count = 0;		//传感器进入第二阶段检测时，计算低电平的时间
tByte host_2ndtouch_speech_count = 0;		//主机被第二次触碰后，语音提示的次数
tWord sensor_3rdstage_time = 0;			//传感器进入第三阶段的时间，
tByte sensor_3rdstage_effcount = 0;		//传感器进入第三阶段后，有效触碰次数的计数
tByte sensor_3rdstage_count = 0;			//传感器进入第三阶段后，低电平的计数
tWord sensor_3rdstage_interval = 0;		//传感器在第三阶段中，每次有效低电平计数之间的时间间隔。在这期间的低电平不认为有效。
bit host_touch_speech_EN = 0;				//第一次触碰后语音使能
bit host_2ndtouch_speech_EN = 0;			//第二次触碰后语音使能
bit raised_fell_flag = 0;					//倒地或者抬起触发后，此标志位置1
tByte sensor_trigger_count=0;		//传感器触发计数
tWord sensor_2ndstage_time=0;		//传感器进入第二阶段后流逝时间的计数
tByte sensor_1ststage_count=0;	//传感器第一阶段判断低电平的计数
tByte nearby_away_interval = 0;		//附机离开和靠近时，语音提示和开关锁的时间间隔
bit magnet_ACW_flag=0;

// ------ Private variable definitions -----------------------------
tByte timer0_8H, timer0_8L, timer1_8H, timer1_8L;		// register value of timer0 and timer1, caculated from InitTimer()

tByte host_touch_speech_count=0;
tByte host_touched_flag=0;			//主机被触碰后置1，进行后面的传感器判断
tByte raised_alarm_count = 0;    //主机被抬起后，向附机发出报警信号的次数
tByte fell_alarm_count=0;        //主机倒地后，向附机发出报警信号的次数
tWord timer0_count=0;		// counter for timer0, increment every ticket 			
tByte received_data_buffer[7]={0x00,0x00,0x00,0x00,0x00,0x00,0x00};		//接收数据缓存
bit receive_data_finished_flag = 0;		//接收这一串数据完成后，此标志位置1
tByte data_count = 0;				//接收数据缓存的位数，即表明第几个缓存数据
tByte one_receive_byte = 0;		//接收数据的一个字节，接收完后将其赋值给received_data_buffer相对应的字节
tByte one_receive_byte_count = 0;			//one_receive_byte有8位，此计数表明接收到第几位，每次计数到8的时候表明一个字节接收完成。
bit receive_wire_flag = 1;		//接收通信线的标志位，1表明高电平，0表明低电平，每次timer1溢出时，判断P1.1一次。以此来表明是否为一次正确的下降沿
tByte receive_HV_count = 0;		//定时器T1在没有信号到来的时候，对高电平计数，一旦超过某个值，则将one_receive_byte_count清0
tByte receive_LV_count = 0;		//每次timer1溢出时判断接收线如果为LV，则计数加1，以此来表明低电平的时间
tByte fell_wire_time=0;          //倒地检测线，检测低电平的时间
tByte raise_wire_time=0;			//抬起检测线，检测低电平的时间
tWord raised_fell_number = 0;				//倒地或者抬起出发后，计数，到达一定数值后，将其与标志位一起清零。
bit raised_fell_once_flag = 0;			//raised_fell_flag是否曾经标志过，如果标志过则置1.然后主机被恢复倒地或者恢复抬起时，此标志位复位。
tByte key_rotated_on_flag=0;			//电动车开启关闭标志位，1表示电动车开启了，0表示电动车关闭了
tByte slave_nearby_speech_count=0;
tByte slave_away_speech_count=0;
tWord ADC_check_result = 0;		//作为AD检测值
bit slave_nearby_EN = 0;			// 作为slave靠近后操作的使能
tByte slave_nearby_operation_count = 0;	// 作为slave靠近后操作的次数
tByte wire_broken_count = 0;		// 作为断线后的时间检测
bit battery_stolen_EN = 0;			// 作为电池被盗的使能端
tByte battery_stolen_count = 0;	// 作为电池被盗的报警次数
bit horizontal_vibration = 0;		// 表示垂直传感器在振动，此时就算关钥匙，也不能执行关钥匙的操作�
tWord horizontal_vibration_count = 0;	//垂直传感器触发后，对时间进行计数。
bit vibration_flag = 0;
tWord vibration_count = 0;

/*------------------------------------------------------------------
	timerT0()
	operation every ticket.
--------------------------------------------------------------------*/

void timer0() interrupt interrupt_timer_0_overflow
	{
	// manually reload timer0 configuration
	TH0 = timer0_8H;
	TL0 = timer0_8L;
	
	// timer0 is 1ms ticket, count the time flow of timer0, then operate every 2s.
	if(++timer0_count >= 2000)
		{

		// transmit the verification signal
		ComMode_1_Data(); 				
		
		// judge the communication control is on
		if(comm_whole_control == 1)
			{
			// if communication is on, increase the leave_count every 2s.
//			if((++leave_count >= 2)&&(slave_away == 0))
			if(++leave_count >= 2)
				{
				leave_count = 4;
				
				if((vibration_flag == 0)&&(slave_away == 0)&&(slave_nearby_EN == 1))
					{
					if((nearby_away_interval > 6)&&(key_rotated_on_flag == 1))
						{
						// turn off the magnet 
						magnet_ACW();
						nearby_away_interval = 0;
						
						// flag slave already away
						slave_away = 1;
						
						slave_nearby_EN = 0;
						slave_nearby_operation_count = 0;
						}
					}
            if(sensor_EN == 0)
					{
					// reset relatively sensor count
					sensor_trigger_count=0;
					sensor_1ststage_count=0;										
					}
								
				if(vibration_flag == 0)
					{
					// enable position sensor(vertical and horizontal) and vibration sensor
					position_sensor_EN=1;	
					sensor_EN = 1;					
					}
				}
			}
		// reset timer0 ticket counter every 2s
		timer0_count=0;
		
		// detect the battery voltage
		ADC_check_result = GetADCResult(6);	
		
		// if fell and raised flag is 1, send alarm signal every 2s.
		if((fell_flag==1)&&(fell_alarm_count<5))
			{
			ComMode_5_Data();
			fell_alarm_count++;
			}
		if((raised_flag==1)&&(raised_alarm_count<5))		
			{
			ComMode_4_Data();
			raised_alarm_count++;
			}
			
		if(++nearby_away_interval > 6)
			{
			nearby_away_interval = 7;
			}
			
		// detect whether key is rotated on,  
		if((key_rotated_on_flag == 0)||(slave_away == 1))
			{
			if(key_rotate == 0)		
				{
				Delay(5);
				// anti-trigger, Delay(5) confirm the key rotation.
				if(key_rotate == 0)
					{
					if((slave_nearby_EN == 1)&&(slave_nearby_operation_count < 1))
						{
						slave_nearby_operation();
						// flag key rotation status
						key_rotated_on_flag = 1;
						// reset the battery stolen flag
						battery_stolen_EN = 0;
						battery_stolen_count = 0;
						leave_count = 0;
						}
					
					if((host_stolen_alarm1_EN == 1)&&(host_stolen_alarm1_count < 4)&&(key_rotated_on_flag == 0))
						{
						// reset all the flag in sensor
						fell_flag=0;  
						raised_flag=0;
						sensor_trigger_count = 0;
						sensor_1ststage_count = 0;
						sensor_2ndstage_count = 0;
						sensor_2ndstage_time = 0;
						sensor_3rdstage_time = 0;
						sensor_3rdstage_interval = 0;
						sensor_3rdstage_count = 0;
						sensor_3rdstage_effcount = 0;
						stolen_alarm_flag = 0;
						host_stolen_alarm1_EN = 0;
						host_stolen_alarm1_count = 0;
						host_stolen_alarm2_EN = 0;
						host_stolen_alarm2_count = 0;
						
						raised_fell_flag = 0;
						
						// reset the wire broken count
						wire_broken_count = 0;
						
						// reset the battery stolen flag
						battery_stolen_EN = 0;
						battery_stolen_count = 0;
						}
					}
				} 
			}
		
		// detect whether key is rotated off
		if(vibration_flag == 0)
			{
			if((key_rotate == 1)&&(key_rotated_on_flag == 1))
				{
				Delay(5);
				if(key_rotate == 1)
					{
					Delay(10);
					// handle with battery status
					verifybattery(ADC_check_result);
					// reset key rotation flag
					key_rotated_on_flag = 0;
					
					slave_away_operation();
					}
				
				leave_count = 0;
				}				
			}
		
		// whether host has been touched 3 times, if yes, then alarm 2 speech alternantively.
		if((host_stolen_alarm1_EN == 1)&&(host_stolen_alarm1_count < 4))
			{
			stolen_alarm_flag = 1;
			if((key_rotate == 1)||((key_rotate == 0)&&(slave_away == 1)))
				{
				ComMode_3_Data();
				stolen_alarm_speech1();
				}
			if(++host_stolen_alarm1_count >= 4)
				{
				host_stolen_alarm1_count = 0;
				host_stolen_alarm1_EN = 0;
				stolen_alarm_flag = 0;
				}
			}
		if((host_stolen_alarm2_EN == 1)&&(host_stolen_alarm2_count < 4))
			{
			stolen_alarm_flag = 1;
			if((key_rotate == 1)||((key_rotate == 0)&&(slave_away == 1)))
				{
				ComMode_3_Data();
				stolen_alarm_speech2();
				}
			if(++host_stolen_alarm2_count >= 4)
				{
				host_stolen_alarm2_count = 0;
				host_stolen_alarm2_EN = 0;
				stolen_alarm_flag = 0;
				}
			}
		
		if((battery_stolen_EN == 1)&&(battery_stolen_count < 6))
			{
			ComMode_2_Data();
			battery_stolen_speech();
			battery_stolen_count++;
			}
		}

	
	if(vibration_flag == 1)
		{
		if(++vibration_count > 3000)
			{
			vibration_flag = 0;
			vibration_count = 0;
			}
		}		
	if((sensor_detect == 0)||(horizontal_sensor == 0))
		{
		vibration_flag = 1;
		vibration_count = 0;
		}		

	// judge host is fell or raised every 1ms?
	if(raised_fell_flag == 0)
		{
		// judge vibration sensor is enable?
		if(sensor_EN == 1)	
			{
			
			// sensor trigger status, 0, 1 or 2?
			switch(sensor_trigger_count)
				{
				// case 0, it means host is in waiting status. waiting for first touch.
				case 0:
					{
					
					// judge host been touched and also not in vibration alarm
//					if((sensor_detect == 0)&&(stolen_alarm_flag == 0)&&(transmiter_EN == 1))		
					if(((sensor_detect == 0)||(horizontal_sensor == 0))&&(stolen_alarm_flag == 0))		
						{
						// judge LV is more than 2ms, if yes, it means a effective touch
						if(++sensor_1ststage_count >= 2)			
							{
							sensor_1ststage_count=0;
							
							// sensor trigge status progress to case 1.
							sensor_trigger_count = 1;
							// alarm speech for first touoch
							host_touch_speech();
                     }
						}
					else
						{
						// if no LV, reset the count.
						sensor_1ststage_count = 0;
						}
					}
				break;
				
				// waiting for next touch, 
				case 1:
					{
					if((sensor_detect == 0)||(horizontal_sensor == 0))
						{
						// LV for 2s, means a effective touch
						if(++sensor_2ndstage_count >= 2)
							{
							sensor_2ndstage_count = 0;
							sensor_trigger_count = 2;
							host_2ndtouch_speech_count = 0;
							// alarm speech for 2nd touch
							host_2ndtouch_speech();
							}
						}
					else
						{
						sensor_2ndstage_count = 0;
						}
					
					// if there is no touch in 4s, reset sensor trigger status, etc.
					if(++sensor_2ndstage_time >= 4000)
						{
						sensor_trigger_count = 0;
						sensor_2ndstage_count = 0;
						sensor_1ststage_count = 0;
						sensor_2ndstage_time = 0;
						}
					}
				break;
				
				// waiting for 3rd touch
				case 2:
					{
					if((sensor_detect == 0)||(horizontal_sensor == 0))
						{
						// 2s LV is a effective touch
						if(++sensor_3rdstage_count >= 2)
							{
							sensor_3rdstage_count = 0;
							// stolen alarm speech enable
							host_stolen_alarm1_EN = 1;
							host_stolen_alarm2_EN = 1;						
							}
						}
					else
						{
						sensor_3rdstage_count = 0;
						}
					
					// if there is no touch in 4s, reset all.
					if(++sensor_3rdstage_time >= 4000)
						{
						sensor_trigger_count = 0;
						sensor_1ststage_count = 0;
						sensor_2ndstage_count = 0;
						sensor_2ndstage_time = 0;
						sensor_3rdstage_time = 0;
						sensor_3rdstage_interval = 800;
						sensor_3rdstage_count = 0;
						sensor_3rdstage_effcount = 0;					
						}
					}
				break;
				}
			
			// judge the wire broken, if yes, it means someone has cut the wire of magnet lock
			if((wire_broken == 0) && (wire_broken_count < 51))  
				{
            if(++wire_broken_count > 50)
					{
					host_stolen_alarm1_EN = 1;
					host_stolen_alarm2_EN = 1;	
					wire_broken_count = 51;
               }
				}
			                                                                                               
			// judge whether battery has been stolen, 0.4V/4V
			if(ADC_check_result < 0x100)
				{
				battery_stolen_EN = 1;
				}
			else if(ADC_check_result > 0x200)
				{
				battery_stolen_EN = 0;
				battery_stolen_count = 0;
				}
			}
		}
	
	// judge whether position sensor is enable
	if(position_sensor_EN==1)		
		{
		// judge whether there is a LV
		if(raised_sensor_detect == 0)	
			{
			// LV > 0.5s means a effective input
			if(++raise_wire_time==10)
				{
				// flag raised, and reset fell
				raised_flag=1;
				fell_flag=0;
				// judge whether there once been a raised or fell.
				if(raised_fell_once_flag == 0)
					{
					// if no, flag raised and fell flag
					raised_fell_flag = 1;					
					}
				}		
			}
		else
			{
			raised_flag=0;
			raised_alarm_count=0;
			raise_wire_time=0;
			raised_fell_flag = 0;
			}
      // judge whether there is a LV
		if(fell_sensor_detect==0)
			{
			// LV > 0.5s means a effective input
			if(++fell_wire_time==10)	
				{
				// flag fell, and reset raised
				fell_flag=1;			
				raised_flag=0;
				// judge whether there once been a raised or fell
				if(raised_fell_once_flag == 0)
					{
					raised_fell_flag = 1;					
					}
				}		
			}
		else
			{
			fell_flag=0;
			fell_alarm_count=0;
			fell_wire_time=0;
			raised_fell_flag = 0;
			}
		}
	
	// judge whether raised or fell, if yes, reset all after 10s.
	if(raised_fell_flag == 1)
		{
		if(++raised_fell_number >= 4000)
			{
			raised_fell_flag = 0;
			raised_fell_number = 0;
			raised_fell_once_flag = 1;
			
			sensor_trigger_count = 0;
			sensor_1ststage_count = 0;
			sensor_2ndstage_count = 0;
			sensor_2ndstage_time = 0;
			sensor_3rdstage_time = 0;
			sensor_3rdstage_interval = 800;
			sensor_3rdstage_count = 0;
			sensor_3rdstage_effcount = 0;					
			}
		}
	
	// detect the horizontal sensor
	if(((horizontal_sensor == 0)||(sensor_detect == 0))&&(horizontal_vibration_count > 5000))
		{
		Delay(3);
		if((horizontal_sensor == 0)||(sensor_detect == 0))
			{
			horizontal_vibration = 1;
			horizontal_vibration_count = 0;
			}
		}
	if(++horizontal_vibration_count >= 5000)
		{
		horizontal_vibration_count = 5001;
		horizontal_vibration = 0;
		}
 	}


/*--------------------------------------------------------------------
	timerT1()
	定时器1每次溢出后执行的操作
--------------------------------------------------------------------*/

void timerT1() interrupt interrupt_timer_1_overflow
	{
   // reload timer1
	TH1 = timer1_8H;				
	TL1 = timer1_8L;
	
	// receive a tyte
	receive_byte();
	
	// receive a word after every byte
	receive_word();
	
	// judge whether a byte has received successfully
	if(receive_data_finished_flag == 1)
		{
		receive_data_finished_flag = 0;
		// judge which mode is received.
		switch(received_data_buffer[2]) 		
			{
			case ComMode_1:  
				{
				if((nearby_away_interval > 6)||(key_rotate == 0))
					{
					// if receive a ComMode_1 successfully, operation for slave nearby
					slave_nearby_EN = 1;
					nearby_away_interval = 0;					
					}
				leave_count = 0;

				
				// turn off vibration and position sensor
				sensor_EN = 0;							
				position_sensor_EN=0; 			
				// reset all the flag in sensor
				fell_flag=0;  
				raised_flag=0;
				sensor_trigger_count = 0;
				sensor_1ststage_count = 0;
				sensor_2ndstage_count = 0;
				sensor_2ndstage_time = 0;
				sensor_3rdstage_time = 0;
				sensor_3rdstage_interval = 0;
				sensor_3rdstage_count = 0;
				sensor_3rdstage_effcount = 0;
				stolen_alarm_flag = 0;
				host_stolen_alarm1_EN = 0;
				host_stolen_alarm1_count = 0;
				host_stolen_alarm2_EN = 0;
				host_stolen_alarm2_count = 0;
				
				raised_fell_flag = 0;
				
				// reset the wire broken count
				wire_broken_count = 0;
				
				// reset the battery stolen flag
				battery_stolen_EN = 0;
				battery_stolen_count = 0;
				}
			break;
			}
		}
	}

/*--------------------------------------------------
	InitTimer()
	
	初始化定时器T0和T1
---------------------------------------------------*/

void InitTimer(const tByte Tick_ms_T0, Tick_us_T1)
	{
	tLong Inc_T0, Inc_T1;
	tWord timer0_16, timer1_16;
	
	//计算Timer0的寄存器值
	Inc_T0 = (tLong)Tick_ms_T0 * (OSC_FREQ/1000) / (tLong)OSC_PER_INST;
	timer0_16 = (tWord) (65536UL - Inc_T0);
	timer0_8H = (tByte) (timer0_16 / 256);
	timer0_8L = (tByte) (timer0_16 % 256);
	
	//计算Timer1的寄存器值
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
	将MCU进入休眠状态
----------------------------------------------------------------------*/
void sEOS_Go_To_Sleep(void)
   {
   PCON |= 0x01;    // Enter idle mode (generic 8051 version)
   }
	
/*---------------------------------------------------
	end of file
----------------------------------------------------*/