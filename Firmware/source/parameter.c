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


/***************************************************************************/
// 参数初始化函数
// 参数：无
// 返回值：无	
/***************************************************************************/
void parameter_init()
{	
	/*
	WORD i,j;
	if(!parameter_read())
	{
		for(i=0; i < 6; i++)
		{
			for(j=0; j < 40; j++)
			{
//				motorRotationAngle1[i][j] = 0;
//				motorRotationAngle2[i][j] = 0;
			}
		}	
	}
	*/
	runMode = MODEL_RUN;
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
	WORD result = 1;
    
	return result;
}

/***************************************************************************/
// 读取参数
// 参数：无
// 返回值：0失败 1成功	
/***************************************************************************/
unsigned char parameter_read()
{
	WORD result = 0;

	return result;
}

void snapshot_init()
{
	Delay(10); 
	if(IapReadByte(IAP_ADDRESS+511) == 0xEE)
	{	
		productNum = (WORD)((IapReadByte(IAP_ADDRESS + 8) << 8) | IapReadByte(IAP_ADDRESS + 9));
	}
	else
	{
		productNum = 0;	
	}
}

void snapshot_save()
{
   	Delay(10);
	IapEraseSector(IAP_ADDRESS); //擦除EEPROM
	Delay(10);  
	IapProgramByte(IAP_ADDRESS + 8, (BYTE)(productNum>>8));
	IapProgramByte(IAP_ADDRESS + 9, (BYTE)productNum);
	IapProgramByte(IAP_ADDRESS+511, 0xEE); //写入标志位
	Delay(10);
}
