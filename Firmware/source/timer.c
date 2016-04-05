#include <timer.h>
#include <reg52.h>
#include <intrins.h>
#include <parameter.h>
#include <key.h>

#define TIMER_INIT 100


/* define SFR */
sfr AUXR = 0x8e;                    //Auxiliary register

unsigned int timerInit1 = TIMER_INIT, timerInit2 = TIMER_INIT;

unsigned int code timerStep[259] = {  0,  153,  771,  1104,  1379,  1589,  1736,  1824,  1862,  1862,  1830,  1777,  1710,
  1635,  1555,  1473,  1393,  1314,  1239,  1168,  1101,  1037,  979,  923,  871,
  824,  780,  738,  699,  664,  630,  599,  570,  543,  518,  494,  472,  452,  432,
  413,  397,  380,  365,  351,  337,  324,  313,  300,  291,  279,  270,  261,  252,
  244,  236,  228,  221,  214,  207,  201,  195,  190,  184,  178,  174,  168,  164,
  160,  155,
  160,  155,  152,  147,  143,  140,  136,  133,  129,  127,  123,  120,  118,  114,
  113,  109,  107,  105,  102,  100,  98,  95,  94,  92,  89,  88,  86,  84,  83,
  81,  79,  77,  77,  74,  73,  72,  70,  69,  68,  66,  65,  64,  63,  62,  60,  60,
  58,  57,  56,  56,  54,  53,  52,  52,  50,  50,  49,  48,  47,  46,  46,  45,  44,
  43,  43,  41,  42,  40,  40,  39,  39,  37,  38,  36,  36,  36,  35,  34,  34,  33,
  33,  32,  31,  32,  30,  30,  30,  29,  29,  28,  28,  27,  27,  26,  26,  26,  25,
  25,  24,  24,  23,  23,  23,  23,  22,  21,  21,  21,  21,  20,  20,  19,  20,  18,
  19,  18,  18,  18,  17,  17,  16,  17,  16,  15,  16,  15,  15,  14,  14,  14,  14,
  14,  13,  13,  12,  12,  13,  11,  12,  11,  11,  11,  11,  10,  10,  10,  9,  10,
  9,  8,  9,  8,  8,  8,  8,  7,  8,  6,  7,  7,  6,  6,  6,  5,  6,  5,  5,  5,  4,
  5,  4,  3,  4,  4,  3,  3,  3,  2,  2,  3,  2,  1,  2,  1,  1,  1,  1,  0,  0,  1};

//-----------------------------------------------

/* Timer0 interrupt routine */
void tm0_isr() interrupt 1 using 1
{
	unsigned long stepRemain = motor1.stepPWMs -  motor1.stepPassPWMs;
	if(motor1.stepPassPWMs < 259)
	{
		timerInit1 += timerStep[motor1.stepPassPWMs];
	}
	if(stepRemain < 259)
	{
		timerInit1 -= timerStep[stepRemain];
	}
	motor1.stepPassPWMs++;
	if(motor1.stepPassPWMs >= motor1.stepPWMs)
	{
	    TR0 = 0;
		timerInit1 = TIMER_INIT;
		motor1.status = MOTOR_STOP;
		motor1.stepPWMs = 0;
		motor1.stepPassPWMs = 0;
	}
	TL0 = timerInit1;		//设置定时初值
	TH0 = timerInit1>>8;		//设置定时初值
	ioMotor1PWM = !ioMotor1PWM;
}

/* Timer1 interrupt routine */
void tm1_isr() interrupt 3 using 1
{
	unsigned long stepRemain = motor2.stepPWMs -  motor2.stepPassPWMs;
	if(motor2.stepPassPWMs < 259)
	{
		timerInit2 += timerStep[motor2.stepPassPWMs];
	}
	if(stepRemain < 259)
	{
		timerInit2 -= timerStep[stepRemain];
	}
	motor2.stepPassPWMs++;
	if(motor2.stepPassPWMs >= motor2.stepPWMs)
	{
	    TR1 = 0;
		timerInit2 = TIMER_INIT;
		motor2.status = MOTOR_STOP;
		motor2.stepPWMs = 0;
		motor2.stepPassPWMs = 0;
	}
	TL1 = timerInit2;		//设置定时初值
	TH1 = timerInit2>>8;		//设置定时初值
	ioMotor2PWM = !ioMotor2PWM;
}

//-----------------------------------------------


void Timer0Init(void)		
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = TIMER_INIT;		//设置定时初值
	TH0 = TIMER_INIT>>8;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 0;		//定时器0开始计时
	ET0 = 0;                        //enable timer0 interrupt
	EA = 1;                         //open global interrupt switch
}

void Motor1Start(void)		
{
	TL0 = TIMER_INIT;		//设置定时初值
	TH0 = TIMER_INIT>>8;		//设置定时初值
	timerInit1 = TIMER_INIT;
	ET0 = 1;
	TR0 = 1;                       
}

void Motor1Stop(void)		
{
	ET0 = 0;
	TR0 = 0;                       
}

void Timer1Init(void)		
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x10;		//设置定时器模式
	TL1 = TIMER_INIT;		//设置定时初值
	TH1 = TIMER_INIT>>8;		//设置定时初值
	TF1 = 0;		//清除TF0标志
	TR1 = 0;		//定时器0开始计时
	ET1 = 0;                        //enable timer1 interrupt
	EA = 1;                         //open global interrupt switch
}

void Motor2Start(void)		
{
	TL1 = TIMER_INIT;		//设置定时初值
	TH1 = TIMER_INIT>>8;		//设置定时初值
	timerInit2 = TIMER_INIT;
	ET1 = 1;
	TR1 = 1;                       
}

void Motor2Stop(void)		
{
	TR1 = 0;
	ET1 = 0;                       
}