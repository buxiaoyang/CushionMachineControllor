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
	delay_ms(10);
	snapshot_init();
	Init595();
	parameter_init();
	uart_init();
	Timer0Init();
	Timer1Init();
	ChangeScreenPage(0x01);
	PCON &= 0xDF ;//��LVDFλ 

	OutPut595(); //����Ĭ�����595

	while(1)
	{
//		testOutput1 = !testOutput1;
		delay_ms(10);
		timeTick ++;
		if(timeTick > 500 && isNotificationUI == 1)
		{
		 	ChangeScreenPage(0x01);
			isNotificationUI = 0;
		}
		//���籣������
		if (PCON & 0x20){	
			//ChangeScreenPage(0x05);
			PCON &= 0xDF ;//��LVDFλ
			//snapshot_save();
			//ChangeScreenPage(0x01);	
		}

		#ifndef RUN_MODE_SIMU
			if(sensorRun1 == 1 && sensorRun2 == 1)
			{
				 sensorRunCheck = 1;
			}
		#else
			if(sensorRun1 == 1 && sensorRun1 == 2 && sensorRunMotor1 == 1 && sensorRunMotor2 == 1)
			{
				 sensorRunCheck = 1;
			}
		#endif

		if(runMode == MODEL_RUN && sensorRunCheck == 1)
		{
			//����ɨ��
			Key_Scan1();
			#ifndef RUN_MODE_SIMU
				Key_Scan2();
			#endif
		}
		//�����ʼ��״̬��
		ResetMotorDispatch();

		//��������ֵ
		if(saveMode == SAVE_SETTING) 
		{
			ChangeScreenPage(0x05);
			parameter_save();
			saveMode = SAVE_NO_SAVING;
			ChangeScreenPage(0x03);
			//if(displayMode == DISPLAY_NO_FRESH)
			//{
				setting.currentStep ++;
				if(setting.currentStep > setting.totalSteps)
				{
					setting.currentStep = setting.totalSteps;
				}
				saveMode = SAVE_SETTING_STEP;
				displayMode = DISPLAY_RUN;
			//}
		}
		else if(saveMode == SAVE_SETTING_MOOD) 
		{
			//ChangeScreenPage(0x05);
			parameter_save_mood();
			parameter_read();
			saveMode = SAVE_NO_SAVING;
			//ChangeScreenPage(0x03);
		} 
		else if(saveMode == READ_STEP) 
		{
			//��������
			setting.currentStep ++;
			if(setting.currentStep > setting.totalSteps)
			{
				setting.currentStep = 1;
				productNum ++;
			}
			parameter_read();
			//�̵������
			#ifdef RUN_MODE_SIMU
		    	KeyRunPress();
			#else
				OutPut595();
			#endif
			snapshot_save();
			saveMode = SAVE_NO_SAVING;
		}
		else if(saveMode == READ_STEP_SIM) 
		{
			//��������
			/*
			setting.currentStep ++;
			if(setting.currentStep > setting.totalSteps)
			{
				setting.currentStep = 1;
				productNum ++;
			}
			*/
			parameter_read();
			//�̵������
			#ifndef RUN_MODE_SIMU
				OutPut595();
			#endif
			KeyRunPress();
			//snapshot_save();
			saveMode = SAVE_NO_SAVING;
		}
		else if(saveMode == SAVE_SNAPSHOT)
		{
			ChangeScreenPage(0x05);
			snapshot_save();
			saveMode = SAVE_NO_SAVING;
			ChangeScreenPage(0x01);
		}
		if(saveMode == SAVE_SETTING_STEP) 
		{
			//ChangeScreenPage(0x05);
			parameter_read();
			saveMode = SAVE_NO_SAVING;
			//ChangeScreenPage(0x03);
		}

		//ˢ����ʾ��
		if(displayMode == DISPLAY_RUN)
		{
			 refreshDisplay();
			 displayMode = DISPLAY_NO_FRESH;
		}
		else if(displayMode == DISPLAY_MAX_POSITION)
		{
			ChangeScreenPage(0x08);
			timeTick = 0;
			isNotificationUI = 1;
			displayMode = DISPLAY_NO_FRESH;
		}
		else if(displayMode == DIAPLAY_MIN_POSITION)
		{
			ChangeScreenPage(0x07);
			timeTick = 0;
			isNotificationUI = 1;
			displayMode = DISPLAY_NO_FRESH;
		}
		
		
	}   

}








