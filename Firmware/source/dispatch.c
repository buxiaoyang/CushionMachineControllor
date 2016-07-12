#include <reg52.h>
#include <parameter.h>
#include <intrins.h>
#include <timer.h>
#include <dispatch.h>


unsigned char ResetMotorDispatchSteps = 20; 


unsigned long xdata lastStepPWMs1; //单步总脉冲数1
unsigned char xdata lastStatus1; //电机方向1

unsigned long xdata lastStepPWMs2; //单步总脉冲数1
unsigned char xdata lastStatus2; //电机方向1

//电机回零状态机，每10毫秒调用一次，调用之前确保电机处于空闲状态。在ResetMotorDispatchSteps == 20 时设置ResetMotorDispatchSteps = 0 开始状态机
void ResetMotorDispatch(void)
{
	switch(ResetMotorDispatchSteps)
	{
		case 0:
			if( motor1.status == MOTOR_STOP && 
				motor2.status == MOTOR_STOP &&
				motor3.status == MOTOR_STOP &&
				motor4.status == MOTOR_STOP && 
				motor1.position == motor2.position && 
				motor3.position == motor4.position &&
				motor1.currentStage == motor2.currentStage &&
				motor3.currentStage == motor4.currentStage) //必须在二号，四号电机走完之后才启动回零
			{
				//启动电机1
				ioMotor1Direction = MOTOR_BACKWARD;
			   	motor1.status = MOTOR_BACKWARD;
				motor1.stepPWMs = 3000000;
				motor1.stepPassPWMs = 0;
				MotorGroup1Start();

				ResetMotorDispatchSteps ++;
			}
		break;
		case 1:
			if(sensorStartPosi1 == 0)
			{
				//关闭电机1
			   	MotorGroup1Stop();
				motor1.position = 0;
				motor1.status = MOTOR_STOP;
				motor1.isStartPosition = 1;
				motor1.stepPWMs = 0;
				motor1.stepPassPWMs = 0;
				motor1.totalPWMs = 0;
				motor1.currentStage = 0; 
				//启动电机2
				ioMotor2Direction = MOTOR_BACKWARD;
				motor2.status = MOTOR_BACKWARD;
				motor2.stepPWMs = 3000000;
				motor2.stepPassPWMs = 0;
				MotorGroup1Start();

				displayMode = DISPLAY_RUN;
				ResetMotorDispatchSteps ++;
			}
		break;
		case 2:
			if(sensorStartPosi2 == 0)
			{
				//关闭电机2
			   	MotorGroup1Stop();
				motor2.position = 0;
				motor2.status = MOTOR_STOP;
				motor2.isStartPosition = 1;
				motor2.stepPWMs = 0;
				motor2.stepPassPWMs = 0;
				motor2.totalPWMs = 0;
				motor2.currentStage = 0; 
				//启动电机3
				ioMotor3Direction = MOTOR_BACKWARD;
				motor3.status = MOTOR_BACKWARD;
				motor3.stepPWMs = 3000000;
				motor3.stepPassPWMs = 0;
				MotorGroup2Start();

				displayMode = DISPLAY_RUN;
				ResetMotorDispatchSteps ++;
			}
		break;
		case 3:
			if(sensorStartPosi3 == 0)
			{
				//关闭电机3
			   	MotorGroup2Stop();
				motor3.position = 0;
				motor3.status = MOTOR_STOP;
				motor3.isStartPosition = 1;
				motor3.stepPWMs = 0;
				motor3.stepPassPWMs = 0;
				motor3.totalPWMs = 0;
				motor3.currentStage = 0; 
				//启动电机4
				ioMotor4Direction = MOTOR_BACKWARD;
				motor4.status = MOTOR_BACKWARD;
				motor4.stepPWMs = 3000000;
				motor4.stepPassPWMs = 0;
				MotorGroup2Start();

				displayMode = DISPLAY_RUN;
				ResetMotorDispatchSteps ++;
			}	
		break;
		case 4:
			if(sensorStartPosi4 == 0)
			{
				//关闭电机4
			   	MotorGroup2Stop();
				motor4.position = 0;
				motor4.status = MOTOR_STOP;
				motor4.isStartPosition = 1;
				motor4.stepPWMs = 0;
				motor4.stepPassPWMs = 0;
				motor4.totalPWMs = 0;
				motor4.currentStage = 0; 

				displayMode = DISPLAY_RUN;
				ResetMotorDispatchSteps = 20;
			}	
		break;
		default:
			 _nop_();
	}
}


