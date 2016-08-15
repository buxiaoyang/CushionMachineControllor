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
	if(!parameter_read())
	{
		setting.currentMood = 1;
		setting.totalMood = 2;
		setting.currentStep = 3;
		setting.totalSteps = 600;
	
		setting.motor1Steps = 100;
		setting.motor2Steps = 200;
		setting.motor3Steps = 300;
		setting.motor4Steps = 400;
	
		setting.moodStatus[0] = 0xFFFE;
		setting.moodStatus[1] = 0x7FFF;
		setting.moodStatus[2] = 0xFFFF;
		setting.moodStatus[3] = 0xFFFF;
		setting.moodStatus[4] = 0xFFFF;
		setting.moodStatus[5] = 0xFFFF;
		setting.moodStatus[6] = 0xFFFF;
		setting.moodStatus[7] = 0xFFFF;
	}
	productNum = 0;

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
	WORD result = 1, baseAddress, stepAddress;
	WrToROM(0x0000, setting.currentMood);
	WrToROM(0x0001, setting.totalMood);
	
	baseAddress = (setting.currentMood - 1) * MOOD_STORAGE_SIZE + 2;
	WrToROM(baseAddress, (BYTE)(setting.totalSteps >> 8));
	WrToROM(baseAddress+1, (BYTE)setting.totalSteps);
	/*	
	stepAddress =  baseAddress + 24 * (setting.currentStep-1) + 2;
	WrToROM(stepAddress, (BYTE)(setting.motor1Steps >> 8));
	WrToROM(stepAddress+1, (BYTE)setting.motor1Steps);
	WrToROM(stepAddress+2, (BYTE)(setting.motor2Steps >> 8));
	WrToROM(stepAddress+3, (BYTE)setting.motor2Steps);
	WrToROM(stepAddress+4, (BYTE)(setting.motor3Steps >> 8));
	WrToROM(stepAddress+5, (BYTE)setting.motor3Steps);
	WrToROM(stepAddress+6, (BYTE)(setting.motor4Steps >> 8));
	WrToROM(stepAddress+7, (BYTE)setting.motor4Steps);
	WrToROM(stepAddress+8, (BYTE)(setting.moodStatus[0] >> 8));
	WrToROM(stepAddress+9, (BYTE)setting.moodStatus[0]);
	WrToROM(stepAddress+10, (BYTE)(setting.moodStatus[1] >> 8));
	WrToROM(stepAddress+11, (BYTE)setting.moodStatus[1]);
	WrToROM(stepAddress+12, (BYTE)(setting.moodStatus[2] >> 8));
	WrToROM(stepAddress+13, (BYTE)setting.moodStatus[2]);
	WrToROM(stepAddress+14, (BYTE)(setting.moodStatus[3] >> 8));
	WrToROM(stepAddress+15, (BYTE)setting.moodStatus[3]);
	WrToROM(stepAddress+16, (BYTE)(setting.moodStatus[4] >> 8));
	WrToROM(stepAddress+17, (BYTE)setting.moodStatus[4]);
	WrToROM(stepAddress+18, (BYTE)(setting.moodStatus[5] >> 8));
	WrToROM(stepAddress+19, (BYTE)setting.moodStatus[5]);
	WrToROM(stepAddress+20, (BYTE)(setting.moodStatus[6] >> 8));
	WrToROM(stepAddress+21, (BYTE)setting.moodStatus[6]);
	WrToROM(stepAddress+22, (BYTE)(setting.moodStatus[7] >> 8));
	WrToROM(stepAddress+23, (BYTE)setting.moodStatus[7]);
	*/
	return result;
}

