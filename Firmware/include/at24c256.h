/*------------------------------------------------------------------*/
/* --- STC MCU Limited ---------------------------------------------*/
/* --- STC12C5Axx Series MCU ISP/IAP/EEPROM Demo -------------------*/
/* --- Mobile: (86)13922805190 -------------------------------------*/
/* --- Fax: 86-755-82905966 ----------------------------------------*/
/* --- Tel: 86-755-82948412 ----------------------------------------*/
/* --- Web: www.STCMCU.com -----------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/


#ifndef __AT24C256_H__
#define __AT24C256_H__

#define AddWr 0xa0 //Write Address
#define AddRd 0xa1 //Read Address

void WrToROM(unsigned int Address, unsigned char Data);
unsigned char RdFromROM(unsigned int Address);

#endif