void motor1Forward(void)
{
	if(motor1.status == MOTOR_STOP && motor2.status == MOTOR_STOP && motor1.position == motor2.position && motor1.currentStage == motor2.currentStage)
	{
		 if(motor1.currentStage % 2 == 0) //在前进过程中
		 {
		 	 ioMotor1Direction = MOTOR_FORWARD;
			 motor1.status = MOTOR_FORWARD;
			 lastStatus1 =  MOTOR_FORWARD;
			 motor1.isStartPosition = 0;
			 motor1.stepPWMs = motorRotationAngle1[motor1.currentStage][motor1.position] << 1;
			 lastStepPWMs1 = motor1.stepPWMs;
			 motor1.stepPassPWMs = 0;
			 motor1.totalPWMs += motorRotationAngle1[motor1.currentStage][motor1.position];
			 if(motorRotationAngle1[motor1.currentStage][motor1.position + 1] == 0) //下面没有步数了
			 {
			 	motor1.currentStage ++;
				motor1.position = 0;
	
			 }
			 else //下面还有步数
			 {
			 	 motor1.position ++;
			 }
			 MotorGroup1Start();
			 displayMode = DISPLAY_RUN;
		 }
		 else  //不在前进过程中
		 {
		 	displayMode = DIAPLAY_MOTOR_BACKWARD;
		 }
	}
	else
	{
		displayMode = DIAPLAY_MOTOR2_OPEATION;
	}
}


void motor1Backward(void)
{
	if(motor1.status == MOTOR_STOP && motor2.status == MOTOR_STOP && motor1.position == motor2.position && motor1.currentStage == motor2.currentStage)
	{
		 if(motor1.currentStage % 2 == 1) //在后退过程中
		{
			ioMotor1Direction = MOTOR_BACKWARD;
			motor1.status = MOTOR_BACKWARD;
			lastStatus1 =  MOTOR_BACKWARD;
			motor1.stepPWMs = motorRotationAngle1[motor1.currentStage][motor1.position] << 1;
			lastStepPWMs1 = motor1.stepPWMs;
			motor1.stepPassPWMs = 0;
			motor1.totalPWMs -= motorRotationAngle1[motor1.currentStage][motor1.position];
			if(motorRotationAngle1[motor1.currentStage][motor1.position + 1] == 0) //下面没有步数了
			{
				motor1.isStartPosition = 1;
				motor1.position = 0;
				motor1.currentStage ++;
				if(motor1.currentStage > 5)  //三个来回走完
				{
					motor1.currentStage = 0;
				}
				else if(motorRotationAngle1[motor1.currentStage][motor1.position] == 0) //下一个过程没有配置
				{
					motor1.currentStage = 0;
				}
			}
			else //下面还有步数
			{
				motor1.position++;
			}				
			MotorGroup1Start();
			displayMode = DISPLAY_RUN;
		}
		else  //不在后退过程中
		{
			displayMode = DIAPLAY_MOTOR_FORWARD;
		}
	}
	else
	{
		displayMode = DIAPLAY_MOTOR2_OPEATION;
	}	
}

void motor2Copy(void)
{
	if(motor1.status != MOTOR_STOP || motor2.status != MOTOR_STOP || (motor1.position == motor2.position && motor1.currentStage == motor2.currentStage))
	{

	}
	else
	{
		ioMotor2Direction = lastStatus1;
		motor2.position = motor1.position;
		motor2.status = lastStatus1;
		motor2.isStartPosition = motor1.isStartPosition;
		motor2.stepPWMs = lastStepPWMs1;
		motor2.stepPassPWMs = 0;
		motor2.totalPWMs = motor1.totalPWMs;
		motor2.currentStage = motor1.currentStage;
		MotorGroup1Start();
		displayMode = DISPLAY_RUN;
	}
}

