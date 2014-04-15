/*----------------------------------------------------
	port.h (v1.00)

	'port header' (see chap 5) for DDC-Z
----------------------------------------------------*/

#ifndef _PORT_H
#define _PORT_H

#include "main.h"

// P2.4��PIN15�����ӵ�Կ��ת����Կ��ת��ʱ����pinΪ1��Կ�׹ر�ʱ����pinΪ0
sbit key_rotate = P2^4;			

// P2.6��PIN27��������Ĺ��ʿ��ơ�1��ʾС���ʣ�0��ʾ���ʡ����ʷ������Զ��
sbit transmiter_power = P2^6;

// P2.7��PIN28�������ʹ�ܶ˿ڡ�0��ʾ����1��ʾ��
sbit transmiter_EN = P2^7;

// P0.0, PIN3, horizontal sensor input
sbit sensor_detect = P0^0;

// P2.5��PIN16����������ʹ�ܿ��ƣ�1��ʾ�򿪣�0��ʾ�ر�
sbit sensor_EN=P2^5;

// P2.2��P2.3���ֱ�ΪPIN13��PIN14�������,  ƽʱֵΪ00��������10��������01��Ȼ�󶼻ָ���00
sbit MagentControl_1 = P2^2;
sbit MagentControl_2 = P2^3;

// P0.4��PIN23��̧�𴫸����ļ��
sbit raised_sensor_detect = P0^4;

// P0.3��PIN24�����ش������ļ��
sbit fell_sensor_detect = P0^3;

//P1.4��PIN10�����ſ��ؿ��ƣ�1Ϊ�򿪹��ţ�0Ϊ�رչ���
sbit voice_EN = P1^4;

// P2.0��P2.1�ֱ�ΪPIN1��PIN2����Ӧ����оƬ�ĸ�λ�����ݹܽ�
sbit SC_RST=P2^0;
sbit SC_DATA=P2^1;

// P0.7��PIN19���������ļ������ͨ����PIN����MCU��Ȼ��MCU�ж�
sbit horizontal_sensor = P0^7; 

// P1.3, PIN11, receiver enable��0�Ǵ򿪣�1�ǹر�
sbit receiver_EN = P1^3;

// P0.5, PIN22, �ⲿ�綯����������ź��������ֹ����û��ȫ�򿪵�ʱ�򣬳��ּӵ絼�����������ĸ�����ײ��0�Ǵ��ⲿ���ӣ�1�ǹر��ⲿ���� 
sbit motor_lock = P0^5;

// P0.2, PIN25, �����״̬��������
sbit magnet_status = P0^2;

// P0.1, PIN26, ���߱������ܽţ���̬Ϊ1����Ϊ0ʱ�����ʾ���˼��ߡ�
sbit wire_broken = P0^1;

// P1.2, PIN12, ������������
sbit magnet_overcurrent = P1^2;

#endif
											
/*---------------------------------------------------
	end of file
----------------------------------------------------*/