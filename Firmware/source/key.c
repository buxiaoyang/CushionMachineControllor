#include <reg52.h>
#include <intrins.h>
#include <parameter.h>
#include <key.h>
#include <dispatch.h>
#include <timer.h>


void Key_Scan1(void); //接近开关送信号X2 按键扫描

unsigned char Key_Scan_Steps1 = 0; 		//0：初始状态 如果有按键按下则进入1
										//1：延迟10ms
										//2：如果按键确实按下则进入3
										//3：确定按键
										//4：如果按键释放则进入0

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
				//步数递增
				setting.currentStep ++;
				if(setting.currentStep > setting.totalSteps)
				{
					setting.currentStep = 1;
					productNum ++;
				}
				saveMode = SAVE_SETTING_STEP;
				displayMode = DISPLAY_RUN;
			}
			else if(sensorRun2 == 0)
			{
				//步数递增
				setting.currentStep ++;
				if(setting.currentStep > setting.totalSteps)
				{
					setting.currentStep = 1;
					productNum ++;
				}
				saveMode = SAVE_SETTING_STEP;
				displayMode = DISPLAY_RUN;
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