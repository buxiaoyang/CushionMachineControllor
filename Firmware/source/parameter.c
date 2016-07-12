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
#include <dispatch.h>

/***************************************************************************/
// ��������
/***************************************************************************/

enum RunMode runMode; //����ģʽ 0��ֹͣ 1������
struct Motor motor1, motor2, motor3, motor4; //���
unsigned int xdata motorRotationAngle1[6][40]; //�����ת��һ�� 6�������ݣ�ÿ����40��
unsigned int xdata motorRotationAngle2[6][40]; //�����ת�Ƕ��� 6�������ݣ�ÿ����40��
unsigned char motorCurrentRatationStage; //��ǰ������ת�ǹ���
unsigned char motorCurrentRatationGroup; //��ǰ������ת���� 1~2
enum DisplayMode displayMode; //ˢ����Ļ��־λ 0 ��ˢ��
enum SaveMode saveMode;

/***************************************************************************/
// ��ȡ����
// ��������
// ����ֵ��0ʧ�� 1�ɹ�	
/***************************************************************************/
unsigned char parameter_read()
{
	WORD result = 0,i,j,address;
	Delay(10); 
	if(IapReadByte(IAP_ADDRESS1+511) == 0xEE && IapReadByte(IAP_ADDRESS3+511) == 0xEE)
	{	
		for(i=0; i < 6; i++)
		{
			for(j=0; j < 40; j++)
			{
				address = i * 80 + (j<<1);
				motorRotationAngle1[i][j] = (WORD)((IapReadByte(IAP_ADDRESS1 + address) << 8) | IapReadByte(IAP_ADDRESS1 + address + 1));
				motorRotationAngle2[i][j] = (WORD)((IapReadByte(IAP_ADDRESS3 + address) << 8) | IapReadByte(IAP_ADDRESS3 + address + 1));
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
				motorRotationAngle1[i][j] = 0;
				motorRotationAngle2[i][j] = 0;
			}
		}	
	}

	runMode = MODEL_RUN;
	motorCurrentRatationStage = 0;
	motorCurrentRatationGroup = 1;
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
	WORD result = 1,i,j,address;
    Delay(10);                     //Delay
	IapEraseSector(IAP_ADDRESS1); //����EEPROM
	Delay(10);  
	//д�����
	for(i=0; i < 6; i++)
	{
		for(j=0; j < 40; j++)
		{
			address = i * 80 + (j<<1);
			IapProgramByte(IAP_ADDRESS1 + address, (BYTE)(motorRotationAngle1[i][j]>>8));
			IapProgramByte(IAP_ADDRESS1 + address+1, (BYTE)(motorRotationAngle1[i][j]));
		}
	}
	IapProgramByte(IAP_ADDRESS1+511, 0xEE); //д���־λ
	Delay(10);
	//����д��
	for(i=0; i < 6; i++)
	{
		for(j=0; j < 40; j++)
		{
			address = i * 80 + (j<<1);
			if(motorRotationAngle1[i][j] != (WORD)((IapReadByte(IAP_ADDRESS1 + address) << 8) | IapReadByte(IAP_ADDRESS1 + address + 1)))
			{
				result = 0;
			}
		}
		
	}

	Delay(10);                     //Delay
	IapEraseSector(IAP_ADDRESS3); //����EEPROM
	Delay(10);  
	//д�����
	for(i=0; i < 6; i++)
	{
		for(j=0; j < 40; j++)
		{
			address = i * 80 + (j<<1);
			IapProgramByte(IAP_ADDRESS3 + address, (BYTE)(motorRotationAngle2[i][j]>>8));
			IapProgramByte(IAP_ADDRESS3 + address+1, (BYTE)(motorRotationAngle2[i][j]));
		}
	}
	IapProgramByte(IAP_ADDRESS3+511, 0xEE); //д���־λ
	Delay(10);
	//����д��
	for(i=0; i < 6; i++)
	{
		for(j=0; j < 40; j++)
		{
			address = i * 80 + (j<<1);
			if(motorRotationAngle2[i][j] != (WORD)((IapReadByte(IAP_ADDRESS3 + address) << 8) | IapReadByte(IAP_ADDRESS3 + address + 1)))
			{
				result = 0;
			}
		}
		
	}
	return result;
}

