#include <reg52.h>
#include <intrins.h>
#include <parameter.h>
#include <key.h>
#include <dispatch.h>
#include <timer.h>


void Key_Scan1(void); //接近开关送信号X2 按键扫描
void Key_Scan2(void); //接近开关送信号X2 按键扫描

unsigned char Key_Scan_Steps1 = 0; 		//0：初始状态 如果有按键按下则进入1
										//1：延迟10ms
										//2：如果按键确实按下则进入3
										//3：确定按键
										//4：如果按键释放则进入0



unsigned char Key_Scan_Steps2 = 0; 		//0：初始状态 如果有按键按下则进入1
										//1：延迟10ms
										//2：如果按键确实按下则进入3
										//3：确定按键
										//4：如果按键释放则进入0

void Key_Scan1(void)  //运行接近开关控制
{
	switch(Key_Scan_Steps1)
	{
		case 0:
			if(sensorRun1 == 0 || sensorRun2 == 0)
			{
			   	Key_Scan_Steps1 = 1;
			}
		break;
		case 1:
			if(sensorRun1 == 0 || sensorRun2 == 0)
			{
			   	Key_Scan_Steps1 = 2;
			}
			else
			{
				Key_Scan_Steps1 = 0;
			}
		break;
		case 2:
			if(sensorRun1 == 0)
			{
				saveMode = READ_STEP;
				displayMode = DISPLAY_RUN;
			}
			else if(sensorRun2 == 0)
			{
				saveMode = READ_STEP;
				displayMode = DISPLAY_RUN;
			}
			Key_Scan_Steps1 = 3;
		break;
		case 3:
			if(sensorRun1 == 1 && sensorRun2 == 1)
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
			if(powerOffSave == 0)
			{
			   	Key_Scan_Steps2 = 1;
			}
		break;
		case 1:
			if(powerOffSave == 0)
			{
			   	Key_Scan_Steps2 = 2;
			}
			else
			{
				Key_Scan_Steps2 = 0;
			}
		break;
		case 2:
			if(powerOffSave == 0)
			{
				saveMode = SAVE_SNAPSHOT;
			}
			Key_Scan_Steps2 = 3;
		break;
		case 3:
			if(powerOffSave == 1)
			{
			   	Key_Scan_Steps2 = 0;
			}
		break;
		default:
			 _nop_();
	}
}

void KeyRunPress(void)
{
	//继电器输出
	OutPut595();
	//电机运动
	if(motor1.status == MOTOR_STOP && motor2.status == MOTOR_STOP && motorEnable == 0)
	{
		if(setting.motor1Steps != 0)
		{
			 if(setting.motor1Steps > 0x7FFF) //反向
			 {
				ioMotor1Direction = MOTOR_BACKWARD;
				motor1.status = MOTOR_BACKWARD;
				motor1.stepPWMs = 0xFFFF - setting.motor1Steps + 1;
			 }
			 else //正向
			 {
				ioMotor1Direction = MOTOR_FORWARD;
				motor1.status = MOTOR_FORWARD;
				motor1.stepPWMs = setting.motor1Steps;
			 }
			 motor1.stepPassPWMs = 0;
			 MotorGroup1Start();
		}
		else if(setting.motor2Steps != 0)
		{
			if(setting.motor2Steps > 0x7FFF) //反向
			 {
				ioMotor2Direction = MOTOR_BACKWARD;
				motor2.status = MOTOR_BACKWARD;
				motor2.stepPWMs = 0xFFFF - setting.motor2Steps + 1;
			 }
			 else //正向
			 {
				ioMotor2Direction = MOTOR_FORWARD;
				motor2.status = MOTOR_FORWARD;
				motor2.stepPWMs = setting.motor2Steps;
			 }
			 motor2.stepPassPWMs = 0;
			 MotorGroup1Start();
		}
	}

	if(motor3.status == MOTOR_STOP && motor4.status == MOTOR_STOP && motorEnable == 0)
	{
		if(setting.motor3Steps != 0)
		{
			 if(setting.motor3Steps > 0x7FFF) //反向
			 {
				ioMotor3Direction = MOTOR_BACKWARD;
				motor3.status = MOTOR_BACKWARD;
				motor3.stepPWMs = 0xFFFF - setting.motor3Steps + 1;
			 }
			 else //正向
			 {
				ioMotor3Direction = MOTOR_FORWARD;
				motor3.status = MOTOR_FORWARD;
				motor3.stepPWMs = setting.motor3Steps;
			 }
			 motor3.stepPassPWMs = 0;
			 MotorGroup2Start();
		}
		else if(setting.motor4Steps != 0)
		{
			if(setting.motor4Steps > 0x7FFF) //反向
			 {
				ioMotor4Direction = MOTOR_BACKWARD;
				motor4.status = MOTOR_BACKWARD;
				motor4.stepPWMs = 0xFFFF - setting.motor4Steps + 1;
			 }
			 else //正向
			 {
				ioMotor4Direction = MOTOR_FORWARD;
				motor4.status = MOTOR_FORWARD;
				motor4.stepPWMs = setting.motor4Steps;
			 }
			 motor4.stepPassPWMs = 0;
			 MotorGroup2Start();
		}
	}
}