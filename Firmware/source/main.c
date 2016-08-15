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

/***************************************************************************/
// 主函数
// 参数：无
// 返回值：无	
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
	PCON &= 0xDF ;//清LVDF位 
	while(1)
	{
		delay_ms(5);
		timeTick ++;
		if(timeTick > 500 && isNotificationUI == 1)
		{
		 	ChangeScreenPage(0x01);
			isNotificationUI = 0;
		}
		//掉电保存数据
		if (PCON & 0x20){
			//testOutput1 = !testOutput1;
			PCON &= 0xDF ;//清LVDF位
			delay_ms(10);
			if(PCON & 0x20){
				//testOutput1 = !testOutput1;
				MotorGroup1Stop();
				MotorGroup2Stop();
				PCON &= 0xDF ;//清LVDF位
				snapshot_save();
				if(PCON & 0x20){
					while(1) //确实掉电了，等待关机
					{
						testOutput1 = !testOutput1;
					};
				}
				else
				{
					//MotorGroup1Start();
					//MotorGroup2Start();
				}
			}		
		}

		if(runMode == MODEL_RUN)
		{
			//按键扫描
			Key_Scan1();
		}


		//保存设置值
		if(saveMode == SAVE_SETTING) 
		{
			//ChangeScreenPage(0x05);
			parameter_save();
			saveMode = SAVE_NO_SAVING;
			//ChangeScreenPage(0x03);
		}
		else if(saveMode == SAVE_SETTING_MOOD) 
		{
			//ChangeScreenPage(0x05);
			parameter_save_mood();
			parameter_read();
			saveMode = SAVE_NO_SAVING;
			//ChangeScreenPage(0x03);
		}
		else if(saveMode == SAVE_SETTING_STEP) 
		{
			//ChangeScreenPage(0x05);
			parameter_read();
			saveMode = SAVE_NO_SAVING;
			//ChangeScreenPage(0x03);
		}

		//刷新显示器
		if(displayMode == DISPLAY_RUN)
		{
			 refreshDisplay();
			 displayMode = DISPLAY_NO_FRESH;
		}

		//电机初始化状态机
		//ResetMotorDispatch();

		

		/*
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
		*/
		
		
	}   

}








