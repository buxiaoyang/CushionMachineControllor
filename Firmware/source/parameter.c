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
#include <at24c256.h>
#include <basefunc.h>
#include <dispatch.h>

/***************************************************************************/
// ��������
/***************************************************************************/

enum RunMode runMode; //����ģʽ 0��ֹͣ 1������
struct Motor motor1, motor2, motor3, motor4; //���
enum DisplayMode displayMode; //ˢ����Ļ��־λ 0 ��ˢ��
enum SaveMode saveMode; //����ģʽ
struct Setting setting; //��������
unsigned int productNum; //�������� 


/***************************************************************************/
// ������ʼ������
// ��������
// ����ֵ����	
/***************************************************************************/
void parameter_init()
{	
	/*
	WORD i,j;
	if(!parameter_read())
	{
		for(i=0; i < 6; i++)
		{
			for(j=0; j < 40; j++)
			{
//				motorRotationAngle1[i][j] = 0;
//				motorRotationAngle2[i][j] = 0;
			}
		}	
	}
	*/
	runMode = MODEL_RUN;
	displayMode = DISPLAY_RUN;
	saveMode = SAVE_NO_SAVING;
}

/***************************************************************************/
// �������浽eeprom
// ��������
// ����ֵ��0ʧ�� 1�ɹ�	
/***************************************************************************/
unsigned char parameter_save()
{
	WORD result = 1;
    
	return result;
}

/***************************************************************************/
// ��ȡ����
// ��������
// ����ֵ��0ʧ�� 1�ɹ�	
/***************************************************************************/
unsigned char parameter_read()
{
	WORD result = 0;

	return result;
}

void snapshot_init()
{
	Delay(10); 
	if(IapReadByte(IAP_ADDRESS+511) == 0xEE)
	{	
		productNum = (WORD)((IapReadByte(IAP_ADDRESS + 8) << 8) | IapReadByte(IAP_ADDRESS + 9));
	}
	else
	{
		productNum = 0;	
	}
}

void snapshot_save()
{
   	Delay(10);
	IapEraseSector(IAP_ADDRESS); //����EEPROM
	Delay(10);  
	IapProgramByte(IAP_ADDRESS + 8, (BYTE)(productNum>>8));
	IapProgramByte(IAP_ADDRESS + 9, (BYTE)productNum);
	IapProgramByte(IAP_ADDRESS+511, 0xEE); //д���־λ
	Delay(10);
}
