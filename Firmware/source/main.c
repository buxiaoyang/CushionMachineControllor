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
	delay_ms(2000);
	Init595();
	snapshot_init();
	parameter_init();
	uart_init();
	Timer0Init();
	Timer1Init();
	ChangeScreenPage(0x01);
	
	PCON &= 0xDF ;//��LVDFλ 
	while(1)
	{
		delay_ms(10);
		timeTick ++;
		if(timeTick > 500 && isNotificationUI == 1)
		{
		 	ChangeScreenPage(0x01);
			isNotificationUI = 0;
		}
		//���籣������
		if (PCON & 0x20){
			testOutput1 = !testOutput1;
			PCON &= 0xDF ;//��LVDFλ
			delay_ms(10);
			if(PCON & 0x20){
				testOutput1 = !testOutput1;
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
			KeyRunPress();
			saveMode = SAVE_NO_SAVING;
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








