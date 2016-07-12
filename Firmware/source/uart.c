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
	

	if(uartBuffer[2] == 0x07)//回零按钮
	{
		if(ResetMotorDispatchSteps == 20)
		{
			ResetMotorDispatchSteps = 0;	
		}
	}
	else if(uartBuffer[2] == 0x08) //运行按钮
	{
		runMode = MODEL_RUN;
		displayMode = DISPLAY_RUN;
	}
	else if(uartBuffer[2] == 0x0A) //停止按钮
	{
		runMode = MODEL_STOP;
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
		displayMode = DISPLAY_RUN;
	}
	else if(uartBuffer[2] == 0x0C) //前进按钮
	{
		if(runMode == MODEL_RUN)
		{
			motor1Forward();
		}
	}
	else if(uartBuffer[2] == 0x0E) //后退按钮
	{
		if(runMode == MODEL_RUN)
		{
			motor1Backward();
		}
	}
	else if(uartBuffer[2] == 0x10)	//跟随按钮
	{
		if(runMode == MODEL_RUN)
		{
			motor2Copy();
		}
	}
	else if(uartBuffer[2] == 0x1E) //保存按钮
	{
		saveMode = SAVE_SETTING;
	}
	else if(uartBuffer[2] == 0x20) //返回按钮
	{

	}
	else if(uartBuffer[2] >= 0x12 && uartBuffer[2] <= 0x1C) //过程设置按钮
	{
		motorCurrentRatationStage = (uartBuffer[2] - 0x12)>>1;
		displayMode = motorCurrentRatationStage + DISPLAY_SETTING1; //刷新显示
	}
	else if(uartBuffer[2] >= 0x2E && uartBuffer[2] <= 0x7C) //过程设置值
	{
		motorRotationAngle1[motorCurrentRatationStage][(uartBuffer[2] - 0x2E)>>1] = dat;
	}

	uartReceiveOK = 1;	
}

void refreshDisplaySetting()
{
	BYTE i;
	//设置按钮显示非选中
	SendDataToScreen(0x0022, 0);
	SendDataToScreen(0x0024, 0);
	SendDataToScreen(0x0026, 0);
	SendDataToScreen(0x0028, 0);
	SendDataToScreen(0x002A, 0);
	SendDataToScreen(0x002C, 0);
	//设置按钮显示当前选中
	SendDataToScreen((motorCurrentRatationStage<<1)+0x22, 1);
	//当组设置值
	for(i=0; i < 40; i++)
	{
		SendDataToScreen(0x002E + (i<<1) ,motorRotationAngle1[motorCurrentRatationStage][i]);
	   	
	}
}

void refreshDisplayRunning()
{
	SendDataToScreen(0x0000, runMode);
	SendDataToScreen(0x0001, motor1.currentStage + 1);
	SendDataToScreen(0x0002, motor2.currentStage + 1);
	SendLongDataToScreen(0x0003, motor1.position +1);
	SendLongDataToScreen(0x0005, motor2.position +1);
}