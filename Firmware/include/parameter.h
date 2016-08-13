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
sbit keyMotor2Copy = P0^4; //���2�����ź�

sbit sensorStartPosi3 = P0^5; //��ʼλ�ô�����3
sbit sensorStartPosi4 = P0^6; //��ʼλ�ô�����4
sbit keyMotor3Forward = P0^7; //���3��ǰ�ź�
sbit keyMotor3Backward = P2^0; //���3����ź�
sbit keyMotor4Copy = P2^1; //���4�����ź�

sbit sensorEndPosi1 = P2^2; //����λ�ô�����1
sbit sensorEndPosi2 = P2^3; //����λ�ô�����2
sbit sensorEndPosi3 = P2^4; //����λ�ô�����3
sbit sensorEndPosi4 = P2^5; //����λ�ô�����4

//������
//P1^2 RXD2
//P1^3 TXD2

//�洢
sbit storageSCL = P2^6; //eeprom�洢 SLC
sbit storageSDA = P2^7; //eeprom�洢 SDA

//���
sbit ioMotor1Direction = P1^4; //���1����
sbit ioMotor2Direction = P1^5; //���2����
sbit ioMotor3Direction = P1^6; //���3����
sbit ioMotor4Direction = P1^7; //���4����
sbit ioMotor1PWM = P3^4; //���1����
sbit ioMotor2PWM = P3^5; //���2����
sbit ioMotor3PWM = P3^6; //���3����
sbit ioMotor4PWM = P3^7; //���4����

sbit testOutput1 = P1^0;
sbit testOutput2 = P1^1;


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
	unsigned char currentStage; //��ǰ�������ݹ��� 0~5
};


extern enum RunMode runMode; //����ģʽ
extern struct Motor motor1, motor2, motor3, motor4; //���
extern unsigned int xdata motorRotationAngle1[6][40]; //�����ת��һ�� 6�������ݣ�ÿ����40��
extern unsigned int xdata motorRotationAngle2[6][40]; //�����ת�Ƕ��� 6�������ݣ�ÿ����40��
extern unsigned char motorCurrentRatationStage; //��ǰ������ת�ǹ���
extern unsigned char motorCurrentRatationGroup; //��ǰ������ת���� 1~2
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
