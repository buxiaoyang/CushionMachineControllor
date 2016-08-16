#include <uart.h>
#include <reg52.h>
#include <intrins.h>
#include <parameter.h>
#include <dispatch.h>
#include <timer.h>

#define FOSC 11059200L      //System frequency
#define BAUD 115200         //UART baudrate

/*Define UART parity mode*/
#define NONE_PARITY     0   //None parity
#define ODD_PARITY      1   //Odd parity
#define EVEN_PARITY     2   //Even parity
#define MARK_PARITY     3   //Mark parity
#define SPACE_PARITY    4   //Space parity

#define PARITYBIT NONE_PARITY   //Testing even parity

/*Declare SFR associated with the UART2 */
sfr AUXR  = 0x8e;           //Auxiliary register
sfr S2CON = 0x9a;           //UART2 control register
sfr S2BUF = 0x9b;           //UART2 data buffer
sfr BRT   = 0x9c;           //Baudrate generator
sfr IE2   = 0xaf;           //Interrupt control 2

#define S2RI  0x01          //S2CON.0
#define S2TI  0x02          //S2CON.1
#define S2RB8 0x04          //S2CON.2
#define S2TB8 0x08          //S2CON.3

bit busy;
bit uartReceiveOK = 0;

BYTE xdata uartBuffer[15] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

BYTE receiveSteps = 0; 	// 0: 没有数据接收
						// 1: 接收到5A
						// 2：接收到A5
						// 3：接收到数据长度
						// 4: 接收数据
						// 5：确定指令动作
BYTE dataIndex = 0;
BYTE dataLength = 0;
  
void SendData(BYTE dat);
void ReceiveData(BYTE dat);
void anyData();

void uart_init()
{
#if (PARITYBIT == NONE_PARITY)
    S2CON = 0x50;           //8-bit variable UART
#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    S2CON = 0xda;           //9-bit variable UART, parity bit initial to 1
#elif (PARITYBIT == SPACE_PARITY)
    S2CON = 0xd2;           //9-bit variable UART, parity bit initial to 0
#endif

    BRT = -(FOSC/32/BAUD);  //Set auto-reload vaule of baudrate generator
    AUXR |= 0x14;            //Baudrate generator work in 1T mode
    IE2 = 0x01;             //Enable UART2 interrupt
    EA = 1;                 //Open master interrupt switch

    //SendString("STC12C5A60S2\r\nUart2 Test !\r\n");
    //while(1);
}

/*----------------------------
UART2 interrupt service routine
----------------------------*/
void Uart2() interrupt 8 using 1
{
    if (S2CON & S2RI)
    {
        S2CON &= ~S2RI;     //Clear receive interrupt flag
        ReceiveData(S2BUF);
		//P2 = S2BUF;         //P0 show UART data
        //P2 = (S2CON & S2RB8);//P2.2 show parity bit
    }
    if (S2CON & S2TI)
    {
        S2CON &= ~S2TI;     //Clear transmit interrupt flag
        busy = 0;           //Clear transmit busy flag
    }
}

/*----------------------------
Send a byte data to UART
Input: dat (data to be sent)
Output:None
----------------------------*/
void SendData(BYTE dat)
{
    while (busy);           //Wait for the completion of the previous data is sent
    ACC = dat;              //Calculate the even parity bit P (PSW.0)
    if (P)                  //Set the parity bit according to P
    {
#if (PARITYBIT == ODD_PARITY)
        S2CON &= ~S2TB8;    //Set parity bit to 0
#elif (PARITYBIT == EVEN_PARITY)
        S2CON |= S2TB8;     //Set parity bit to 1
#endif
    }
    else
    {
#if (PARITYBIT == ODD_PARITY)
        S2CON |= S2TB8;     //Set parity bit to 1
#elif (PARITYBIT == EVEN_PARITY)
        S2CON &= ~S2TB8;    //Set parity bit to 0
#endif
    }
    busy = 1;
    S2BUF = ACC;            //Send data to UART2 buffer
}

