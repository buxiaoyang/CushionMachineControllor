/***************************************************************************/
// ����15�����������ϵͳ
// ģ�飺������	
// �ļ���main.c
// ���ߣ������D
// �汾��V0.01
// ���ڣ�2016��4��3��
// ���ܣ�������
// оƬ��STC12C5A60S2
// ���룺Keil uVision3 V3.90
/***************************************************************************/

#include <reg52.h>
#include <basefunc.h>
#include <parameter.h>
#include <uart.h>
#include <timer.h>
#include <key.h>
#include <dispatch.h>

/***************************************************************************/
// ������
// ��������
// ����ֵ����	
/***************************************************************************/
void main()
{
	unsigned int timeTick = 0;
	unsigned char isNotificationUI = 0; 
	delay_ms(500);
	parameter_init();
	uart_init();
	Timer0Init();
	Timer1Init();
	ChangeScreenPage(0x01);
	snapshot_init();
	PCON &= 0xDF ;//��LVDFλ	 
	while(1)
	{
		timeTick ++;
		if(timeTick > 50 && isNotificationUI == 1)
		{
		 	ChangeScreenPage(0x01);
			isNotificationUI = 0;
		}
		//������ѭ��ִ��ʱ��Ϊ10ms
		if( motor1.status != MOTOR_STOP && motor2.status != MOTOR_STOP)
		{
			//�������һ������
			delay_ms(4);
		}
		else if(motor1.status != MOTOR_STOP || motor2.status != MOTOR_STOP)
		{
			//ֻ��һ���������
			delay_ms(6);
		}
		else
		{
			//û�е������
			delay_ms(10);
		}
		//���籣������
		if (PCON & 0x20){
			//testOutput1 = !testOutput1;
			PCON &= 0xDF ;//��LVDFλ
			delay_ms(10);
			if(PCON & 0x20){
				//testOutput1 = !testOutput1;
				MotorGroup1Stop();
				MotorGroup2Stop();
				PCON &= 0xDF ;//��LVDFλ
				snapshot_save();
				if(PCON & 0x20){
					while(1) //ȷʵ�����ˣ��ȴ��ػ�
					{
						testOutput1 = !testOutput1;
					};
				}
				else
				{
					MotorGroup1Start();
					MotorGroup2Start();
				}
			}		
		}
		if(runMode == MODEL_RUN)
		{
			//����ɨ��
			Key_Scan1();
		}
		//�����ʼ��״̬��
		ResetMotorDispatch();
		//ˢ����ʾ��
		if(displayMode == DISPLAY_RUN)
		{
			 refreshDisplayRunning();
			 displayMode = DISPLAY_NO_FRESH;
		}
		else if(displayMode == DISPLAY_MAX_POSITION)
		{
			ChangeScreenPage(0x06);
			timeTick = 0;
			isNotificationUI = 1;
			displayMode = DISPLAY_NO_FRESH;
		}
		else if(displayMode == DIAPLAY_MIN_POSITION)
		{
			ChangeScreenPage(0x05);
			timeTick = 0;
			isNotificationUI = 1;
			displayMode = DISPLAY_NO_FRESH;
		}

		
		//��������ֵ
		/*
		if(saveMode == SAVE_SETTING) 
		{
			ChangeScreenPage(0x04);
			if(!parameter_save())
			{
				//ERROR
				ChangeScreenPage(0x02);
			}
			else
			{
				//succeed
				ChangeScreenPage(0x02);
			}
			saveMode = SAVE_NO_SAVING;
		}
		*/
	}   

}








