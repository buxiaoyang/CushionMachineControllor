#include <reg52.h>
#include <parameter.h>
#include <intrins.h>
#include <timer.h>
#include <dispatch.h>


unsigned char ResetMotorDispatchSteps = 20; 


unsigned long xdata lastStepPWMs1; //������������1
unsigned char xdata lastStatus1; //�������1

unsigned long xdata lastStepPWMs2; //������������1
unsigned char xdata lastStatus2; //�������1

//�������״̬����ÿ10�������һ�Σ�����֮ǰȷ��������ڿ���״̬����ResetMotorDispatchSteps == 20 ʱ����ResetMotorDispatchSteps = 0 ��ʼ״̬��
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
				motor3.currentStage == motor4.currentStage) //�����ڶ��ţ��ĺŵ������֮�����������
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
				motor1.position = 0;
				motor1.status = MOTOR_STOP;
				motor1.isStartPosition = 1;
				motor1.stepPWMs = 0;
				motor1.stepPassPWMs = 0;
				motor1.totalPWMs = 0;
				motor1.currentStage = 0; 
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
				motor2.position = 0;
				motor2.status = MOTOR_STOP;
				motor2.isStartPosition = 1;
				motor2.stepPWMs = 0;
				motor2.stepPassPWMs = 0;
				motor2.totalPWMs = 0;
				motor2.currentStage = 0; 
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
				motor3.position = 0;
				motor3.status = MOTOR_STOP;
				motor3.isStartPosition = 1;
				motor3.stepPWMs = 0;
				motor3.stepPassPWMs = 0;
				motor3.totalPWMs = 0;
				motor3.currentStage = 0; 
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
		 if(motor1.currentStage % 2 == 0) //��ǰ��������
		 {
		 	 ioMotor1Direction = MOTOR_FORWARD;
			 motor1.status = MOTOR_FORWARD;
			 lastStatus1 =  MOTOR_FORWARD;
			 motor1.isStartPosition = 0;
			 motor1.stepPWMs = motorRotationAngle1[motor1.currentStage][motor1.position] << 1;
			 lastStepPWMs1 = motor1.stepPWMs;
			 motor1.stepPassPWMs = 0;
			 motor1.totalPWMs += motorRotationAngle1[motor1.currentStage][motor1.position];
			 if(motorRotationAngle1[motor1.currentStage][motor1.position + 1] == 0) //����û�в�����
			 {
			 	motor1.currentStage ++;
				motor1.position = 0;
	
			 }
			 else //���滹�в���
			 {
			 	 motor1.position ++;
			 }
			 MotorGroup1Start();
			 displayMode = DISPLAY_RUN;
		 }
		 else  //����ǰ��������
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
		 if(motor1.currentStage % 2 == 1) //�ں��˹�����
		{
			ioMotor1Direction = MOTOR_BACKWARD;
			motor1.status = MOTOR_BACKWARD;
			lastStatus1 =  MOTOR_BACKWARD;
			motor1.stepPWMs = motorRotationAngle1[motor1.currentStage][motor1.position] << 1;
			lastStepPWMs1 = motor1.stepPWMs;
			motor1.stepPassPWMs = 0;
			motor1.totalPWMs -= motorRotationAngle1[motor1.currentStage][motor1.position];
			if(motorRotationAngle1[motor1.currentStage][motor1.position + 1] == 0) //����û�в�����
			{
				motor1.isStartPosition = 1;
				motor1.position = 0;
				motor1.currentStage ++;
				if(motor1.currentStage > 5)  //������������
				{
					motor1.currentStage = 0;
				}
				else if(motorRotationAngle1[motor1.currentStage][motor1.position] == 0) //��һ������û������
				{
					motor1.currentStage = 0;
				}
			}
			else //���滹�в���
			{
				motor1.position++;
			}				
			MotorGroup1Start();
			displayMode = DISPLAY_RUN;
		}
		else  //���ں��˹�����
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
		 if(motor3.currentStage % 2 == 0) //��ǰ��������
		 {
		 	 ioMotor3Direction = MOTOR_FORWARD;
			 motor3.status = MOTOR_FORWARD;
			 lastStatus2 =  MOTOR_FORWARD;
			 motor3.isStartPosition = 0;
			 motor3.stepPWMs = motorRotationAngle2[motor3.currentStage][motor3.position] << 1;
			 lastStepPWMs2 = motor3.stepPWMs;
			 motor3.stepPassPWMs = 0;
			 motor3.totalPWMs += motorRotationAngle2[motor3.currentStage][motor3.position];
			 if(motorRotationAngle2[motor3.currentStage][motor3.position + 1] == 0) //����û�в�����
			 {
			 	motor3.currentStage ++;
				motor3.position = 0;
	
			 }
			 else //���滹�в���
			 {
			 	 motor3.position ++;
			 }
			 MotorGroup2Start();
			 displayMode = DISPLAY_RUN;
		 }
		 else  //����ǰ��������
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
		 if(motor3.currentStage % 2 == 1) //�ں��˹�����
		{
			ioMotor3Direction = MOTOR_BACKWARD;
			motor3.status = MOTOR_BACKWARD;
			lastStatus2 =  MOTOR_BACKWARD;
			motor3.stepPWMs = motorRotationAngle2[motor3.currentStage][motor3.position] << 1;
			lastStepPWMs2 = motor3.stepPWMs;
			motor3.stepPassPWMs = 0;
			motor3.totalPWMs -= motorRotationAngle2[motor3.currentStage][motor3.position];
			if(motorRotationAngle2[motor3.currentStage][motor3.position + 1] == 0) //����û�в�����
			{
				motor3.isStartPosition = 1;
				motor3.position = 0;
				motor3.currentStage ++;
				if(motor3.currentStage > 5)  //������������
				{
					motor3.currentStage = 0;
				}
				else if(motorRotationAngle2[motor3.currentStage][motor3.position] == 0) //��һ������û������
				{
					motor3.currentStage = 0;
				}
			}
			else //���滹�в���
			{
				motor3.position++;
			}				
			MotorGroup2Start();
			displayMode = DISPLAY_RUN;
		}
		else  //���ں��˹�����
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