void SendDataToScreen(WORD address, WORD dat)
{
	SendData(0x5A);
	SendData(0xA5);
	SendData(0x05);
	SendData(0x82);
	SendData(address>>8);
	SendData(address);
	SendData(dat>>8);
	SendData(dat);
}

void SendLongDataToScreen(WORD address, unsigned long dat)
{
	SendData(0x5A);
	SendData(0xA5);
	SendData(0x07);
	SendData(0x82);
	SendData(address>>8);
	SendData(address);
	SendData(dat>>24);
	SendData(dat>>16);
	SendData(dat>>8);
	SendData(dat);
}

void ChangeScreenPage(WORD page)
{
	SendData(0x5A);
	SendData(0xA5);
	SendData(0x04);
	SendData(0x80);
	SendData(0x03);
	SendData(page>>8);
	SendData(page);
}

void ReceiveData(BYTE dat)
{
	// 0: 接收到5A
	// 1：接收到A5
	// 2：接收到数据长度
	// 3: 接收数据
	// 4：确定指令动作
	switch(receiveSteps)
	{
		case 0:
			if(dat == 0x5A)
			{
				receiveSteps ++;
			}
		break;
		case 1:
			if(dat == 0xA5)
			{
				receiveSteps ++;
			}
			else
			{
			  	receiveSteps = 0;
			}
		break;
		case 2:
			dataLength = dat;
			dataIndex = 0;
			receiveSteps ++;
		break;
		case 3:
			uartBuffer[dataIndex] = dat;
			dataIndex ++;
			if(dataIndex >= dataLength)
			{
				anyData();
				receiveSteps = 0;
			}
		break;
		default:
			_nop_();
			receiveSteps = 0;
	}
}

