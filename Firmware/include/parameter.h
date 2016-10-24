/***************************************************************************/
// 程序：15型座垫机控制系统
// 模块：参数头文件
// 文件：parameter.h
// 作者：卜晓旸
// 版本：V0.01
// 日期：2016年4月3日
// 功能：主函数
// 芯片：STC12C5A60S2
// 编译：Keil uVision3 V3.90
/***************************************************************************/


#ifndef __PARAMETER_H__
#define __PARAMETER_H__

#include <reg52.h>

#define MAX_MOOD 2
#define MAX_STEP 600
//24 * MAX_STEP + 2
#define MOOD_STORAGE_SIZE 14402

//定义电机和继电器同时运行模式
#define RUN_MODE_SIMU

/***************************************************************************/
// 引脚定义
/***************************************************************************/
  //输入
sbit sensorStartPosi1 = P0^0; //电机1初始位置传感器
sbit sensorStartPosi2 = P0^1; //电机2初始位置传感器
sbit sensorStartPosi3 = P0^2; //电机3初始位置传感器
sbit sensorStartPosi4 = P0^3; //电机4初始位置传感器

sbit sensorEndPosi1 = P0^4; //电机1结束位置传感器
sbit sensorEndPosi2 = P0^5; //电机2结束位置传感器
sbit sensorEndPosi3 = P2^4; //电机3结束位置传感器
sbit sensorEndPosi4 = P2^5; //电机4结束位置传感器

sbit sensorRun1 = P2^0; //接近开关信号1
sbit sensorRun2 = P2^1; //接近开关信号2

sbit sensorRunMotor1 = P0^6; //电机运行接近开关信号1
sbit sensorRunMotor2 = P0^7; //电机运行接近开关信号1

//迪文屏
//P1^2 RXD2
//P1^3 TXD2

//存储
sbit storageSCL = P2^7; //at24c256 存储 SLC
sbit storageSDA = P2^6; //at24c256 存储 SDA


//输出
sbit ioMotor1Direction = P1^0; //电机1方向
sbit ioMotor2Direction = P1^1; //电机2方向
sbit ioMotor3Direction = P1^4; //电机3方向
sbit ioMotor4Direction = P1^5; //电机4方向
sbit ioMotor1PWM = P1^6; //电机1脉冲
sbit ioMotor2PWM = P1^7; //电机2脉冲
sbit ioMotor3PWM = P3^0; //电机3脉冲
sbit ioMotor4PWM = P3^1; //电机4脉冲	                                     
sbit ioRelays1SCK = P3^4;  //继电器输出595 SCK信号
sbit ioRelays1LAT = P3^3;  //继电器输出595 LAT信号
sbit ioRelays1DATA = P3^2;  //继电器输出595 DATA信号

sbit ioRelays2SCK = P3^7;  //继电器输出595二 SCK信号
sbit ioRelays2LAT = P3^6;  //继电器输出595二 LAT信号
sbit ioRelays2DATA = P3^5;  //继电器输出595二 DATA信号

sbit ioAllowMotorStart = P2^2;  //允许电机启动信号

//sbit testOutput2 = P2^3;



/***************************************************************************/
// 参数声明
/***************************************************************************/

enum RunMode {MODEL_STOP, MODEL_RUN};  //运行模式 0：停止 1：运行
enum MotorStatus {MOTOR_BACKWARD, MOTOR_FORWARD, MOTOR_STOP};  //电机状态 0：向前 1：向后 2：停止
enum DisplayMode {DISPLAY_NO_FRESH, DISPLAY_RUN, DISPLAY_MAX_POSITION, DIAPLAY_MIN_POSITION, DIAPLAY_SAVING}; //屏幕显示模式
enum SaveMode {SAVE_NO_SAVING, SAVE_SETTING_MOOD, SAVE_SETTING_STEP, SAVE_SETTING, SAVE_SNAPSHOT, READ_STEP, READ_STEP_SIM}; //是否保存EEPROM状态位  0：不保存 1：保存设置值 2：保存设置值并且读取设置 3：保存运行状态

struct Motor //电机结构体
{
	enum MotorStatus status; //电机状态 0：停止 1：向前 2：向后
	unsigned long stepPWMs; //单步总脉冲数
	unsigned long stepPassPWMs; //单步已走脉冲数
};

struct Setting //参数设置结构体
{
	unsigned char currentMood; //当前花样
	unsigned char totalMood; //总花样
	unsigned int currentStep; //当前步数
	unsigned int totalSteps; //总步数

	unsigned int motor1Steps; //电机1设置步数
	unsigned int motor2Steps; //电机2设置步数
	unsigned int motor3Steps; //电机3设置步数
	unsigned int motor4Steps; //电机4设置步数

	unsigned int moodStatus[8]; //电磁铁状态 
};



extern enum RunMode runMode; //运行模式
extern struct Motor motor1, motor2, motor3, motor4; //电机
extern enum DisplayMode displayMode; //刷新屏幕标志位 0 不刷新
extern enum SaveMode saveMode; //保存EEPROM状态位
extern struct Setting setting; //参数设置 
extern unsigned int productNum; //生产件数 

extern unsigned char activeSensorRun; //当前有效运行感应器
extern unsigned char sensorRunCheck;

/***************************************************************************/
// 函数声明
/***************************************************************************/
void parameter_init();
unsigned char parameter_save();
unsigned char parameter_save_mood();
unsigned char parameter_read();

void snapshot_init();
void snapshot_save();

#endif
