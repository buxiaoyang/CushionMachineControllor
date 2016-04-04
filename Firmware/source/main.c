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

void getSensorStatus()
{
	/*
	//��ʼλ�ô�����1
	if(sensorStartPosi1 == 0)
	{
		motor1.isStartPosition = 1;
	}
	if(sensorStartPosi1 == 1)
	{
		motor1.isStartPosition = 0;
	}
	//��ʼλ�ô�����2
	if(sensorStartPosi2 == 0)
	{
		motor2.isStartPosition = 1;
	}
	if(sensorStartPosi2 == 1)
	{
		motor2.isStartPosition = 0;
	}
	*/
}

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
	timer_init();
	ChangeScreenPage(0x00);
	while(1)
	{
		if(displayMode >= 2 && displayMode <= 7)
		{
			 refreshDisplaySetting();
			 displayMode = 0;
		}
		if(saveSetting == 1) //��������ֵ
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
			saveSetting = 0;
		}
		/*
		delay_us(100);
		//����ɨ��
		count10ms++;
		if(count10ms > 29)
		{
			Key_Scan();
			count10ms = 0;
		}
		//General the PWM
		if(pulseSettingNumCount > 20)
		{
			timer_count --;
		}
		else
		{
			timer_count ++;
		}
		if(timer_count < 1)
		{
			timer_count = 1;	
		}
		else if(timer_count > 50)
		{
			timer_count = 50;
		}
		//Get the sensor status
		getSensorStatus();
		if(isStartPosition2 || isStartPosition1)
		{
			isStartPosition = 1;
		}
		else if(!isStartPosition2 && !isStartPosition1)
		{
			isStartPosition = 0;
		}
		//��ʼ���������ж�ԭ��
		if(initFlag == 1 && isStartPosition == 1)
		{
			pulseSettingNumCount = 4;
			currentPosition = 1;
			initFlag = 0;
		}
		//Save setting to eeprom
		if(saveSetting)
		{
			//��������ת��
			temp = (unsigned long)pulseSettingNum*motorStepAngle;
			motorRotationAngle = temp/motorReducGearRatio;
			//����˿�˵���
			temp = (unsigned long)pulseSettingNum*motorStepAngle;
			temp = 	(unsigned long)temp * screwPitch;
			temp = 	(unsigned long)temp / motorReducGearRatio;
			ballScrew = temp/3600;
			//��������
			ChangeScreenPage(0x04);
			parameter_save();
			ChangeScreenPage(0x02);
			saveSetting = 0;
			refreshDisplay = 1;
		}
		//ˢ����ʾ	
		if(refreshDisplay)
		{
			parameter_send_screen();
			refreshDisplay = 0;
		}
		*/
	}   

}








