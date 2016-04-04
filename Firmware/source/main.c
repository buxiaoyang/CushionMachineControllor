/***************************************************************************/
// 程序：15型座垫机控制系统
// 模块：主函数	
// 文件：main.c
// 作者：卜晓旸
// 版本：V0.01
// 日期：2016年4月3日
// 功能：主函数
// 芯片：STC12C5A60S2
// 编译：Keil uVision3 V3.90
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
	//初始位置传感器1
	if(sensorStartPosi1 == 0)
	{
		motor1.isStartPosition = 1;
	}
	if(sensorStartPosi1 == 1)
	{
		motor1.isStartPosition = 0;
	}
	//初始位置传感器2
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
// 主函数
// 参数：无
// 返回值：无	
/***************************************************************************/
void main()
{
	delay_ms(500);
	parameter_init();
	uart_init();
	Timer0Init();
	Timer1Init();
	ChangeScreenPage(0x00);

	/*
	motor1.stepPWMs = 32000;
	motor1.stepPassPWMs = 0;
	Motor1Start();

	motor2.stepPWMs = 3200;
	motor2.stepPassPWMs = 0;
	Motor2Start();
	*/

	while(1)
	{
		if(displayMode == DISPLAY_RUN)
		{
			 refreshDisplayRunning();
		}
		else if(displayMode >= DISPLAY_SETTING1 && displayMode <= DISPLAY_SETTING6)
		{
			 refreshDisplaySetting();
			 displayMode = DISPLAY_NO_FRESH;
		}
		if(saveMode == SAVE_SETTING) //保存设置值
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
		/*
		delay_us(100);
		//按键扫描
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
		//初始化过程中判断原点
		if(initFlag == 1 && isStartPosition == 1)
		{
			pulseSettingNumCount = 4;
			currentPosition = 1;
			initFlag = 0;
		}
		//Save setting to eeprom
		if(saveSetting)
		{
			//计算电机旋转角
			temp = (unsigned long)pulseSettingNum*motorStepAngle;
			motorRotationAngle = temp/motorReducGearRatio;
			//计算丝杆导程
			temp = (unsigned long)pulseSettingNum*motorStepAngle;
			temp = 	(unsigned long)temp * screwPitch;
			temp = 	(unsigned long)temp / motorReducGearRatio;
			ballScrew = temp/3600;
			//保存设置
			ChangeScreenPage(0x04);
			parameter_save();
			ChangeScreenPage(0x02);
			saveSetting = 0;
			refreshDisplay = 1;
		}
		//刷新显示	
		if(refreshDisplay)
		{
			parameter_send_screen();
			refreshDisplay = 0;
		}
		*/
	}   

}








