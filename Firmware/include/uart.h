

#ifndef __UART_H__
#define __UART_H__

typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef unsigned long LONG;

extern BYTE saveSetting; //是否保存设置值状态位

void uart_init();
void SendData(BYTE dat);
void SendDataToScreen(WORD address, WORD dat);
void SendString(char *s);
void anyData();
void ChangeScreenPage(WORD page);

#endif