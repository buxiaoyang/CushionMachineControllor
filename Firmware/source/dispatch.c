#include <reg52.h>
#include <parameter.h>
#include <intrins.h>
#include <timer.h>
#include <dispatch.h>


unsigned char ResetMotorDispatchSteps = 20; 


unsigned long lastStepPWMs; //������������
unsigned char lastStatus; //�������

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
				
				ioMotor2Direction = MOTOR_BACKWARD;
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


void motor1Forward(void)
{
	if(motor1.status == MOTOR_STOP && motor2.status == MOTOR_STOP && motor1.position == motor2.position && motor1.currentStage == motor2.currentStage)
	{
		 if(motor1.currentStage % 2 == 0) //��ǰ��������
		 {
		 	 ioMotor1Direction = MOTOR_FORWARD;
			 motor1.status = MOTOR_FORWARD;
			 lastStatus =  MOTOR_FORWARD;
			 motor1.isStartPosition = 0;
			 motor1.stepPWMs = motorRotationAngle[motor1.currentStage][motor1.position] << 1;
			 lastStepPWMs = motor1.stepPWMs;
			 motor1.stepPassPWMs = 0;
			 motor1.totalPWMs += motorRotationAngle[motor1.currentStage][motor1.position];
			 if(motorRotationAngle[motor1.currentStage][motor1.position + 1] == 0) //����û�в�����
			 {
			 	motor1.currentStage ++;
				motor1.position = 0;
	
			 }
			 else //���滹�в���
			 {
			 	 motor1.position ++;
			 }
			 Motor1Start();
			 displayMode = DISPLAY_RUN;
		 }
		 else  //����ǰ��������
		 {
		 	displayMode = DISPLAY_MAX_POSITION;
		 }
	}
}


void motor1Backward(void)
{
	if(motor1.status == MOTOR_STOP && motor2.status == MOTOR_STOP && motor1.position == motor2.position && motor1.currentStage == motor2.currentStage)
	{
		 if(motor1.currentStage % 2 == 1) //�ں��˹�����
		{
			ioMotor1Direction = MOTOR_BACKWARD;
			motor1.status = MOTOR_BACKWARD;
			lastStatus =  MOTOR_BACKWARD;
			motor1.stepPWMs = motorRotationAngle[motor1.currentStage][motor1.position] << 1;
			lastStepPWMs = motor1.stepPWMs;
			motor1.stepPassPWMs = 0;
			motor1.totalPWMs -= motorRotationAngle[motor1.currentStage][motor1.position];
			if(motorRotationAngle[motor1.currentStage][motor1.position + 1] == 0) //����û�в�����
			{
				motor1.isStartPosition = 1;
				motor1.position = 0;
				motor1.currentStage ++;
				if(motor1.currentStage > 5)  //������������
				{
					motor1.currentStage = 0;
				}
				else if(motorRotationAngle[motor1.currentStage][motor1.position] == 0) //��һ������û������
				{
					motor1.currentStage = 0;
				}
			}
			else //���滹�в���
			{
				motor1.position++;
			}				
			Motor1Start();
			displayMode = DISPLAY_RUN;
		}
		else  //���ں��˹�����
		{
			displayMode = DIAPLAY_MIN_POSITION;
		}
	}	
}


void motor2Forward(void)
{
	if(motor2.status == MOTOR_STOP)
	{
		if(motor2.currentStage % 2 == 0) //��ǰ��������
		{
		 	 ioMotor2Direction = MOTOR_FORWARD;
			 motor2.status = MOTOR_FORWARD;
			 motor2.isStartPosition = 0;
			 motor2.stepPWMs = motorRotationAngle[motor2.currentStage][motor2.position] << 1;
			 motor2.stepPassPWMs = 0;
			 motor2.totalPWMs += motorRotationAngle[motor2.currentStage][motor2.position];
			 if(motorRotationAngle[motor2.currentStage][motor2.position + 1] == 0) //����û�в�����
			 {
			 	motor2.currentStage ++;
				motor2.position = 0;
		
			 }
			 else //���滹�в���
			 {
			 	 motor2.position ++;
			 }
			 Motor2Start();
			 displayMode = DISPLAY_RUN;
		 }
		 else  //����ǰ��������
		 {
		 	displayMode = DISPLAY_MAX_POSITION;
		 }	
	}
	
}


void motor2Backward(void)
{
	if(motor2.status == MOTOR_STOP)
	{
	   	if(motor2.currentStage % 2 == 1) //�ں��˹�����
		{
			ioMotor2Direction = MOTOR_BACKWARD;
			motor2.status = MOTOR_BACKWARD;
			motor2.stepPWMs = motorRotationAngle[motor2.currentStage][motor2.position] << 1;
			motor2.stepPassPWMs = 0;
			motor2.totalPWMs -= motorRotationAngle[motor2.currentStage][motor2.position];
			if(motorRotationAngle[motor2.currentStage][motor2.position + 1] == 0) //����û�в�����
			{
				motor2.isStartPosition = 1;
				motor2.position = 0;
				motor2.currentStage ++;
				if(motor2.currentStage > 5)  //������������
				{
					motor2.currentStage = 0;
				}
				else if(motorRotationAngle[motor2.currentStage][motor2.position] == 0) //��һ������û������
				{
					motor2.currentStage = 0;
				}
			}
			else //���滹�в���
			{
				motor2.position++;
			}				
			Motor2Start();
			displayMode = DISPLAY_RUN;
		}
		else  //���ں��˹�����
		{
			displayMode = DIAPLAY_MIN_POSITION;
		}
	}
}

void motor2Copy(void)
{
	if(motor1.status == MOTOR_STOP && motor2.status == MOTOR_STOP)
	{
		if(motor1.position == motor2.position && motor1.currentStage == motor2.currentStage)
		{
			_nop_();
		}
		else
		{
			ioMotor2Direction = lastStatus;
			motor2.position = motor1.position;
			motor2.status = lastStatus;
			motor2.isStartPosition = motor1.isStartPosition;
			motor2.stepPWMs = lastStepPWMs;
			motor2.stepPassPWMs = 0;
			motor2.totalPWMs = motor1.totalPWMs;
			motor2.currentStage = motor1.currentStage;
			Motor2Start();
			displayMode = DISPLAY_RUN;
		}
		
	}
}