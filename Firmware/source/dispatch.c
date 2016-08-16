#include <reg52.h>
#include <parameter.h>
#include <intrins.h>
#include <timer.h>
#include <dispatch.h>


unsigned char ResetMotorDispatchSteps = 20; 



//电机回零状态机，每10毫秒调用一次，调用之前确保电机处于空闲状态。在ResetMotorDispatchSteps == 20 时设置ResetMotorDispatchSteps = 0 开始状态机
void ResetMotorDispatch(void)
{
	switch(ResetMotorDispatchSteps)
	{
		case 0:
			if( motor1.status == MOTOR_STOP && 
				motor2.status == MOTOR_STOP &&
				motor3.status == MOTOR_STOP &&
				motor4.status == MOTOR_STOP
			)
			{
				//启动电机1
				ioMotor1Direction = MOTOR_BACKWARD;
			   	motor1.status = MOTOR_BACKWARD;
				motor1.stepPWMs = 3000000;
				motor1.stepPassPWMs = 0;
				MotorGroup1Start();
				ResetMotorDispatchSteps ++;
			}
		break;
		case 1:
			if(sensorStartPosi1 == 0)
			{
				//关闭电机1
			   	MotorGroup1Stop();
				motor1.status = MOTOR_STOP;
				motor1.stepPWMs = 0;
				motor1.stepPassPWMs = 0;
				//启动电机2
				ioMotor2Direction = MOTOR_BACKWARD;
				motor2.status = MOTOR_BACKWARD;
				motor2.stepPWMs = 3000000;
				motor2.stepPassPWMs = 0;
				MotorGroup1Start();

				displayMode = DISPLAY_RUN;
				ResetMotorDispatchSteps ++;
			}
		break;
		case 2:
			if(sensorStartPosi2 == 0)
			{
				//关闭电机2
			   	MotorGroup1Stop();
				motor2.status = MOTOR_STOP;
				motor2.stepPWMs = 0;
				motor2.stepPassPWMs = 0;
				//启动电机3
				ioMotor3Direction = MOTOR_BACKWARD;
				motor3.status = MOTOR_BACKWARD;
				motor3.stepPWMs = 3000000;
				motor3.stepPassPWMs = 0;
				MotorGroup2Start();

				displayMode = DISPLAY_RUN;
				ResetMotorDispatchSteps ++;
			}
		break;
		case 3:
			if(sensorStartPosi3 == 0)
			{
				//关闭电机3
			   	MotorGroup2Stop();
				motor3.status = MOTOR_STOP;
				motor3.stepPWMs = 0;
				motor3.stepPassPWMs = 0;
				//启动电机4
				ioMotor4Direction = MOTOR_BACKWARD;
				motor4.status = MOTOR_BACKWARD;
				motor4.stepPWMs = 3000000;
				motor4.stepPassPWMs = 0;
				MotorGroup2Start();

				displayMode = DISPLAY_RUN;
				ResetMotorDispatchSteps ++;
			}	
		break;
		case 4:
			if(sensorStartPosi4 == 0)
			{
				//关闭电机4
			   	MotorGroup2Stop();
				motor4.status = MOTOR_STOP;
				motor4.stepPWMs = 0;
				motor4.stepPassPWMs = 0;

				displayMode = DISPLAY_RUN;
				ResetMotorDispatchSteps = 20;
			}	
		break;
		default:
			 _nop_();
	}
}

void OutPut595(void)
{
	unsigned char i, j;
	//1~64路继电器
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 16; j++)
		{
			ioRelays1SCK = 0;
			ioRelays1DATA  = setting.moodStatus[i] & 0x80;
			ioRelays1SCK = 1;			
			setting.moodStatus[i] <<= 1;
		}	
	}
	ioRelays1LAT = 0;
	ioRelays1LAT = 1;
	//65~128路继电器
	for(i = 4; i < 8; i++)
	{
		for(j = 0; j < 16; j++)
		{
			ioRelays2SCK = 0;
			ioRelays2DATA  = setting.moodStatus[i] & 0x80;
			ioRelays2SCK = 1;			
			setting.moodStatus[i] <<= 1;
		}	
	}
	ioRelays2LAT = 0;
	ioRelays2LAT = 1;
}
/*
控制74HC595输出数据
void Output(void)
{
    RCK = 0;
    RCK = 1;
}


写入一字节数据
void Write_Byte(unsigned char dat)
{
    unsigned char i = 0;
   
    for(i=0; i<8; i++)
    {
        SCK = 0;
        SI  = dat & 0x80;
        SCK = 1;
       
        dat <<= 1;      
    }
}
*/

