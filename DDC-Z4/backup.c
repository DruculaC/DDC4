//backup

if(detect_stolen_flag==0)
		{
			if(ReceWave==1)
			{
				time0Count_2++;
				if(time0Count_2==10)
				{
					time0Count_2=20;
					SensorCount=1;
					alarmFlag2=1;
					detect_stolen_flag=1;
					detect_stolen_count=0;
				}
			}
		}
		else if(detect_stolen_flag==1)
		{
			detect_stolen_count++;
			if((detect_stolen_count>=20)&&(detect_stolen_count<40))
			{
				if(ReceWave==1)
				{
					time0Count_6++;
					if(time0Count_6==10)
					{
						SensorCount=2;
						alarmFlag2=1;
					}
				}
			}
			else if((detect_stolen_count>=240)&&(detect_stolen_count<360))
			{
				if(ReceWave==1)
				{
					time0Count_7++;
					if(time0Count_7==10)
					{
						SensorCount=3;
						alarmFlag2=1;
					}
				}	
			}
			else
			{
				time0Count_2=0;
				time0Count_6=0;
				time0Count_7=0;
				SensorCount=0;
				alarmFlag2=0;
				detect_stolen_count=0;
				detect_stolen_flag=0;
				speech1_count=0;
				speech2_count=0;
			}
		}




if(ModeFlag==2&&SensorCount>=1)//���ᴫ�����������Ӧ����
	{
		if(SensorCount==1&&alarmFlag2==1)//���ᴫ����һ�δ���,alarmFlag2���Ʒ���1��
		{					
			if((upSignal!=0)&&(downSignal!=0))
			{
//				sendspeech1=1;
				alarmFlag2=0;

				PAshutdown=1;
				SC_Speech(17);  //�ػ���������
				Delay(80);
				PAshutdown=0;
			}
		}

		if((SensorCount==2)&&(alarmFlag2==1))
		{
			if((upSignal!=0)&&(downSignal!=0))
			{
				sendspeech2=1;
				alarmFlag2=0;

				PAshutdown=1;
				SC_Speech(18);  //�ػ���������
				Delay(80);
				SC_Speech(16);  //�ػ���������
				Delay(80);
				PAshutdown=0;
			}			
		}

		if(SensorCount>=3)
		{
			sendspeech3=1;
			alarmFlag2=0;
			stolenflag=1;
			alarmFlag=1;//����������λ
			alarmFlag2=0;
			downFlag=0;
			upFlag=0;
			SensorCount=0; //�����������
		}
/*
		if(SensorCount>=4)//���ᴫ����һ�δ���
		{
			ModeFlag=3;//���ᴫ�����Ѿ���2�δ����ˣ�Ҫ�ı䷢��ģʽ��
			sendspeech4=1;
			stolenflag=1;

			alarmFlag=1;//����������λ
			alarmFlag2=0;
			downFlag=0;
			upFlag=0;
			SensorCount=0; //�����������
		}
*/
	}




/*
	if(ModeFlag==3)
	{



		alarmCount++;
		if(alarmCount>=2000) //���������Ķ���
		{
			alarmCount=0;//�屨��������
			alarmFlag=0;//�屨����־
		}
	}
*/