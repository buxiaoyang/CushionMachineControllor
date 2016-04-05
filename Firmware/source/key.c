#include <reg52.h>
#include <intrins.h>
#include <parameter.h>
#include <key.h>
#include <dispatch.h>
#include <timer.h>


void Key_Scan1(void); //���1����ɨ��

unsigned char Key_Scan_Steps1 = 0; 		//0����ʼ״̬ ����а������������1
										//1���ӳ�10ms
										//2���������ȷʵ���������3
										//3��ȷ������
										//4����������ͷ������0

void Key_Scan2(void); //���2����ɨ��

unsigned char Key_Scan_Steps2 = 0; 		//0����ʼ״̬ ����а������������1
										//1���ӳ�10ms
										//2���������ȷʵ���������3
										//3��ȷ������
										//4����������ͷ������0

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
			if(keyMotor1Forward == 0 && motor1.status == MOTOR_STOP) //���1ǰ��
			{
				 if(motor1.currentStage % 2 == 0) //��ǰ��������
				 {
				 	 ioMotor1Direction = MOTOR_FORWARD;
					 motor1.status = MOTOR_FORWARD;
					 motor1.isStartPosition = 0;
					 motor1.stepPWMs = motorRotationAngle[motor1.currentStage][motor1.position] << 1;
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
				 }
				 else  //����ǰ��������
				 {
				 	displayMode = DISPLAY_MAX_POSITION;
				 }
			}
			else if(keyMotor1Backward == 0 && motor1.status == MOTOR_STOP) //���1����
			{
				if(motor1.currentStage % 2 == 1) //�ں��˹�����
				{
					ioMotor1Direction = MOTOR_BACKWARD;
					motor1.status = MOTOR_BACKWARD;
					motor1.stepPWMs = motorRotationAngle[motor1.currentStage][motor1.position] << 1;
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
				}
				else  //���ں��˹�����
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
			if(keyMotor2Forward == 0 && motor2.status == MOTOR_STOP) //���2ǰ��
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
				 }
				 else  //����ǰ��������
				 {
				 	displayMode = DISPLAY_MAX_POSITION;
				 }
			}
			else if(keyMotor2Backward == 0 && motor2.status == MOTOR_STOP) //���2����
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
				}
				else  //���ں��˹�����
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