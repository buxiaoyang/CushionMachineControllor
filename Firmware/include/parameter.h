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

/***************************************************************************/
// 引脚定义
/***************************************************************************/
//输入
sbit sensorStartPosi1 = P0^0; //初始位置传感器1
sbit sensorStartPosi2 = P0^1; //初始位置传感器2
sbit keyMotor1Forward = P0^2; //电机1向前信号
sbit keyMotor1Backward = P0^3; //电机1向后信号
sbit keyMotor2Copy = P0^4; //电机2跟随信号

sbit sensorStartPosi3 = P0^5; //初始位置传感器3
sbit sensorStartPosi4 = P0^6; //初始位置传感器4
sbit keyMotor3Forward = P0^7; //电机3前进信号
sbit keyMotor3Backward = P2^0; //电机3后退信号
sbit keyMotor4Copy = P2^1; //电机4跟随信号

sbit sensorEndPosi1 = P2^2; //结束位置传感器1
sbit sensorEndPosi2 = P2^3; //结束位置传感器2
sbit sensorEndPosi3 = P2^4; //结束位置传感器3
sbit sensorEndPosi4 = P2^5; //结束位置传感器4

//迪文屏
//P1^2 RXD2
//P1^3 TXD2

//输出
sbit ioMotor1Direction = P1^4; //电机1方向
sbit ioMotor2Direction = P1^5; //电机2方向
sbit ioMotor3Direction = P1^6; //电机3方向
sbit ioMotor4Direction = P1^7; //电机4方向
sbit ioMotor1PWM = P3^4; //电机1脉冲
sbit ioMotor2PWM = P3^5; //电机2脉冲
sbit ioMotor3PWM = P3^6; //电机3脉冲
sbit ioMotor4PWM = P3^7; //电机4脉冲

sbit testOutput1 = P1^0;
sbit testOutput2 = P1^1;


/***************************************************************************/
// 参数声明
/***************************************************************************/

enum RunMode {MODEL_STOP, MODEL_RUN};  //运行模式 0：停止 1：运行
enum MotorStatus {MOTOR_BACKWARD, MOTOR_FORWARD, MOTOR_STOP};  //电机状态 0：向前 1：向后 2：停止
enum DisplayMode {DISPLAY_NO_FRESH, DISPLAY_RUN, DISPLAY_MAX_POSITION, DIAPLAY_MIN_POSITION,DIAPLAY_MOTOR_FORWARD, DIAPLAY_MOTOR_BACKWARD,DIAPLAY_MOTOR2_OPEATION, DISPLAY_SETTING1, DISPLAY_SETTING2, DISPLAY_SETTING3, DISPLAY_SETTING4, DISPLAY_SETTING5, DISPLAY_SETTING6}; //屏幕显示模式
enum SaveMode {SAVE_NO_SAVING, SAVE_SETTING, SAVE_SNAPSHOT}; //是否保存EEPROM状态位  0：不保存 1：保存设置值 2：保存运行状态

struct Motor //电机结构体
{
	unsigned char position; //电机当前位置 0~39
	enum MotorStatus status; //电机状态 0：停止 1：向前 2：向后
   	unsigned char isStartPosition; //电机是否在初始位置标志位
	unsigned long stepPWMs; //单步总脉冲数
	unsigned long stepPassPWMs; //单步已走脉冲数
	unsigned long totalPWMs; //运行总脉冲数
	unsigned char currentStage; //当前运行数据组 0~5
};


extern enum RunMode runMode; //运行模式
extern struct Motor motor1, motor2; //电机
extern unsigned int motorRotationAngle[6][40]; //电机旋转角 6组数据，每组40个
extern unsigned char motorCurrentRatationGroup; //当前设置旋转角组
extern enum DisplayMode displayMode; //刷新屏幕标志位 0 不刷新
extern enum SaveMode saveMode; //保存EEPROM状态位 

/***************************************************************************/
// 函数声明
/***************************************************************************/
void parameter_init();
unsigned char parameter_save();

void snapshot_init();
void snapshot_save();

#endif
