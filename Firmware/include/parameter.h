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

#define MAX_MOOD 2
#define MAX_STEP 600
//24 * MAX_STEP + 2
#define MOOD_STORAGE_SIZE 14402

//�������ͼ̵���ͬʱ����ģʽ
//#define RUN_MODE_SIMU

/***************************************************************************/
// ���Ŷ���
/***************************************************************************/
  //����
sbit sensorStartPosi1 = P0^0; //���1��ʼλ�ô�����
sbit sensorStartPosi2 = P0^1; //���2��ʼλ�ô�����
sbit sensorStartPosi3 = P0^2; //���3��ʼλ�ô�����
sbit sensorStartPosi4 = P0^3; //���4��ʼλ�ô�����

sbit sensorEndPosi1 = P0^4; //���1����λ�ô�����
sbit sensorEndPosi2 = P0^5; //���2����λ�ô�����
sbit sensorEndPosi3 = P2^4; //���3����λ�ô�����
sbit sensorEndPosi4 = P2^5; //���4����λ�ô�����

sbit sensorRun1 = P2^0; //�ӽ������ź�1
sbit sensorRun2 = P2^1; //�ӽ������ź�2

sbit sensorRunMotor1 = P0^6; //������нӽ������ź�1
sbit sensorRunMotor2 = P0^7; //������нӽ������ź�1

//������
//P1^2 RXD2
//P1^3 TXD2

//�洢
sbit storageSCL = P2^7; //at24c256 �洢 SLC
sbit storageSDA = P2^6; //at24c256 �洢 SDA


//���
sbit ioMotor1Direction = P1^0; //���1����
sbit ioMotor2Direction = P1^1; //���2����
sbit ioMotor3Direction = P1^4; //���3����
sbit ioMotor4Direction = P1^5; //���4����
sbit ioMotor1PWM = P1^6; //���1����
sbit ioMotor2PWM = P1^7; //���2����
sbit ioMotor3PWM = P3^0; //���3����
sbit ioMotor4PWM = P3^1; //���4����	                                     
sbit ioRelays1SCK = P3^4;  //�̵������595 SCK�ź�
sbit ioRelays1LAT = P3^3;  //�̵������595 LAT�ź�
sbit ioRelays1DATA = P3^2;  //�̵������595 DATA�ź�

sbit ioRelays2SCK = P3^7;  //�̵������595�� SCK�ź�
sbit ioRelays2LAT = P3^6;  //�̵������595�� LAT�ź�
sbit ioRelays2DATA = P3^5;  //�̵������595�� DATA�ź�

sbit ioAllowMotorStart = P2^2;  //�����������ź�

//sbit testOutput2 = P2^3;



/***************************************************************************/
// ��������
/***************************************************************************/

enum RunMode {MODEL_STOP, MODEL_RUN};  //����ģʽ 0��ֹͣ 1������
enum MotorStatus {MOTOR_BACKWARD, MOTOR_FORWARD, MOTOR_STOP};  //���״̬ 0����ǰ 1����� 2��ֹͣ
enum DisplayMode {DISPLAY_NO_FRESH, DISPLAY_RUN, DISPLAY_MAX_POSITION, DIAPLAY_MIN_POSITION, DIAPLAY_SAVING}; //��Ļ��ʾģʽ
enum SaveMode {SAVE_NO_SAVING, SAVE_SETTING_MOOD, SAVE_SETTING_STEP, SAVE_SETTING, SAVE_SNAPSHOT, READ_STEP, READ_STEP_SIM}; //�Ƿ񱣴�EEPROM״̬λ  0�������� 1����������ֵ 2����������ֵ���Ҷ�ȡ���� 3����������״̬

struct Motor //����ṹ��
{
	enum MotorStatus status; //���״̬ 0��ֹͣ 1����ǰ 2�����
	unsigned long stepPWMs; //������������
	unsigned long stepPassPWMs; //��������������
};

struct Setting //�������ýṹ��
{
	unsigned char currentMood; //��ǰ����
	unsigned char totalMood; //�ܻ���
	unsigned int currentStep; //��ǰ����
	unsigned int totalSteps; //�ܲ���

	unsigned int motor1Steps; //���1���ò���
	unsigned int motor2Steps; //���2���ò���
	unsigned int motor3Steps; //���3���ò���
	unsigned int motor4Steps; //���4���ò���

	unsigned int moodStatus[8]; //�����״̬ 
};



extern enum RunMode runMode; //����ģʽ
extern struct Motor motor1, motor2, motor3, motor4; //���
extern enum DisplayMode displayMode; //ˢ����Ļ��־λ 0 ��ˢ��
extern enum SaveMode saveMode; //����EEPROM״̬λ
extern struct Setting setting; //�������� 
extern unsigned int productNum; //�������� 

extern unsigned int activeSensorRun; //��ǰ��Ч���и�Ӧ��

/***************************************************************************/
// ��������
/***************************************************************************/
void parameter_init();
unsigned char parameter_save();
unsigned char parameter_save_mood();
unsigned char parameter_read();

void snapshot_init();
void snapshot_save();

#endif
