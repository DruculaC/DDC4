/*---------------------------------------------------
	Other.c (v1.00)
	
	��������
---------------------------------------------------*/

#include "main.h"
#include "port.h"

#include "Other.h"
#include "Delay.h"

/*-------------------------------------------------------
	magnet_CW()
	�������ת��˳ʱ�룬������
--------------------------------------------------------*/
void magnet_CW(void)
	{
	MagentControl_1 = 0;
	MagentControl_2 = 1;
	magnet_delay();
	MagentControl_1 = 1;
	MagentControl_2 = 1;
	motor_lock = 0;
	}

/*-------------------------------------------------------
	magnet_ACW()
	�������ת����ʱ�룬�����ر�
--------------------------------------------------------*/
void magnet_ACW(void)
	{
	motor_lock = 1;
	MagentControl_1 = 1;
	MagentControl_2 = 0;
	magnet_delay();
	MagentControl_1 = 1;
	MagentControl_2 = 1;
	}
/*---------------------------------------------------
	end of file
----------------------------------------------------*/