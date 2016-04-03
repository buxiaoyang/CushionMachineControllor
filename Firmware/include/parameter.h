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
sbit keyMotor2Operation = P0^4; //电机2动作信号

//输出
sbit ioMotor1Direction = P1^0; //电机1方向
sbit ioMmotor1PWM = P2^6; //电机1脉冲
sbit ioMmotor2Direction = P2^0; //电机2方向
sbit ioMmotor2PWM = P2^1; //电机2脉冲

sbit testOut = P3^7;

/***************************************************************************/
// 参数声明
/***************************************************************************/
void parameter_init();
unsigned char parameter_save();


extern unsigned char runMode; //运行模式 0：停止 1：运行
extern unsigned char motor1Position; //电机1当前位置 0~39
extern unsigned char motor2Position; //电机2当前位置 0~39
extern unsigned char motor1Direction; //电机1运行方向
extern unsigned char motor2Direction; //电机2运行方向
extern unsigned char isStartPosition1; //电机1是否在初始位置标志位
extern unsigned char isStartPosition2; //电机2是否在初始位置标志位
extern unsigned long stepTotalPWMs; //单步总脉冲数
extern unsigned long stepRemainPWMs; //单步剩余脉冲数
extern unsigned long totalPWMs; //运行总脉冲数
extern unsigned int motorRotationAngle[6][40]; //电机旋转角 6组数据，每组40个
extern unsigned int currentStage; //当前运行数据组0~5
extern unsigned char refreshDisplay; //刷新屏幕标志位 0 不刷新 1刷新

#endif
