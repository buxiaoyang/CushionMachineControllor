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
				motor4.status == MOTOR_STOP
			)
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
				motor1.status = MOTOR_STOP;
				motor1.stepPWMs = 0;
				motor1.stepPassPWMs = 0;
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
				motor2.status = MOTOR_STOP;
				motor2.stepPWMs = 0;
				motor2.stepPassPWMs = 0;
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
				motor3.status = MOTOR_STOP;
				motor3.stepPWMs = 0;
				motor3.stepPassPWMs = 0;
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
				motor4.status = MOTOR_STOP;
				motor4.stepPWMs = 0;
				motor4.stepPassPWMs = 0;

				displayMode = DISPLAY_RUN;
				ResetMotorDispatchSteps = 20;
			}	
		break;
		default:
			 _nop_();
	}
}


