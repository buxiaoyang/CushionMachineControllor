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
#include <basefunc.h>

/***************************************************************************/
// 参数定义	
/***************************************************************************/

unsigned char runMode; //运行模式 0：停止 1：运行

unsigned char motor1Position; //电机1当前位置 0~39
unsigned char motor2Position; //电机2当前位置 0~39

unsigned char motor1Direction; //电机1运行方向
unsigned char motor2Direction; //电机2运行方向

unsigned char isStartPosition1; //电机1是否在初始位置标志位
unsigned char isStartPosition2; //电机2是否在初始位置标志位

unsigned long stepTotalPWMs; //单步总脉冲数
unsigned long stepRemainPWMs; //单步剩余脉冲数
unsigned long totalPWMs; //运行总脉冲数

unsigned int motorRotationAngle[6][40]; //电机旋转角 6组数据，每组40个
unsigned int currentStage; //当前运行数据组0~5

unsigned char refreshDisplay; //刷新屏幕标志位 0 不刷新 1刷新



/***************************************************************************/
// 读取参数
// 参数：无
// 返回值：0失败 1成功	
/***************************************************************************/
unsigned char parameter_read()
{
	
	unsigned char result = 0;
	delay_ms(10); 
	if(IapReadByte(IAP_ADDRESS+200) == 0xEE)
	{	
		/*
		pulseSettingNum = ((IapReadByte(IAP_ADDRESS+0) << 8) | IapReadByte(IAP_ADDRESS+1));
		//pulseSettingFreq = ((IapReadByte(IAP_ADDRESS+2) << 8) | IapReadByte(IAP_ADDRESS+3));
		//motorStepAngle = ((IapReadByte(IAP_ADDRESS+4) << 8) | IapReadByte(IAP_ADDRESS+5));
		screwPitch = ((IapReadByte(IAP_ADDRESS+6) << 8) | IapReadByte(IAP_ADDRESS+7));
		motorReducGearRatio = ((IapReadByte(IAP_ADDRESS+8) << 8) | IapReadByte(IAP_ADDRESS+9));
		ballScrew = ((IapReadByte(IAP_ADDRESS+10) << 8) | IapReadByte(IAP_ADDRESS+11));
		motorRotationAngle = ((IapReadByte(IAP_ADDRESS+12) << 8) | IapReadByte(IAP_ADDRESS+13));
		*/
		result = 1;
	}
	return result;
}

/***************************************************************************/
// 参数初始化函数
// 参数：无
// 返回值：无	
/***************************************************************************/
void parameter_init()
{
	//currentPosition = 1; //当前位置 1~20
	if(!parameter_read())
	{
		/*
		pulseSettingNum = 400; //脉冲个数
		pulseSettingFreq = 400; //脉冲频率
		motorStepAngle = 180; //电机步进角
		screwPitch = 200; //丝杆丝距
		motorReducGearRatio = 100; //电机减速比
		ballScrew = 200; //丝杆导程
		motorRotationAngle = 36000; //电机旋转角
		*/
	}
	/*
	isStartPosition = 0; //初始位置
	refreshDisplay = 1;
	initFlag = 0;
	*/
}

/***************************************************************************/
// 参数保存到eeprom
// 参数：无
// 返回值：0失败 1成功	
/***************************************************************************/
unsigned char parameter_save()
{
	
	unsigned char result = 1;
    delay_ms(10);                      //Delay
	IapEraseSector(IAP_ADDRESS); //擦除EEPROM
	/*
   	IapProgramByte(IAP_ADDRESS+0, (BYTE)(pulseSettingNum>>8));
	IapProgramByte(IAP_ADDRESS+1, (BYTE)pulseSettingNum);
	IapProgramByte(IAP_ADDRESS+2, (BYTE)(pulseSettingFreq>>8));
	IapProgramByte(IAP_ADDRESS+3, (BYTE)pulseSettingFreq);
	IapProgramByte(IAP_ADDRESS+4, (BYTE)(motorStepAngle>>8));
	IapProgramByte(IAP_ADDRESS+5, (BYTE)motorStepAngle);
	IapProgramByte(IAP_ADDRESS+6, (BYTE)(screwPitch>>8));
	IapProgramByte(IAP_ADDRESS+7, (BYTE)screwPitch);
	IapProgramByte(IAP_ADDRESS+8, (BYTE)(motorReducGearRatio>>8));
	IapProgramByte(IAP_ADDRESS+9, (BYTE)motorReducGearRatio);
	IapProgramByte(IAP_ADDRESS+10, (BYTE)(ballScrew>>8));
	IapProgramByte(IAP_ADDRESS+11, (BYTE)ballScrew);
	IapProgramByte(IAP_ADDRESS+12, (BYTE)(motorRotationAngle>>8));
	IapProgramByte(IAP_ADDRESS+13, (BYTE)motorRotationAngle);
	IapProgramByte(IAP_ADDRESS+200, 0xEE); //写入标志位
	refreshDisplay = 0;
	*/
	return result;
}


