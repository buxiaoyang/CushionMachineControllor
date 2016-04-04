/***************************************************************************/
// ����15�����������ϵͳ
// ģ�飺��������
// �ļ���parameter.c
// ���ߣ������D
// �汾��V0.01
// ���ڣ�2016��4��3��
// ���ܣ�������
// оƬ��STC12C5A60S2
// ���룺Keil uVision3 V3.90
/***************************************************************************/

#include <parameter.h>
#include <eeprom.h>
#include <basefunc.h>

/***************************************************************************/
// ��������
/***************************************************************************/

enum RunMode runMode; //����ģʽ 0��ֹͣ 1������
struct Motor motor1, motor2; //���
unsigned int motorRotationAngle[6][40]; //�����ת�� 6�����ݣ�ÿ��40��
unsigned char motorCurrentRatationGroup; //��ǰ������ת����
enum DisplayMode displayMode; //ˢ����Ļ��־λ 0 ��ˢ��

/***************************************************************************/
// ��ȡ����
// ��������
// ����ֵ��0ʧ�� 1�ɹ�	
/***************************************************************************/
unsigned char parameter_read()
{
	WORD result = 0,i,j,address;
	Delay(10); 
	if(IapReadByte(IAP_ADDRESS+511) == 0xEE)
	{	
		for(i=0; i < 6; i++)
		{
			for(j=0; j < 40; j++)
			{
				address = i * 80 + (j<<1);
				motorRotationAngle[i][j] = (WORD)((IapReadByte(IAP_ADDRESS + address) << 8) | IapReadByte(IAP_ADDRESS + address + 1));
			}
			
		}
		result = 1;
	}
	return result;
}

/***************************************************************************/
// ������ʼ������
// ��������
// ����ֵ����	
/***************************************************************************/
void parameter_init()
{	
	WORD i,j;
	if(!parameter_read())
	{
		for(i=0; i < 6; i++)
		{
			for(j=0; j < 40; j++)
			{
				motorRotationAngle[i][j] = 0;
			}
		}	
	}

	//���������ʼ��
	motor1.position = 0;
	motor1.status = MOTOR_STOP;
	motor1.isStartPosition = 1;
	motor1.stepPWMs = 0;
	motor1.stepPassPWMs = 0;
	motor1.totalPWMs = 0;
	motor1.currentStage = 0;

	motor2.position = 0;
	motor2.status = MOTOR_STOP;
	motor2.isStartPosition = 1;
	motor2.stepPWMs = 0;
	motor2.stepPassPWMs = 0;
	motor2.totalPWMs = 0;
	motor2.currentStage = 0;
}

/***************************************************************************/
// �������浽eeprom
// ��������
// ����ֵ��0ʧ�� 1�ɹ�	
/***************************************************************************/
unsigned char parameter_save()
{
	WORD result = 1,i,j,address;
    Delay(10);                     //Delay
	IapEraseSector(IAP_ADDRESS); //����EEPROM
	Delay(10);  
	//д�����
	for(i=0; i < 6; i++)
	{
		for(j=0; j < 40; j++)
		{
			address = i * 80 + (j<<1);
			IapProgramByte(IAP_ADDRESS + address, (BYTE)(motorRotationAngle[i][j]>>8));
			IapProgramByte(IAP_ADDRESS + address+1, (BYTE)(motorRotationAngle[i][j]));
		}
	}
	IapProgramByte(IAP_ADDRESS+511, 0xEE); //д���־λ
	Delay(10);
	//����д��
	for(i=0; i < 6; i++)
	{
		for(j=0; j < 40; j++)
		{
			address = i * 80 + (j<<1);
			if(motorRotationAngle[i][j] != (WORD)((IapReadByte(IAP_ADDRESS + address) << 8) | IapReadByte(IAP_ADDRESS + address + 1)))
			{
				result = 0;
			}
		}
		
	}
	return result;
}


