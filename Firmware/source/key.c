#include <reg52.h>
#include <intrins.h>
#include <parameter.h>
#include <key.h>
#include <dispatch.h>
#include <timer.h>


void Key_Scan1(void); //电机1按键扫描

unsigned char Key_Scan_Steps1 = 0; 		//0：初始状态 如果有按键按下则进入1
										//1：延迟10ms
										//2：如果按键确实按下则进入3
										//3：确定按键
										//4：如果按键释放则进入0

void Key_Scan2(void); //电机2按键扫描

unsigned char Key_Scan_Steps2 = 0; 		//0：初始状态 如果有按键按下则进入1
										//1：延迟10ms
										//2：如果按键确实按下则进入3
										//3：确定按键
										//4：如果按键释放则进入0

void Key_Scan1(void)
{
	switch(Key_Scan_Steps1)
	{
		case 0:
			if(keyMotor1Forward == 0 || keyMotor1Backward == 0)
			{
			   	Key_Scan_Steps1 = 1;
			}
		break;
		case 1:
			if(keyMotor1Forward == 0 || keyMotor1Backward == 0)
			{
			   	Key_Scan_Steps1 = 2;
			}
			else
			{
				Key_Scan_Steps1 = 0;
			}
		break;
		case 2:
			if(keyMotor1Forward == 0) //电机1前进
			{
				motor1Forward();
			}
			else if(keyMotor1Backward == 0) //电机1后退
			{
				motor1Backward();
			}
			displayMode = DISPLAY_RUN;
			Key_Scan_Steps1 = 3;
		break;
		case 3:
			if(keyMotor1Forward == 1 && keyMotor1Backward == 1)
			{
			   	Key_Scan_Steps1 = 0;
			}
		break;
		default:
			 _nop_();
	}
}

void Key_Scan2(void)
{
	switch(Key_Scan_Steps2)
	{
		case 0:
			if(keyMotor2Forward == 0 || keyMotor2Backward == 0)
			{
			   	Key_Scan_Steps2 = 1;
			}
		break;
		case 1:
			if(keyMotor2Forward == 0 || keyMotor2Backward == 0)
			{
			   	Key_Scan_Steps2 = 2;
			}
			else
			{
				Key_Scan_Steps2 = 0;
			}
		break;
		case 2:
			if(keyMotor2Forward == 0) //电机2前进
			{
				motor2Forward();
			}
			else if(keyMotor2Backward == 0) //电机2后退
			{
				motor2Backward();
			}
			displayMode = DISPLAY_RUN;
			Key_Scan_Steps2 = 3;
		break;
		case 3:
			if(keyMotor2Forward == 1 && keyMotor2Backward == 1)
			{
			   	Key_Scan_Steps2 = 0;
			}
		break;
		default:
			 _nop_();
	}
}