void snapshot_init()
{
	if(IapReadByte(IAP_ADDRESS2+511) == 0xEE)
	{
		//���������ʼ��
		motor1.position = IapReadByte(IAP_ADDRESS2);
		motor1.status = IapReadByte(IAP_ADDRESS2+1);
		motor1.isStartPosition = IapReadByte(IAP_ADDRESS2+2);
		motor1.currentStage = IapReadByte(IAP_ADDRESS2+3);
	
		motor2.position = IapReadByte(IAP_ADDRESS2+20);
		motor2.status = IapReadByte(IAP_ADDRESS2+21);
		motor2.isStartPosition = IapReadByte(IAP_ADDRESS2+22);
		motor2.currentStage = IapReadByte(IAP_ADDRESS2+23);

		motor3.position = IapReadByte(IAP_ADDRESS2+40);
		motor3.status = IapReadByte(IAP_ADDRESS2+41);
		motor3.isStartPosition = IapReadByte(IAP_ADDRESS2+42);
		motor3.currentStage = IapReadByte(IAP_ADDRESS2+43);
		
		motor4.position = IapReadByte(IAP_ADDRESS2+60);
		motor4.status = IapReadByte(IAP_ADDRESS2+61);
		motor4.isStartPosition = IapReadByte(IAP_ADDRESS2+62);
		motor4.currentStage = IapReadByte(IAP_ADDRESS2+63);

		lastStatus1 = IapReadByte(IAP_ADDRESS2+82);
		lastStepPWMs1 = (unsigned long)((IapReadByte(IAP_ADDRESS2+83)<<24) | (IapReadByte(IAP_ADDRESS2+84)<<16) | (IapReadByte(IAP_ADDRESS2+85)<<8) | (IapReadByte(IAP_ADDRESS2+86)));

		lastStatus2 = IapReadByte(IAP_ADDRESS2+87);
		lastStepPWMs2 = (unsigned long)((IapReadByte(IAP_ADDRESS2+88)<<24) | (IapReadByte(IAP_ADDRESS2+89)<<16) | (IapReadByte(IAP_ADDRESS2+90)<<8) | (IapReadByte(IAP_ADDRESS2+91)));
	}
	else
	{
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

		motor3.position = 0;
		motor3.status = MOTOR_STOP;
		motor3.isStartPosition = 1;
		motor3.stepPWMs = 0;
		motor3.stepPassPWMs = 0;
		motor3.totalPWMs = 0;
		motor3.currentStage = 0;

		motor4.position = 0;
		motor4.status = MOTOR_STOP;
		motor4.isStartPosition = 1;
		motor4.stepPWMs = 0;
		motor4.stepPassPWMs = 0;
		motor4.totalPWMs = 0;
		motor4.currentStage = 0;

		lastStatus1 = MOTOR_STOP;
		lastStepPWMs1 = 0;

		lastStatus2 = MOTOR_STOP;
		lastStepPWMs2 = 0;

	}
	displayMode = DISPLAY_RUN;
}

void snapshot_save()
{
	IapEraseSector(IAP_ADDRESS2); //����EEPROM
	delay_ms(3);  
	//д����� ���1
	IapProgramByte(IAP_ADDRESS2, motor1.position);
	IapProgramByte(IAP_ADDRESS2+1, motor1.status); 
	IapProgramByte(IAP_ADDRESS2+2, motor1.isStartPosition); 
	IapProgramByte(IAP_ADDRESS2+3, motor1.currentStage);
	//д����� ���2
	IapProgramByte(IAP_ADDRESS2+20, motor2.position);
	IapProgramByte(IAP_ADDRESS2+21, motor2.status); 
	IapProgramByte(IAP_ADDRESS2+22, motor2.isStartPosition); 
	IapProgramByte(IAP_ADDRESS2+23, motor2.currentStage);
	//д����� ���3
	IapProgramByte(IAP_ADDRESS2+40, motor3.position);
	IapProgramByte(IAP_ADDRESS2+41, motor3.status); 
	IapProgramByte(IAP_ADDRESS2+42, motor3.isStartPosition); 
	IapProgramByte(IAP_ADDRESS2+43, motor3.currentStage);
	//д����� ���2
	IapProgramByte(IAP_ADDRESS2+60, motor4.position);
	IapProgramByte(IAP_ADDRESS2+61, motor4.status); 
	IapProgramByte(IAP_ADDRESS2+62, motor4.isStartPosition); 
	IapProgramByte(IAP_ADDRESS2+63, motor4.currentStage);


	IapProgramByte(IAP_ADDRESS2+82, lastStatus1);
	IapProgramByte(IAP_ADDRESS2+83, (BYTE)(lastStepPWMs1>>24)); 
	IapProgramByte(IAP_ADDRESS2+84, (BYTE)(lastStepPWMs1>>16)); 
	IapProgramByte(IAP_ADDRESS2+85, (BYTE)(lastStepPWMs1>>8)); 
	IapProgramByte(IAP_ADDRESS2+86, (BYTE)(lastStepPWMs1));

	IapProgramByte(IAP_ADDRESS2+87, lastStatus2);
	IapProgramByte(IAP_ADDRESS2+88, (BYTE)(lastStepPWMs2>>24)); 
	IapProgramByte(IAP_ADDRESS2+89, (BYTE)(lastStepPWMs2>>16)); 
	IapProgramByte(IAP_ADDRESS2+90, (BYTE)(lastStepPWMs2>>8)); 
	IapProgramByte(IAP_ADDRESS2+91, (BYTE)(lastStepPWMs2));

	//д���־λ
	IapProgramByte(IAP_ADDRESS2+511, 0xEE);
	delay_ms(10);
}
