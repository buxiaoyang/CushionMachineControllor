#include <reg52.h>
#include <parameter.h>
#include <intrins.h>
#include <timer.h>
#include <dispatch.h>


unsigned char ResetMotorDispatchSteps = 20; 



//�������״̬����ÿ10�������һ�Σ�����֮ǰȷ��������ڿ���״̬����ResetMotorDispatchSteps == 20 ʱ����ResetMotorDispatchSteps = 0 ��ʼ״̬��
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
				//�������1
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
				//�رյ��1
			   	MotorGroup1Stop();
				motor1.status = MOTOR_STOP;
				motor1.stepPWMs = 0;
				motor1.stepPassPWMs = 0;
				//�������2
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
				//�رյ��2
			   	MotorGroup1Stop();
				motor2.status = MOTOR_STOP;
				motor2.stepPWMs = 0;
				motor2.stepPassPWMs = 0;
				//�������3
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
				//�رյ��3
			   	MotorGroup2Stop();
				motor3.status = MOTOR_STOP;
				motor3.stepPWMs = 0;
				motor3.stepPassPWMs = 0;
				//�������4
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
				//�رյ��4
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


