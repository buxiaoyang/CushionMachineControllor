/***************************************************************************/
// 程序：15型座垫机控制系统
// 模块：参数函数
// 文件：at24c256.c
// 作者：卜晓旸
// 版本：V0.01
// 日期：2016年8月13日
// 功能：主函数
// 芯片：STC12C5A60S2
// 编译：Keil uVision3 V3.90
/***************************************************************************/

#include <intrins.h>
#include <parameter.h>
#include <at24c256.h>


void Start(void)
{
	storageSDA=1;
	_nop_();_nop_();
	storageSCL=1;
	_nop_();_nop_();_nop_();_nop_();_nop_();
	storageSDA=0;
	_nop_();_nop_();_nop_();_nop_();_nop_();
	storageSCL=0;
}

void Stop(void)
{
	storageSDA=0;
	_nop_();
	storageSCL=1;
	_nop_();_nop_();_nop_();_nop_();_nop_();
	storageSDA=1;
	_nop_();_nop_();_nop_();_nop_();_nop_();
	storageSCL=0;
}

void Ack(void)
{
	storageSDA=0;
	_nop_();_nop_();_nop_();
	storageSCL=1;
	_nop_();_nop_();_nop_();_nop_();_nop_();
	storageSCL=0;
	_nop_();_nop_();
}

void NoAck(void)
{
	storageSDA=1;
	_nop_();_nop_();_nop_();
	storageSCL=1;
	_nop_();_nop_();_nop_();_nop_();_nop_();
	storageSCL=0;
	_nop_();_nop_();
}

void Send(unsigned char Data)
{ 
	unsigned char BitCounter=8;
	unsigned char temp;

	do {
		temp=Data;
		storageSCL=0;
		_nop_();_nop_();_nop_();_nop_();_nop_();
		if((temp&0x80)==0x80)
			storageSDA=1;
		else
			storageSDA=0;


		storageSCL=1;
		temp=Data<<1;
		Data=temp;
		BitCounter--;
	} while(BitCounter);
	
	storageSCL=0;
}

unsigned char Read(void)
{
	unsigned char temp=0;
	unsigned char temp1=0;
	unsigned char BitCounter=8;

	storageSDA=1;
	do {
		storageSCL=0;
		_nop_();_nop_();_nop_();_nop_();_nop_();
		storageSCL=1;
		_nop_();_nop_();_nop_();_nop_();_nop_();
		if(storageSDA)
			temp=temp|0x01;
		else
			temp=temp&0xfe;

		if(BitCounter-1){
			temp1=temp<<1;
			temp=temp1;
		}
		
		BitCounter--;
	} while(BitCounter);
	
	return(temp);
}

void mDelay(unsigned char j)
{
	unsigned int i;
	for(;j>0;j--){
	for(i=0;i<125;i++){
			;
		}
	}
}

void WrToROM(unsigned int Address, unsigned char Data)
{
	Start();
	Send(AddWr);
	Ack();
	Send(Address>>8);
	Ack();
	Send(Address);
	Ack();
	Send(Data);
	Ack();
	Stop();
	mDelay(5);
}

unsigned char RdFromROM(unsigned int Address)
{
	unsigned char i;
	Start();
	Send(AddWr);
	Ack();
	Send(Address>>8);
	Ack();
	Send(Address);
	Ack();
	Start();
	Send(AddRd);
	Ack();
	i=Read();
	storageSCL=0;
	NoAck();
	Stop();
	return i;
}