/***************************************************************************/
// �������浽eeprom
// ��������
// ����ֵ��0ʧ�� 1�ɹ�	
/***************************************************************************/
unsigned char parameter_save_step()
{
	WORD result = 1, baseAddress, stepAddress;
	//WrToROM(0x0000, setting.currentMood);
	//WrToROM(0x0001, setting.totalMood);
	
	baseAddress = (setting.currentMood - 1) * MOOD_STORAGE_SIZE + 2;
	//WrToROM(baseAddress, (BYTE)(setting.totalSteps >> 8));
	//WrToROM(baseAddress+1, (BYTE)setting.totalSteps);
	
	stepAddress =  baseAddress + 24 * (setting.currentStep-1) + 2;
	WrToROM(stepAddress, (BYTE)(setting.motor1Steps >> 8));
	WrToROM(stepAddress+1, (BYTE)setting.motor1Steps);
	WrToROM(stepAddress+2, (BYTE)(setting.motor2Steps >> 8));
	WrToROM(stepAddress+3, (BYTE)setting.motor2Steps);
	WrToROM(stepAddress+4, (BYTE)(setting.motor3Steps >> 8));
	WrToROM(stepAddress+5, (BYTE)setting.motor3Steps);
	WrToROM(stepAddress+6, (BYTE)(setting.motor4Steps >> 8));
	WrToROM(stepAddress+7, (BYTE)setting.motor4Steps);
	WrToROM(stepAddress+8, (BYTE)(setting.moodStatus[0] >> 8));
	WrToROM(stepAddress+9, (BYTE)setting.moodStatus[0]);
	WrToROM(stepAddress+10, (BYTE)(setting.moodStatus[1] >> 8));
	WrToROM(stepAddress+11, (BYTE)setting.moodStatus[1]);
	WrToROM(stepAddress+12, (BYTE)(setting.moodStatus[2] >> 8));
	WrToROM(stepAddress+13, (BYTE)setting.moodStatus[2]);
	WrToROM(stepAddress+14, (BYTE)(setting.moodStatus[3] >> 8));
	WrToROM(stepAddress+15, (BYTE)setting.moodStatus[3]);
	WrToROM(stepAddress+16, (BYTE)(setting.moodStatus[4] >> 8));
	WrToROM(stepAddress+17, (BYTE)setting.moodStatus[4]);
	WrToROM(stepAddress+18, (BYTE)(setting.moodStatus[5] >> 8));
	WrToROM(stepAddress+19, (BYTE)setting.moodStatus[5]);
	WrToROM(stepAddress+20, (BYTE)(setting.moodStatus[6] >> 8));
	WrToROM(stepAddress+21, (BYTE)setting.moodStatus[6]);
	WrToROM(stepAddress+22, (BYTE)(setting.moodStatus[7] >> 8));
	WrToROM(stepAddress+23, (BYTE)setting.moodStatus[7]);
	return result;
}

/***************************************************************************/
// ��ȡ����
// ��������
// ����ֵ��0ʧ�� 1�ɹ�	
/***************************************************************************/
unsigned char parameter_read()
{
	WORD result = 1, baseAddress, stepAddress;
	setting.currentMood = RdFromROM(0x0000);
	setting.totalMood = RdFromROM(0x0001);

	baseAddress = (setting.currentMood - 1) * MOOD_STORAGE_SIZE + 2;
	setting.totalSteps = (WORD)((RdFromROM(baseAddress) << 8) | RdFromROM(baseAddress + 1));

	stepAddress =  baseAddress + 24 * (setting.currentStep-1) + 2;
	setting.motor1Steps = (WORD)((RdFromROM(stepAddress) << 8) | RdFromROM(stepAddress + 1));
	setting.motor2Steps = (WORD)((RdFromROM(stepAddress + 2) << 8) | RdFromROM(stepAddress + 3));
	setting.motor3Steps = (WORD)((RdFromROM(stepAddress + 4) << 8) | RdFromROM(stepAddress + 5));
	setting.motor4Steps = (WORD)((RdFromROM(stepAddress + 6) << 8) | RdFromROM(stepAddress + 7));
	setting.moodStatus[0] = (WORD)((RdFromROM(stepAddress + 8) << 8) | RdFromROM(stepAddress + 9));
	setting.moodStatus[1] = (WORD)((RdFromROM(stepAddress + 10) << 8) | RdFromROM(stepAddress + 11));
	setting.moodStatus[2] = (WORD)((RdFromROM(stepAddress + 12) << 8) | RdFromROM(stepAddress + 13));
	setting.moodStatus[3] = (WORD)((RdFromROM(stepAddress + 14) << 8) | RdFromROM(stepAddress + 15));
	setting.moodStatus[4] = (WORD)((RdFromROM(stepAddress + 16) << 8) | RdFromROM(stepAddress + 17));
	setting.moodStatus[5] = (WORD)((RdFromROM(stepAddress + 18) << 8) | RdFromROM(stepAddress + 19));
	setting.moodStatus[6] = (WORD)((RdFromROM(stepAddress + 20) << 8) | RdFromROM(stepAddress + 21));
	setting.moodStatus[7] = (WORD)((RdFromROM(stepAddress + 22) << 8) | RdFromROM(stepAddress + 23));

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