void anyData()
{
	WORD dat = ((uartBuffer[4]<<8) | uartBuffer[5]);
	
	/*
	if(uartBuffer[2] == 0x07)//回零按钮
	{
		if(ResetMotorDispatchSteps == 20)
		{
			ResetMotorDispatchSteps = 0;	
		}
	}
	else 
	*/
	if(uartBuffer[2] == 0x02) //当前花样号
	{
		if(dat > MAX_MOOD)
		{
			dat = MAX_MOOD;
		}
		setting.currentMood = (BYTE)dat;
		saveMode = SAVE_SETTING_MOOD;
		displayMode = DISPLAY_RUN;
	}
	else if(uartBuffer[2] == 0x0A) //总花样数
	{
		setting.totalMood = (BYTE)dat;
		displayMode = DISPLAY_RUN;
	}
	else if(uartBuffer[2] == 0x03) //当前步数
	{
		if(dat > setting.totalSteps)
		{
			dat = setting.totalSteps;
		}
		setting.currentStep = dat;
		saveMode = SAVE_SETTING_STEP;
		displayMode = DISPLAY_RUN;
	}
	else if(uartBuffer[2] == 0x04) //总步数
	{
		if(dat > MAX_STEP)
		{
			dat = MAX_STEP;
		}
		setting.totalSteps = dat;
		//saveMode = SAVE_SETTING;
		displayMode = DISPLAY_RUN;
	}
	else if(uartBuffer[2] == 0x05) //电机步数1
	{
		setting.motor1Steps = dat;
		//saveMode = SAVE_SETTING;
		//displayMode = DISPLAY_RUN;
	}
	else if(uartBuffer[2] == 0x06) //电机步数2
	{
		setting.motor2Steps = dat;
		//saveMode = SAVE_SETTING;
		//displayMode = DISPLAY_RUN;
	}
	else if(uartBuffer[2] == 0x07) //电机步数3
	{
		setting.motor3Steps = dat;
		//saveMode = SAVE_SETTING;
		//displayMode = DISPLAY_RUN;
	}
	else if(uartBuffer[2] == 0x08) //电机步数4
	{
		setting.motor4Steps = dat;
		//saveMode = SAVE_SETTING;
		//displayMode = DISPLAY_RUN;
	}
	else if(uartBuffer[2] == 0x09) //电磁铁1~16
	{
		setting.moodStatus[0] = dat;
		//saveMode = SAVE_SETTING;
		//displayMode = DISPLAY_RUN;
	}
	else if(uartBuffer[2] == 0x10) //电磁铁17~32
	{	
		setting.moodStatus[1] = dat;
		//saveMode = SAVE_SETTING;
		//displayMode = DISPLAY_RUN;
	}
	else if(uartBuffer[2] == 0x11) //电磁铁33~48
	{
		setting.moodStatus[2] = dat;
		//saveMode = SAVE_SETTING;
		//displayMode = DISPLAY_RUN;
	}
	else if(uartBuffer[2] == 0x12) //电磁铁49~64
	{
		setting.moodStatus[3] = dat;
		//saveMode = SAVE_SETTING;
		//displayMode = DISPLAY_RUN;
	}
	else if(uartBuffer[2] == 0x13) //电磁铁65~80
	{
		setting.moodStatus[4] = dat;
		//saveMode = SAVE_SETTING;
		//displayMode = DISPLAY_RUN;
	}
	else if(uartBuffer[2] == 0x14) //电磁铁81~96
	{
		setting.moodStatus[5] = dat;
		//saveMode = SAVE_SETTING;
		//displayMode = DISPLAY_RUN;
	}
	else if(uartBuffer[2] == 0x15) //电磁铁97~112
	{
		setting.moodStatus[6] = dat;
		//saveMode = SAVE_SETTING;
		//displayMode = DISPLAY_RUN;
	}
	else if(uartBuffer[2] == 0x16) //电磁铁113~128
	{
		setting.moodStatus[7] = dat;
		//saveMode = SAVE_SETTING;
		//displayMode = DISPLAY_RUN;
	}
	else if(uartBuffer[2] == 0x17) //运行按钮
	{
		testOutput1 = !testOutput1;
		runMode = MODEL_RUN;
		displayMode = DISPLAY_RUN;
	}
	else if(uartBuffer[2] == 0x18) //停止按钮
	{
		runMode = MODEL_STOP;
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

		setting.currentStep = 1;
		saveMode = SAVE_SETTING_STEP;
		displayMode = DISPLAY_RUN;
	}
	else if(uartBuffer[2] == 0x19) //选花按钮
	{
		if(displayMode == DISPLAY_NO_FRESH && saveMode == SAVE_NO_SAVING)
		{
			setting.currentMood ++;
			if(setting.currentMood > MAX_MOOD)
			{
				setting.currentMood = 1;
			}
			saveMode = SAVE_SETTING_MOOD;
			displayMode = DISPLAY_RUN;
		}
	}
	else if(uartBuffer[2] == 0x20) //外卡按钮
	{

	}
	else if(uartBuffer[2] == 0x21) //内卡复制到外卡按钮
	{

	}
	else if(uartBuffer[2] == 0x22) //外卡复制到内卡按钮
	{

	}
	else if(uartBuffer[2] == 0x23) //编程按钮
	{
		runMode = MODEL_STOP;
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

		displayMode = DISPLAY_RUN;
	}
	else if(uartBuffer[2] == 0x24) //上一花样按钮
	{
		if(displayMode == DISPLAY_NO_FRESH)
		{
			setting.currentMood --;
			if(setting.currentMood > MAX_MOOD || setting.currentMood == 0)
			{
				setting.currentMood = 1;
			}
			saveMode = SAVE_SETTING_MOOD;
			displayMode = DISPLAY_RUN;
		}
	}
	else if(uartBuffer[2] == 0x25) //下一花样按钮
	{
		if(displayMode == DISPLAY_NO_FRESH)
		{
			setting.currentMood ++;
			if(setting.currentMood > MAX_MOOD)
			{
				setting.currentMood = MAX_MOOD;
			}
			saveMode = SAVE_SETTING_MOOD;
			displayMode = DISPLAY_RUN;
		}
	}
	else if(uartBuffer[2] == 0x26) //上一步按钮
	{
		if(displayMode == DISPLAY_NO_FRESH)
		{
			setting.currentStep --;
			if(setting.currentStep > setting.totalSteps || setting.currentStep == 0)
			{
				setting.currentStep = 1;
			}
			saveMode = SAVE_SETTING_STEP;
			displayMode = DISPLAY_RUN;
		}
	}
	else if(uartBuffer[2] == 0x27) //下一步按钮
	{
		if(displayMode == DISPLAY_NO_FRESH)
		{
			setting.currentStep ++;
			if(setting.currentStep > setting.totalSteps)
			{
				setting.currentStep = setting.totalSteps;
			}
			saveMode = SAVE_SETTING_STEP;
			displayMode = DISPLAY_RUN;
		}
	}
	else if(uartBuffer[2] == 0x28) //保存按钮
	{
		saveMode = SAVE_SETTING;
		displayMode = DISPLAY_RUN;
	}
	else if(uartBuffer[2] == 0x29) //清除按钮
	{
		setting.moodStatus[0] = 0x0000;
		setting.moodStatus[1] = 0x0000;
		setting.moodStatus[2] = 0x0000;
		setting.moodStatus[3] = 0x0000;
		setting.moodStatus[4] = 0x0000;
		setting.moodStatus[5] = 0x0000;
		setting.moodStatus[6] = 0x0000;
		setting.moodStatus[7] = 0x0000;
		//saveMode = SAVE_SETTING;
		displayMode = DISPLAY_RUN;
	}
	else if(uartBuffer[2] == 0x30) //全选按钮
	{
		setting.moodStatus[0] = 0xFFFF;
		setting.moodStatus[1] = 0xFFFF;
		setting.moodStatus[2] = 0xFFFF;
		setting.moodStatus[3] = 0xFFFF;
		setting.moodStatus[4] = 0xFFFF;
		setting.moodStatus[5] = 0xFFFF;
		setting.moodStatus[6] = 0xFFFF;
		setting.moodStatus[7] = 0xFFFF;
		//saveMode = SAVE_SETTING;
		displayMode = DISPLAY_RUN;
	}
	else if(uartBuffer[2] == 0x31) //电机回零
	{
		if(ResetMotorDispatchSteps == 20)
		{
			ResetMotorDispatchSteps = 0;	
		}
	}
	else if(uartBuffer[2] == 0x32) //返回按钮
	{
		
	}
	uartReceiveOK = 1;	
}


void refreshDisplay()
{
	SendDataToScreen(0x000F, runMode);

	SendDataToScreen(0x0001, productNum);
	SendDataToScreen(0x0002, setting.currentMood);
	SendDataToScreen(0x000A, setting.totalMood);
	SendDataToScreen(0x0003, setting.currentStep);
	SendDataToScreen(0x0004, setting.totalSteps);
	SendDataToScreen(0x0005, setting.motor1Steps);
	SendDataToScreen(0x0006, setting.motor2Steps);
	SendDataToScreen(0x0007, setting.motor3Steps);
	SendDataToScreen(0x0008, setting.motor4Steps);
	SendDataToScreen(0x0009, setting.moodStatus[0]);
	SendDataToScreen(0x0010, setting.moodStatus[1]);
	SendDataToScreen(0x0011, setting.moodStatus[2]);
	SendDataToScreen(0x0012, setting.moodStatus[3]);
	SendDataToScreen(0x0013, setting.moodStatus[4]);
	SendDataToScreen(0x0014, setting.moodStatus[5]);
	SendDataToScreen(0x0015, setting.moodStatus[6]);
	SendDataToScreen(0x0016, setting.moodStatus[7]);

}