/*-----------------------------------------------------
	T1.h (v1.00)
	
	header file for T1.c
-----------------------------------------------------*/

#ifndef _T1_H
#define _T1_H

//-----public constants-------------------------------

#define timer1H 0xff  //设置定时器的时间，公式65536-timer*FOSC/12，然后来进行填充
#define timer1L 0x8e  //现在这个设置在13.5M的晶振下为100us

//-----public function prototype----------------------

void InitT1();	//初始化定时器01

#endif
											
/*---------------------------------------------------
	end of file
----------------------------------------------------*/


