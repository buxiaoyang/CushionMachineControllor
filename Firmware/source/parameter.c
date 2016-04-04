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
//struct Motor motor1, motor2; //���
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
	unsigned char result = 0,i,j,address;
	delay_ms(10); 
	if(IapReadByte(IAP_ADDRESS+511) == 0xEE)
	{	
		for(i=0; i < 6; i++)
		{
			for(j=0; j < 40; j++)
			{
				address = i * 80 + (j<<1);
				motorRotationAngle[i][j] = ((IapReadByte(IAP_ADDRESS + address) << 8) | IapReadByte(IAP_ADDRESS + address + 1));
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
	WORD i;

    testOutput1 = 0;
    Delay(10);                      //Delay
    IapEraseSector(IAP_ADDRESS);    //Erase current sector
    for (i=0; i<512; i++)           //Check whether all sector data is FF
    {
        if (IapReadByte(IAP_ADDRESS+i) != 0xff)
            goto Error;             //If error, break
    }
    testOutput2 = 0;
    Delay(10);                      //Delay
    for (i=0; i<512; i++)           //Program 512 bytes data into data flash
    {
        IapProgramByte(IAP_ADDRESS+i, (BYTE)i);
    }
   	testOutput3 = 0;
    Delay(10);                      //Delay
    for (i=0; i<512; i++)           //Verify 512 bytes data
    {
        if (IapReadByte(IAP_ADDRESS+i) != (BYTE)i)
            goto Error;             //If error, break
    }
    testOutput4 = 0;
    while (1);
Error:
    P1 &= 0x7f;                     //0xxx,xxxx IAP operation fail
    while (1);

	/*
	unsigned char i,j;
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
	*/
}

/***************************************************************************/
// �������浽eeprom
// ��������
// ����ֵ��0ʧ�� 1�ɹ�	
/***************************************************************************/
unsigned char parameter_save()
{
	unsigned char i,j,address;
    delay_ms(10);                      //Delay
	IapEraseSector(IAP_ADDRESS); //����EEPROM
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
	return 1;
}


