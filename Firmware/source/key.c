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
			if(keyMotor1Forward == 0 && motor1.status == MOTOR_STOP) //电机1前进
			{
				 if(motor1.currentStage % 2 == 0) //在前进过程中
				 {
				 	 ioMotor1Direction = MOTOR_FORWARD;
					 motor1.status = MOTOR_FORWARD;
					 motor1.isStartPosition = 0;
					 motor1.stepPWMs = motorRotationAngle[motor1.currentStage][motor1.position] << 1;
					 motor1.stepPassPWMs = 0;
					 motor1.totalPWMs += motorRotationAngle[motor1.currentStage][motor1.position];
					 if(motorRotationAngle[motor1.currentStage][motor1.position + 1] == 0) //下面没有步数了
					 {
					 	motor1.currentStage ++;
						motor1.position = 0;

					 }
					 else //下面还有步数
					 {
					 	 motor1.position ++;
					 }
					 Motor1Start();
				 }
				 else  //不在前进过程中
				 {
				 	displayMode = DISPLAY_MAX_POSITION;
				 }
			}
			else if(keyMotor1Backward == 0 && motor1.status == MOTOR_STOP) //电机1后退
			{
				if(motor1.currentStage % 2 == 1) //在后退过程中
				{
					ioMotor1Direction = MOTOR_BACKWARD;
					motor1.status = MOTOR_BACKWARD;
					motor1.stepPWMs = motorRotationAngle[motor1.currentStage][motor1.position] << 1;
					motor1.stepPassPWMs = 0;
					motor1.totalPWMs -= motorRotationAngle[motor1.currentStage][motor1.position];
					if(motorRotationAngle[motor1.currentStage][motor1.position + 1] == 0) //下面没有步数了
					{
						motor1.isStartPosition = 1;
						motor1.position = 0;
						motor1.currentStage ++;
						if(motor1.currentStage > 5)  //三个来回走完
						{
							motor1.currentStage = 0;
						}
						else if(motorRotationAngle[motor1.currentStage][motor1.position] == 0) //下一个过程没有配置
						{
							motor1.currentStage = 0;
						}
					}
					else //下面还有步数
					{
						motor1.position++;
					}				
					Motor1Start();
				}
				else  //不在后退过程中
				{
					displayMode = DIAPLAY_MIN_POSITION;
				}
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
			if(keyMotor2Forward == 0 && motor2.status == MOTOR_STOP) //电机2前进
			{
				if(motor2.currentStage % 2 == 0) //在前进过程中
				 {
				 	 ioMotor2Direction = MOTOR_FORWARD;
					 motor2.status = MOTOR_FORWARD;
					 motor2.isStartPosition = 0;
					 motor2.stepPWMs = motorRotationAngle[motor2.currentStage][motor2.position] << 1;
					 motor2.stepPassPWMs = 0;
					 motor2.totalPWMs += motorRotationAngle[motor2.currentStage][motor2.position];
					 if(motorRotationAngle[motor2.currentStage][motor2.position + 1] == 0) //下面没有步数了
					 {
					 	motor2.currentStage ++;
						motor2.position = 0;

					 }
					 else //下面还有步数
					 {
					 	 motor2.position ++;
					 }
					 Motor2Start();
				 }
				 else  //不在前进过程中
				 {
				 	displayMode = DISPLAY_MAX_POSITION;
				 }
			}
			else if(keyMotor2Backward == 0 && motor2.status == MOTOR_STOP) //电机2后退
			{
				if(motor2.currentStage % 2 == 1) //在后退过程中
				{
					ioMotor2Direction = MOTOR_BACKWARD;
					motor2.status = MOTOR_BACKWARD;
					motor2.stepPWMs = motorRotationAngle[motor2.currentStage][motor2.position] << 1;
					motor2.stepPassPWMs = 0;
					motor2.totalPWMs -= motorRotationAngle[motor2.currentStage][motor2.position];
					if(motorRotationAngle[motor2.currentStage][motor2.position + 1] == 0) //下面没有步数了
					{
						motor2.isStartPosition = 1;
						motor2.position = 0;
						motor2.currentStage ++;
						if(motor2.currentStage > 5)  //三个来回走完
						{
							motor2.currentStage = 0;
						}
						else if(motorRotationAngle[motor2.currentStage][motor2.position] == 0) //下一个过程没有配置
						{
							motor2.currentStage = 0;
						}
					}
					else //下面还有步数
					{
						motor2.position++;
					}				
					Motor2Start();
				}
				else  //不在后退过程中
				{
					displayMode = DIAPLAY_MIN_POSITION;
				}
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