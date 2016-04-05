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
	delay_ms(500);
	parameter_init();
	uart_init();
	Timer0Init();
	Timer1Init();
	ChangeScreenPage(0x00);	 
	while(1)
	{
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
		//��������ź�
		//testOutput1 = !testOutput1;
		//����ɨ��
		Key_Scan1();
		Key_Scan2();
		//�����ʼ��״̬��
		ResetMotorDispatch();
		//ˢ����ʾ��
		if(displayMode == DISPLAY_RUN)
		{
			 refreshDisplayRunning();
			 displayMode = DISPLAY_NO_FRESH;
		}
		else if(displayMode >= DISPLAY_SETTING1 && displayMode <= DISPLAY_SETTING6)
		{
			 refreshDisplaySetting();
			 displayMode = DISPLAY_NO_FRESH;
		}
		//��������ֵ
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
	}   

}








