#include <reg52.h>
#include <intrins.h>
#include <parameter.h>
#include <key.h>
#include <dispatch.h>
#include <timer.h>


void Key_Scan1(void); //�ӽ��������ź�X2 ����ɨ��

unsigned char Key_Scan_Steps1 = 0; 		//0����ʼ״̬ ����а������������1
										//1���ӳ�10ms
										//2���������ȷʵ���������3
										//3��ȷ������
										//4����������ͷ������0

void Key_Scan1(void)
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
				if(displayMode == DISPLAY_NO_FRESH)
				{
					setting.currentStep ++;
					if(setting.currentStep > setting.totalSteps)
					{
						setting.currentStep = 1;
					}
					saveMode = SAVE_SETTING_STEP;
					displayMode = DISPLAY_RUN;
				}
			}
			else if(sensorRun2 == 0)
			{
				if(displayMode == DISPLAY_NO_FRESH)
				{
					setting.currentStep ++;
					if(setting.currentStep > setting.totalSteps)
					{
						setting.currentStep = 1;
					}
					saveMode = SAVE_SETTING_STEP;
					displayMode = DISPLAY_RUN;
				}
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