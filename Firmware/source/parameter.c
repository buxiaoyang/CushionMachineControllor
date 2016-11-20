/***************************************************************************/
// 程序：15型座垫机控制系统
// 模块：参数函数
// 文件：parameter.c
// 作者：卜晓旸
// 版本：V0.01
// 日期：2016年4月3日
// 功能：主函数
// 芯片：STC12C5A60S2
// 编译：Keil uVision3 V3.90
/***************************************************************************/

#include <parameter.h>
#include <eeprom.h>
#include <at24c256.h>
#include <basefunc.h>
#include <dispatch.h>

/***************************************************************************/
// 参数定义
/***************************************************************************/

enum RunMode runMode; //运行模式 0：停止 1：运行
struct Motor motor1, motor2, motor3, motor4; //电机
enum DisplayMode displayMode; //刷新屏幕标志位 0 不刷新
enum SaveMode saveMode; //保存模式
struct Setting setting; //参数设置
unsigned int productNum; //生产件数 

unsigned char activeSensorRun; //当前有效运行感应器  1:一号传感器  2：二号传感器 3：三号传感器 4：四号传感器

unsigned char sensorRunCheck; //开机检测是否刚好在传感器位置标志位允许运行标志位  0:不允许运行  1:允许运行

unsigned char powerOnStepStatus; //开机当前时多少布 1：奇数步2号感应器有效  2：偶数步1号感应器有效  3：正常运行

/***************************************************************************/
// 参数初始化函数
// 参数：无
// 返回值：无	
/***************************************************************************/
void parameter_init()
{	
	setting.currentMood = RdFromROM(0x0000);
	//setting.currentStep = 1;
	setting.totalMood = 2;
	if(!parameter_read())
	{
		setting.currentMood = 1;
		setting.totalSteps = 600;
	
		setting.motor1Steps = 100;
		setting.motor2Steps = 200;
		setting.motor3Steps = 300;
		setting.motor4Steps = 400;
	
		setting.moodStatus[0] = 0xFFFE;
		setting.moodStatus[1] = 0x7FFF;
		setting.moodStatus[2] = 0xFFFF;
		setting.moodStatus[3] = 0xFFFF;
		setting.moodStatus[4] = 0xFFFF;
		setting.moodStatus[5] = 0xFFFF;
		setting.moodStatus[6] = 0xFFFF;
		setting.moodStatus[7] = 0xFFFF;
	}

	//电机参数初始化
	motor1.status = MOTOR_STOP;
	motor1.stepPWMs = 0;
	motor1.stepPassPWMs = 0;
	
	motor2.status = MOTOR_STOP;
	motor2.stepPWMs = 0;
	motor2.stepPassPWMs = 0;

	motor3.status = MOTOR_STOP;
	motor3.stepPWMs = 0;
	motor3.stepPassPWMs = 0;

	motor4.status = MOTOR_STOP;
	motor4.stepPWMs = 0;
	motor4.stepPassPWMs = 0;


	runMode = MODEL_RUN;
	ioAllowMotorStart = 0;
	displayMode = DISPLAY_RUN;
	saveMode = SAVE_NO_SAVING;

	activeSensorRun = 0; //设置当前有效传感器为空
	sensorRunCheck = 0; //假设在传感器位置不允许运行
}

/***************************************************************************/
// 参数保存到eeprom
// 参数：无
// 返回值：0失败 1成功	
/***************************************************************************/
unsigned char parameter_save_mood()
{
	WORD result = 1;
	WrToROM(0x0000, setting.currentMood);
	return result;
}

/***************************************************************************/
// 参数保存到eeprom
// 参数：无
// 返回值：0失败 1成功	
/***************************************************************************/
unsigned char parameter_save()
{
	WORD result = 1, baseAddress, stepAddress;

	baseAddress = (setting.currentMood - 1) * MOOD_STORAGE_SIZE + 2;
	WrToROM(baseAddress, (BYTE)(setting.totalSteps >> 8));
	WrToROM(baseAddress+1, (BYTE)setting.totalSteps);
	
	stepAddress =  baseAddress + 24 * (setting.currentStep-1) + 2;
	WrToROM(stepAddress, (BYTE)(setting.motor1Steps >> 8));
	WrToROM(stepAddress+1, (BYTE)setting.motor1Steps);
	WrToROM(stepAddress+2, (BYTE)(setting.motor2Steps >> 8));
	WrToROM(stepAddress+3, (BYTE)setting.motor2Steps);
	WrToROM(stepAddress+4, (BYTE)(setting.motor3Steps >> 8));
	WrToROM(stepAddress+5, (BYTE)setting.motor3Steps);
	WrToROM(stepAddress+6, (BYTE)(setting.motor4Steps >> 8));
	WrToROM(stepAddress+7, (BYTE)setting.motor4Steps);
	WrToROM(stepAddress+8, (BYTE)(setting.moodStatus[0] >> 8));
	WrToROM(stepAddress+9, (BYTE)setting.moodStatus[0]);
	WrToROM(stepAddress+10, (BYTE)(setting.moodStatus[1] >> 8));
	WrToROM(stepAddress+11, (BYTE)setting.moodStatus[1]);
	WrToROM(stepAddress+12, (BYTE)(setting.moodStatus[2] >> 8));
	WrToROM(stepAddress+13, (BYTE)setting.moodStatus[2]);
	WrToROM(stepAddress+14, (BYTE)(setting.moodStatus[3] >> 8));
	WrToROM(stepAddress+15, (BYTE)setting.moodStatus[3]);
	WrToROM(stepAddress+16, (BYTE)(setting.moodStatus[4] >> 8));
	WrToROM(stepAddress+17, (BYTE)setting.moodStatus[4]);
	WrToROM(stepAddress+18, (BYTE)(setting.moodStatus[5] >> 8));
	WrToROM(stepAddress+19, (BYTE)setting.moodStatus[5]);
	WrToROM(stepAddress+20, (BYTE)(setting.moodStatus[6] >> 8));
	WrToROM(stepAddress+21, (BYTE)setting.moodStatus[6]);
	WrToROM(stepAddress+22, (BYTE)(setting.moodStatus[7] >> 8));
	WrToROM(stepAddress+23, (BYTE)setting.moodStatus[7]);
	return result;
}

