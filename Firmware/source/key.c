#include <reg52.h>
#include <intrins.h>
#include <parameter.h>
#include <key.h>
#include <dispatch.h>
#include <timer.h>


void Key_Scan1(void); //电机1,2按键扫描

unsigned char Key_Scan_Steps1 = 0; 		//0：初始状态 如果有按键按下则进入1
										//1：延迟10ms
										//2：如果按键确实按下则进入3
										//3：确定按键
										//4：如果按键释放则进入0

void Key_Scan2(void); //电机3,4按键扫描

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
			if(keyMotor1Forward == 0 || keyMotor1Backward == 0 || keyMotor2Copy == 0)
			{
			   	Key_Scan_Steps1 = 1;
			}
		break;
		case 1:
			if(keyMotor1Forward == 0 || keyMotor1Backward == 0 || keyMotor2Copy == 0)
			{
			   	Key_Scan_Steps1 = 2;
			}
			else
			{
				Key_Scan_Steps1 = 0;
			}
		break;
		case 2:
			if(keyMotor1Forward == 0)
			{
				motor1Forward();
			}
			else if(keyMotor1Backward == 0)
			{
				motor1Backward();
			}
			else if(keyMotor2Copy == 0)
			{
				motor2Copy();
			}
			Key_Scan_Steps1 = 3;
		break;
		case 3:
			if(keyMotor1Forward == 1 && keyMotor1Backward == 1 && keyMotor2Copy == 1)
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
			if(keyMotor3Forward == 0 || keyMotor3Backward == 0 || keyMotor4Copy == 0)
			{
			   	Key_Scan_Steps2 = 1;
			}
		break;
		case 1:
			if(keyMotor3Forward == 0 || keyMotor3Backward == 0 || keyMotor4Copy == 0)
			{
			   	Key_Scan_Steps2 = 2;
			}
			else
			{
				Key_Scan_Steps2 = 0;
			}
		break;
		case 2:
			if(keyMotor3Forward == 0)
			{
				motor3Forward();
			}
			else if(keyMotor3Backward == 0)
			{
				motor3Backward();
			}
			else if(keyMotor4Copy == 0)
			{
				motor4Copy();
			}
			Key_Scan_Steps2 = 3;
		break;
		case 3:
			if(keyMotor3Forward == 1 && keyMotor3Backward == 1 && keyMotor4Copy == 1)
			{
			   	Key_Scan_Steps2 = 0;
			}
		break;
		default:
			 _nop_();
	}
}