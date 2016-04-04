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
			if(motor1.status == MOTOR_STOP && motor2.status == MOTOR_STOP && motor1.position == motor1.position && motor1.currentStage == motor1.currentStage) //�����ڶ��ŵ������֮�����������
			{
				//�������1
				ioMotor1Direction = MOTOR_BACKWARD;
			   	motor1.status = MOTOR_BACKWARD;
				motor1.stepPWMs = 3000000;
				motor1.stepPassPWMs = 0;
				Motor1Start();
				ResetMotorDispatchSteps ++;
			}
		break;
		case 1:
			if(sensorStartPosi1 == 0)
			{
			   	ResetMotorDispatchSteps ++; 
			}
		break;
		case 2:
			if(sensorStartPosi1 == 0)
			{
				Motor1Stop();
				motor1.position = 0;
				motor1.status = MOTOR_STOP;
				motor1.isStartPosition = 1;
				motor1.stepPWMs = 0;
				motor1.stepPassPWMs = 0;
				motor1.totalPWMs = 0;
				motor1.currentStage = 0; 
				
				ioMmotor2Direction = MOTOR_BACKWARD;
				motor2.status = MOTOR_BACKWARD;
				motor2.stepPWMs = 3000000;
				motor2.stepPassPWMs = 0;
				Motor2Start();

				ResetMotorDispatchSteps ++;
				displayMode = DISPLAY_RUN;	 
			}
		break;
		case 3:
			if(sensorStartPosi2 == 0)
			{
			   	ResetMotorDispatchSteps ++; 
			}	
		break;
		case 4:
			if(sensorStartPosi2 == 0)
			{
			   	Motor2Stop();
				motor2.position = 0;
				motor2.status = MOTOR_STOP;
				motor2.isStartPosition = 1;
				motor2.stepPWMs = 0;
				motor2.stepPassPWMs = 0;
				motor2.totalPWMs = 0;
				motor2.currentStage = 0;

				ResetMotorDispatchSteps = 20; 
				displayMode = DISPLAY_RUN;
			}	
		break;
		default:
			 _nop_();
	}
}
