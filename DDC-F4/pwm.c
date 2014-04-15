/*---------------------------------------------------
	pwm.c (v1.00)
	
	�����������
---------------------------------------------------*/

#include "main.h"
#include "port.h"

#include "pwm.h"

tWord PWM_shadow;

/*---------------------------------------------------
	Init_PWM()
	
	��ʼ�������������
----------------------------------------------------*/

void Init_PWM(void)
	{    
    PWMCON0 = 0xC0;	// Enable PWM and select auto reload mode    
    PWM_shadow = PWMP;
    PWMPH = HIBYTE(PWM_shadow);	// PWM Period
    PWMPL = LOBYTE(PWM_shadow);                               
	}

/*---------------------------------------------------
	PWM_Channel()
	
	ѡ��PWM���������ͨ��
----------------------------------------------------*/

void PWM_Channel(E_PWMCNL_SEL Channel)	 //ѡ��PWM���ͨ��
	{
    switch(Channel)
		{
        case E_CHANNEL0:
            PWM_shadow=PWM0_Duty;
            PWM0H = HIBYTE(PWM_shadow);// PWM0 Duty (P0.1)
            PWM0L = LOBYTE(PWM_shadow);
            break;
        case E_CHANNEL1:
            PWM_shadow=PWM1_Duty;
            PWM1H = HIBYTE(PWM_shadow);// PWM1 Duty (P1.6)
            PWM1L = LOBYTE(PWM_shadow);
            break;
        case E_CHANNEL2:
            PWM_shadow=PWM2_Duty;
            PWM2H = HIBYTE(PWM_shadow);// PWM2 Duty (P1.7)
            PWM2L = LOBYTE(PWM_shadow);
            break;
        case E_CHANNEL3:
            PWM_shadow=PWM3_Duty;
            PWM3H = HIBYTE(PWM_shadow);// PWM3 Duty (P0.0)
            PWM3L = LOBYTE(PWM_shadow);
            break; 
		}
	}

/*-------------------------------------------------------
	myPwm()
	
	���������
--------------------------------------------------------*/

void myPwm()
	{
	PWM_Channel(E_CHANNEL0);           // Select PWM channel.
   Init_PWM();                        // Enable PWM function and set PWM period.  
	}

/*---------------------------------------------------
	end of file
----------------------------------------------------*/    



