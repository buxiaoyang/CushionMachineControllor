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
	delay_ms(500);
	parameter_init();
	uart_init();
	Timer0Init();
	Timer1Init();
	ChangeScreenPage(0x00);
	snapshot_init();
	PCON &= 0xDF ;//清LVDF位	 
	while(1)
	{
		//控制主循环执行时间为10ms
		if( motor1.status != MOTOR_STOP && motor2.status != MOTOR_STOP)
		{
			//两个电机一起运作
			delay_ms(4);
		}
		else if(motor1.status != MOTOR_STOP || motor2.status != MOTOR_STOP)
		{
			//只有一个电机运作
			delay_ms(6);
		}
		else
		{
			//没有电机运作
			delay_ms(10);
		}
		//输出测试信号
		//testOutput1 = !testOutput1;
		if (PCON & 0x20){
			PCON &= 0xDF;
			//testOutput1 = !testOutput1;
			snapshot_save();
		}
		//按键扫描
		Key_Scan1();
		Key_Scan2();
		//电机初始化状态机
		ResetMotorDispatch();
		//刷新显示器
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
		//保存设置值
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