void motor3Forward(void)
{
	if(motor3.status == MOTOR_STOP && motor4.status == MOTOR_STOP && motor3.position == motor4.position && motor3.currentStage == motor4.currentStage)
	{
		 if(motor3.currentStage % 2 == 0) //在前进过程中
		 {
		 	 ioMotor3Direction = MOTOR_FORWARD;
			 motor3.status = MOTOR_FORWARD;
			 lastStatus2 =  MOTOR_FORWARD;
			 motor3.isStartPosition = 0;
			 motor3.stepPWMs = motorRotationAngle2[motor3.currentStage][motor3.position] << 1;
			 lastStepPWMs2 = motor3.stepPWMs;
			 motor3.stepPassPWMs = 0;
			 motor3.totalPWMs += motorRotationAngle2[motor3.currentStage][motor3.position];
			 if(motorRotationAngle2[motor3.currentStage][motor3.position + 1] == 0) //下面没有步数了
			 {
			 	motor3.currentStage ++;
				motor3.position = 0;
	
			 }
			 else //下面还有步数
			 {
			 	 motor3.position ++;
			 }
			 MotorGroup2Start();
			 displayMode = DISPLAY_RUN;
		 }
		 else  //不在前进过程中
		 {
		 	displayMode = DIAPLAY_MOTOR_BACKWARD;
		 }
	}
	else
	{
		displayMode = DIAPLAY_MOTOR2_OPEATION;
	}
}


void motor3Backward(void)
{
	if(motor3.status == MOTOR_STOP && motor4.status == MOTOR_STOP && motor3.position == motor4.position && motor3.currentStage == motor4.currentStage)
	{
		 if(motor3.currentStage % 2 == 1) //在后退过程中
		{
			ioMotor3Direction = MOTOR_BACKWARD;
			motor3.status = MOTOR_BACKWARD;
			lastStatus2 =  MOTOR_BACKWARD;
			motor3.stepPWMs = motorRotationAngle2[motor3.currentStage][motor3.position] << 1;
			lastStepPWMs2 = motor3.stepPWMs;
			motor3.stepPassPWMs = 0;
			motor3.totalPWMs -= motorRotationAngle2[motor3.currentStage][motor3.position];
			if(motorRotationAngle2[motor3.currentStage][motor3.position + 1] == 0) //下面没有步数了
			{
				motor3.isStartPosition = 1;
				motor3.position = 0;
				motor3.currentStage ++;
				if(motor3.currentStage > 5)  //三个来回走完
				{
					motor3.currentStage = 0;
				}
				else if(motorRotationAngle2[motor3.currentStage][motor3.position] == 0) //下一个过程没有配置
				{
					motor3.currentStage = 0;
				}
			}
			else //下面还有步数
			{
				motor3.position++;
			}				
			MotorGroup2Start();
			displayMode = DISPLAY_RUN;
		}
		else  //不在后退过程中
		{
			displayMode = DIAPLAY_MOTOR_FORWARD;
		}
	}
	else
	{
		displayMode = DIAPLAY_MOTOR2_OPEATION;
	}	
}

void motor4Copy(void)
{
	if(motor3.status != MOTOR_STOP || motor4.status != MOTOR_STOP || (motor3.position == motor4.position && motor3.currentStage == motor4.currentStage))
	{

	}
	else
	{
		ioMotor4Direction = lastStatus2;
		motor4.position = motor3.position;
		motor4.status = lastStatus2;
		motor4.isStartPosition = motor3.isStartPosition;
		motor4.stepPWMs = lastStepPWMs2;
		motor4.stepPassPWMs = 0;
		motor4.totalPWMs = motor3.totalPWMs;
		motor4.currentStage = motor3.currentStage;
		MotorGroup2Start();
		displayMode = DISPLAY_RUN;
	}
}

