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
			if(keyMotor1Forward == 0) //���1ǰ��
			{
				motor1Forward();
			}
			else if(keyMotor1Backward == 0) //���1����
			{
				motor1Backward();
			}
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
			if(keyMotor2Forward == 0) //���2ǰ��
			{
				motor2Copy();
			}
			else if(keyMotor2Backward == 0) //���2����
			{
				//motor2Backward();
			}
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