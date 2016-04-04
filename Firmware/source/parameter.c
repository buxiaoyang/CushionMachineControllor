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
//struct Motor motor1, motor2; //电机
unsigned int motorRotationAngle[6][40]; //电机旋转角 6组数据，每组40个
unsigned char motorCurrentRatationGroup; //当前设置旋转角组
enum DisplayMode displayMode; //刷新屏幕标志位 0 不刷新

/***************************************************************************/
// 读取参数
// 参数：无
// 返回值：0失败 1成功	
/***************************************************************************/
unsigned char parameter_read()
{
	unsigned char result = 0,i,j,address;
	delay_ms(10); 
	if(IapReadByte(IAP_ADDRESS+511) == 0xEE)
	{	
		for(i=0; i < 6; i++)
		{
			for(j=0; j < 40; j++)
			{
				address = i * 80 + (j<<1);
				motorRotationAngle[i][j] = ((IapReadByte(IAP_ADDRESS + address) << 8) | IapReadByte(IAP_ADDRESS + address + 1));
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
	WORD i;

    testOutput1 = 0;
    Delay(10);                      //Delay
    IapEraseSector(IAP_ADDRESS);    //Erase current sector
    for (i=0; i<512; i++)           //Check whether all sector data is FF
    {
        if (IapReadByte(IAP_ADDRESS+i) != 0xff)
            goto Error;             //If error, break
    }
    testOutput2 = 0;
    Delay(10);                      //Delay
    for (i=0; i<512; i++)           //Program 512 bytes data into data flash
    {
        IapProgramByte(IAP_ADDRESS+i, (BYTE)i);
    }
   	testOutput3 = 0;
    Delay(10);                      //Delay
    for (i=0; i<512; i++)           //Verify 512 bytes data
    {
        if (IapReadByte(IAP_ADDRESS+i) != (BYTE)i)
            goto Error;             //If error, break
    }
    testOutput4 = 0;
    while (1);
Error:
    P1 &= 0x7f;                     //0xxx,xxxx IAP operation fail
    while (1);

	/*
	unsigned char i,j;
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
	*/
}

/***************************************************************************/
// 参数保存到eeprom
// 参数：无
// 返回值：0失败 1成功	
/***************************************************************************/
unsigned char parameter_save()
{
	unsigned char i,j,address;
    delay_ms(10);                      //Delay
	IapEraseSector(IAP_ADDRESS); //擦除EEPROM
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
	return 1;
}


