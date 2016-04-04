

#ifndef __UART_H__
#define __UART_H__

typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef unsigned long LONG;

extern BYTE saveSetting; //�Ƿ񱣴�����ֵ״̬λ

void uart_init();
void SendData(BYTE dat);
void SendDataToScreen(WORD address, WORD dat);
void SendString(char *s);
void anyData();
void ChangeScreenPage(WORD page);
void refreshDisplaySetting();

#endif