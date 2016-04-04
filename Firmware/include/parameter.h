/***************************************************************************/
// ����15�����������ϵͳ
// ģ�飺����ͷ�ļ�
// �ļ���parameter.h
// ���ߣ������D
// �汾��V0.01
// ���ڣ�2016��4��3��
// ���ܣ�������
// оƬ��STC12C5A60S2
// ���룺Keil uVision3 V3.90
/***************************************************************************/


#ifndef __PARAMETER_H__
#define __PARAMETER_H__

#include <reg52.h>

/***************************************************************************/
// ���Ŷ���
/***************************************************************************/
//����
sbit sensorStartPosi1 = P0^0; //��ʼλ�ô�����1
sbit sensorStartPosi2 = P0^1; //��ʼλ�ô�����2
sbit keyMotor1Forward = P0^2; //���1��ǰ�ź�
sbit keyMotor1Backward = P0^3; //���1����ź�
sbit keyMotor2Forward = P0^4; //���2��ǰ�ź�
sbit keyMotor2Backward= P0^5; //���2����ź�

//���
sbit ioMotor1Direction = P1^0; //���1����
sbit ioMmotor1PWM = P2^6; //���1����
sbit ioMmotor2Direction = P2^0; //���2����
sbit ioMmotor2PWM = P2^1; //���2����

sbit testOutput1 = P1^4;
sbit testOutput2 = P1^5;
sbit testOutput3 = P1^6;
sbit testOutput4 = P1^7;

/***************************************************************************/
// ��������
/***************************************************************************/

enum RunMode {MODEL_STOP, MODEL_RUN};  //����ģʽ 0��ֹͣ 1������
enum MotorStatus {MOTOR_STOP, MOTOR_FORWARD, MOTOR_BACKWARD};  //���״̬ 0��ֹͣ 1����ǰ 2�����
enum DisplayMode {DISPLAY_NO_FRESH, DISPLAY_RUN, DISPLAY_SETTING1, DISPLAY_SETTING2, DISPLAY_SETTING3, DISPLAY_SETTING4, DISPLAY_SETTING5, DISPLAY_SETTING6}; //��Ļ��ʾģʽ
enum SaveMode {SAVE_NO_SAVING, SAVE_SETTING, SAVE_SNAPSHOT}; //�Ƿ񱣴�EEPROM״̬λ  0�������� 1����������ֵ 2����������״̬

struct Motor //����ṹ��
{
	unsigned char position; //�����ǰλ�� 0~39
	enum MotorStatus status; //���״̬ 0��ֹͣ 1����ǰ 2�����
   	unsigned char isStartPosition; //����Ƿ��ڳ�ʼλ�ñ�־λ
	unsigned long stepPWMs; //������������
	unsigned long stepPassPWMs; //��������������
	unsigned long totalPWMs; //������������
	unsigned char currentStage; //��ǰ���������� 0~5
};


extern enum RunMode runMode; //����ģʽ
extern struct Motor motor1, motor2; //���
extern unsigned int motorRotationAngle[6][40]; //�����ת�� 6�����ݣ�ÿ��40��
extern unsigned char motorCurrentRatationGroup; //��ǰ������ת����
extern enum DisplayMode displayMode; //ˢ����Ļ��־λ 0 ��ˢ��
extern enum SaveMode saveMode; //����EEPROM״̬λ 

/***************************************************************************/
// ��������
/***************************************************************************/
void parameter_init();
unsigned char parameter_save();


#endif
