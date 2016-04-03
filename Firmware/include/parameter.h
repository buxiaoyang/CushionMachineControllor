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
sbit keyMotor2Operation = P0^4; //���2�����ź�

//���
sbit ioMotor1Direction = P1^0; //���1����
sbit ioMmotor1PWM = P2^6; //���1����
sbit ioMmotor2Direction = P2^0; //���2����
sbit ioMmotor2PWM = P2^1; //���2����

sbit testOut = P3^7;

/***************************************************************************/
// ��������
/***************************************************************************/
void parameter_init();
unsigned char parameter_save();


extern unsigned char runMode; //����ģʽ 0��ֹͣ 1������
extern unsigned char motor1Position; //���1��ǰλ�� 0~39
extern unsigned char motor2Position; //���2��ǰλ�� 0~39
extern unsigned char motor1Direction; //���1���з���
extern unsigned char motor2Direction; //���2���з���
extern unsigned char isStartPosition1; //���1�Ƿ��ڳ�ʼλ�ñ�־λ
extern unsigned char isStartPosition2; //���2�Ƿ��ڳ�ʼλ�ñ�־λ
extern unsigned long stepTotalPWMs; //������������
extern unsigned long stepRemainPWMs; //����ʣ��������
extern unsigned long totalPWMs; //������������
extern unsigned int motorRotationAngle[6][40]; //�����ת�� 6�����ݣ�ÿ��40��
extern unsigned int currentStage; //��ǰ����������0~5
extern unsigned char refreshDisplay; //ˢ����Ļ��־λ 0 ��ˢ�� 1ˢ��

#endif
