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

enum RunMode runMode; //运行模式 0：停止 1：运行
struct Motor motor1, motor2; //电机
unsigned int motorRotationAngle[6][40]; //电机旋转角 6组数据，每组40个
unsigned char motorCurrentRatationGroup; //当前设置旋转角组
enum DisplayMode displayMode; //刷新屏幕标志位 0 不刷新
enum SaveMode saveMode;

/***************************************************************************/
// 读取参数
// 参数：无
// 返回值：0失败 1成功	
/***************************************************************************/
unsigned char parameter_read()
{
	WORD result = 0,i,j,address;
	Delay(10); 
	if(IapReadByte(IAP_ADDRESS+511) == 0xEE)
	{	
		for(i=0; i < 6; i++)
		{
			for(j=0; j < 40; j++)
			{
				address = i * 80 + (j<<1);
				motorRotationAngle[i][j] = (WORD)((IapReadByte(IAP_ADDRESS + address) << 8) | IapReadByte(IAP_ADDRESS + address + 1));
			}
			
		}
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
	WORD i,j;
	if(!parameter_read())
	{
		for(i=0; i < 6; i++)
		{
			for(j=0; j < 40; j++)
			{
				motorRotationAngle[i][j] = 0;
			}
		}	
	}

	runMode = MODEL_RUN;
	motorCurrentRatationGroup = 0;
	displayMode = DISPLAY_RUN;
	saveMode = SAVE_NO_SAVING;
}

/***************************************************************************/
// 参数保存到eeprom
// 参数：无
// 返回值：0失败 1成功	
/***************************************************************************/
unsigned char parameter_save()
{
	WORD result = 1,i,j,address;
    Delay(10);                     //Delay
	IapEraseSector(IAP_ADDRESS); //擦除EEPROM
	Delay(10);  
	//写入参数
	for(i=0; i < 6; i++)
	{
		for(j=0; j < 40; j++)
		{
			address = i * 80 + (j<<1);
			IapProgramByte(IAP_ADDRESS + address, (BYTE)(motorRotationAngle[i][j]>>8));
			IapProgramByte(IAP_ADDRESS + address+1, (BYTE)(motorRotationAngle[i][j]));
		}
	}
	IapProgramByte(IAP_ADDRESS+511, 0xEE); //写入标志位
	Delay(10);
	//测试写入
	for(i=0; i < 6; i++)
	{
		for(j=0; j < 40; j++)
		{
			address = i * 80 + (j<<1);
			if(motorRotationAngle[i][j] != (WORD)((IapReadByte(IAP_ADDRESS + address) << 8) | IapReadByte(IAP_ADDRESS + address + 1)))
			{
				result = 0;
			}
		}
		
	}
	return result;
}

void snapshot_init()
{
	if(IapReadByte(IAP_ADDRESS2+511) == 0xEE)
	{
		//电机参数初始化
		motor1.position = IapReadByte(IAP_ADDRESS2);
		motor1.status = IapReadByte(IAP_ADDRESS2+1);
		motor1.isStartPosition = IapReadByte(IAP_ADDRESS2+2);
		motor1.stepPWMs = (unsigned long)((IapReadByte(IAP_ADDRESS2+3)<<24) | (IapReadByte(IAP_ADDRESS2+4)<<16) | (IapReadByte(IAP_ADDRESS2+5)<<8) | (IapReadByte(IAP_ADDRESS2+6)));
		motor1.stepPassPWMs = (unsigned long)((IapReadByte(IAP_ADDRESS2+7)<<24) | (IapReadByte(IAP_ADDRESS2+8)<<16) | (IapReadByte(IAP_ADDRESS2+9)<<8) | (IapReadByte(IAP_ADDRESS2+10)));
		motor1.totalPWMs = (unsigned long)((IapReadByte(IAP_ADDRESS2+3)<<11) | (IapReadByte(IAP_ADDRESS2+4)<<12) | (IapReadByte(IAP_ADDRESS2+5)<<13) | (IapReadByte(IAP_ADDRESS2+14)));
		motor1.currentStage = IapReadByte(IAP_ADDRESS2+15);
	
		motor2.position = IapReadByte(IAP_ADDRESS2+16);
		motor2.status = IapReadByte(IAP_ADDRESS2+17);
		motor2.isStartPosition = IapReadByte(IAP_ADDRESS2+18);
		motor2.stepPWMs = (unsigned long)((IapReadByte(IAP_ADDRESS2+3)<<19) | (IapReadByte(IAP_ADDRESS2+4)<<20) | (IapReadByte(IAP_ADDRESS2+5)<<21) | (IapReadByte(IAP_ADDRESS2+22)));
		motor2.stepPassPWMs = (unsigned long)((IapReadByte(IAP_ADDRESS2+7)<<23) | (IapReadByte(IAP_ADDRESS2+8)<<24) | (IapReadByte(IAP_ADDRESS2+9)<<25) | (IapReadByte(IAP_ADDRESS2+26)));
		motor2.totalPWMs = (unsigned long)((IapReadByte(IAP_ADDRESS2+3)<<27) | (IapReadByte(IAP_ADDRESS2+4)<<28) | (IapReadByte(IAP_ADDRESS2+5)<<29) | (IapReadByte(IAP_ADDRESS2+30)));
		motor2.currentStage = IapReadByte(IAP_ADDRESS2+31);

		//初始化电机动作

	}
	else
	{
	   	//电机参数初始化
		motor1.position = 0;
		motor1.status = MOTOR_STOP;
		motor1.isStartPosition = 1;
		motor1.stepPWMs = 0;
		motor1.stepPassPWMs = 0;
		motor1.totalPWMs = 0;
		motor1.currentStage = 0;
	
		motor2.position = 0;
		motor2.status = MOTOR_STOP;
		motor2.isStartPosition = 1;
		motor2.stepPWMs = 0;
		motor2.stepPassPWMs = 0;
		motor2.totalPWMs = 0;
		motor2.currentStage = 0;
	}
	displayMode = DISPLAY_RUN;
}

void snapshot_save()
{
	IapEraseSector(IAP_ADDRESS2); //擦除EEPROM
	delay_ms(3);  
	//写入参数 电机1
	IapProgramByte(IAP_ADDRESS2, motor1.position);
	IapProgramByte(IAP_ADDRESS2+1, motor1.status); 
	IapProgramByte(IAP_ADDRESS2+2, motor1.isStartPosition); 
	IapProgramByte(IAP_ADDRESS2+3, (BYTE)(motor1.stepPWMs>>24)); 
	IapProgramByte(IAP_ADDRESS2+4, (BYTE)(motor1.stepPWMs>>16)); 
	IapProgramByte(IAP_ADDRESS2+5, (BYTE)(motor1.stepPWMs>>8)); 
	IapProgramByte(IAP_ADDRESS2+6, (BYTE)(motor1.stepPWMs)); 
	IapProgramByte(IAP_ADDRESS2+7, (BYTE)(motor1.stepPassPWMs>>24)); 
	IapProgramByte(IAP_ADDRESS2+8, (BYTE)(motor1.stepPassPWMs>>16)); 
	IapProgramByte(IAP_ADDRESS2+9, (BYTE)(motor1.stepPassPWMs>>8)); 
	IapProgramByte(IAP_ADDRESS2+10, (BYTE)(motor1.stepPassPWMs));
	IapProgramByte(IAP_ADDRESS2+11, (BYTE)(motor1.totalPWMs>>24)); 
	IapProgramByte(IAP_ADDRESS2+12, (BYTE)(motor1.totalPWMs>>16)); 
	IapProgramByte(IAP_ADDRESS2+13, (BYTE)(motor1.totalPWMs>>8)); 
	IapProgramByte(IAP_ADDRESS2+14, (BYTE)(motor1.totalPWMs));
	IapProgramByte(IAP_ADDRESS2+15, motor1.currentStage); 
	//写入参数 电机2
	IapProgramByte(IAP_ADDRESS2+16, motor2.position);
	IapProgramByte(IAP_ADDRESS2+17, motor2.status); 
	IapProgramByte(IAP_ADDRESS2+18, motor2.isStartPosition); 
	IapProgramByte(IAP_ADDRESS2+19, (BYTE)(motor2.stepPWMs>>24)); 
	IapProgramByte(IAP_ADDRESS2+20, (BYTE)(motor2.stepPWMs>>16)); 
	IapProgramByte(IAP_ADDRESS2+21, (BYTE)(motor2.stepPWMs>>8)); 
	IapProgramByte(IAP_ADDRESS2+22, (BYTE)(motor2.stepPWMs)); 
	IapProgramByte(IAP_ADDRESS2+23, (BYTE)(motor2.stepPassPWMs>>24)); 
	IapProgramByte(IAP_ADDRESS2+24, (BYTE)(motor2.stepPassPWMs>>16)); 
	IapProgramByte(IAP_ADDRESS2+25, (BYTE)(motor2.stepPassPWMs>>8)); 
	IapProgramByte(IAP_ADDRESS2+26, (BYTE)(motor2.stepPassPWMs));
	IapProgramByte(IAP_ADDRESS2+27, (BYTE)(motor2.totalPWMs>>24)); 
	IapProgramByte(IAP_ADDRESS2+28, (BYTE)(motor2.totalPWMs>>16)); 
	IapProgramByte(IAP_ADDRESS2+29, (BYTE)(motor2.totalPWMs>>8)); 
	IapProgramByte(IAP_ADDRESS2+30, (BYTE)(motor2.totalPWMs));
	IapProgramByte(IAP_ADDRESS2+31, motor2.currentStage);
	//写入标志位
	IapProgramByte(IAP_ADDRESS2+511, 0xEE);
	delay_ms(10);
}