/***************************************************************************/
// 读取参数
// 参数：无
// 返回值：0失败 1成功	
/***************************************************************************/
unsigned char parameter_read()
{
	WORD result = 1, baseAddress, stepAddress;
	baseAddress = (setting.currentMood - 1) * MOOD_STORAGE_SIZE + 2;
	setting.totalSteps = (WORD)((RdFromROM(baseAddress) << 8) | RdFromROM(baseAddress + 1));

	stepAddress =  baseAddress + 24 * (setting.currentStep-1) + 2;
	setting.motor1Steps = (WORD)((RdFromROM(stepAddress) << 8) | RdFromROM(stepAddress + 1));
	setting.motor2Steps = (WORD)((RdFromROM(stepAddress + 2) << 8) | RdFromROM(stepAddress + 3));
	setting.motor3Steps = (WORD)((RdFromROM(stepAddress + 4) << 8) | RdFromROM(stepAddress + 5));
	setting.motor4Steps = (WORD)((RdFromROM(stepAddress + 6) << 8) | RdFromROM(stepAddress + 7));
	setting.moodStatus[0] = (WORD)((RdFromROM(stepAddress + 8) << 8) | RdFromROM(stepAddress + 9));
	setting.moodStatus[1] = (WORD)((RdFromROM(stepAddress + 10) << 8) | RdFromROM(stepAddress + 11));
	setting.moodStatus[2] = (WORD)((RdFromROM(stepAddress + 12) << 8) | RdFromROM(stepAddress + 13));
	setting.moodStatus[3] = (WORD)((RdFromROM(stepAddress + 14) << 8) | RdFromROM(stepAddress + 15));
	setting.moodStatus[4] = (WORD)((RdFromROM(stepAddress + 16) << 8) | RdFromROM(stepAddress + 17));
	setting.moodStatus[5] = (WORD)((RdFromROM(stepAddress + 18) << 8) | RdFromROM(stepAddress + 19));
	setting.moodStatus[6] = (WORD)((RdFromROM(stepAddress + 20) << 8) | RdFromROM(stepAddress + 21));
	setting.moodStatus[7] = (WORD)((RdFromROM(stepAddress + 22) << 8) | RdFromROM(stepAddress + 23));

	return result;
}


void snapshot_init()
{
	Delay(10);
	/* 
	if(IapReadByte(IAP_ADDRESS+511) == 0xAA)
	{	
		productNum = (WORD)((IapReadByte(IAP_ADDRESS + 8) << 8) | IapReadByte(IAP_ADDRESS + 9));
		setting.currentStep = (WORD)((IapReadByte(IAP_ADDRESS + 10) << 8) | IapReadByte(IAP_ADDRESS + 11));
	}
	else
	{
		productNum = 0;	
		setting.currentStep = 1;
	}
	*/
	productNum = (WORD)((RdFromROM(32760) << 8) | RdFromROM(32761));
	setting.currentStep = (WORD)((RdFromROM(32762) << 8) | RdFromROM(32763));
	
	if(setting.currentStep%2 == 0)//偶数步
	{
		 powerOnStepStatus = 2;
	}
	else //奇数步
	{
		 powerOnStepStatus = 1;
	}
	

	/*
	//步数递增
	setting.currentStep ++;
	if(setting.currentStep > setting.totalSteps)
	{
		setting.currentStep = 1;
		productNum ++;
	}
	*/
}

void snapshot_save()
{
	/*
   	Delay(10);
	IapEraseSector(IAP_ADDRESS); //擦除EEPROM
	Delay(10);  
	IapProgramByte(IAP_ADDRESS + 8, (BYTE)(productNum>>8));
	IapProgramByte(IAP_ADDRESS + 9, (BYTE)productNum);
	IapProgramByte(IAP_ADDRESS + 10, (BYTE)(setting.currentStep>>8));
	IapProgramByte(IAP_ADDRESS + 11, (BYTE)setting.currentStep);
	IapProgramByte(IAP_ADDRESS+511, 0xAA); //写入标志位
	Delay(10);
	*/
	WrToROM(32760, (BYTE)(productNum >> 8));
	WrToROM(32761, (BYTE)productNum);
	WrToROM(32762, (BYTE)(setting.currentStep >> 8));
	WrToROM(32763, (BYTE)setting.currentStep);
}
