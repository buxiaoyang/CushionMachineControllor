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
sbit sensorEndPosi1 = P0^6; //����λ�ô�����1
sbit sensorEndPosi2 = P0^7; //����λ�ô�����2

//���
sbit ioMotor1Direction = P1^0; //���1����
sbit ioMotor1PWM = P1^1; //���1����
sbit ioMotor2Direction = P2^6; //���2����
sbit ioMotor2PWM = P2^5; //���2����

sbit testOutput1 = P1^4;
sbit testOutput2 = P1^5;
sbit testOutput3 = P1^6;
sbit testOutput4 = P1^7;

/***************************************************************************/
// ��������
/***************************************************************************/

enum RunMode {MODEL_STOP, MODEL_RUN};  //����ģʽ 0��ֹͣ 1������
enum MotorStatus {MOTOR_BACKWARD, MOTOR_FORWARD, MOTOR_STOP};  //���״̬ 0����ǰ 1����� 2��ֹͣ
enum DisplayMode {DISPLAY_NO_FRESH, DISPLAY_RUN, DISPLAY_MAX_POSITION, DIAPLAY_MIN_POSITION,DIAPLAY_MOTOR_FORWARD, DIAPLAY_MOTOR_BACKWARD,DIAPLAY_MOTOR2_OPEATION, DISPLAY_SETTING1, DISPLAY_SETTING2, DISPLAY_SETTING3, DISPLAY_SETTING4, DISPLAY_SETTING5, DISPLAY_SETTING6}; //��Ļ��ʾģʽ
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

void snapshot_init();
void snapshot_save();

#endif
