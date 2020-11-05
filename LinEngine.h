#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h>   

#define START_FRAME   0x00
#define BREAK         0x01
#define SYNCH         0x02
#define PID           0x03
#define TX_DATA0      0x04
#define TX_DATA1      0x05
#define TX_DATA2      0x06
#define TX_DATA3      0x07
#define TX_DATA4      0x08
#define TX_DATA5      0x09
#define TX_DATA6      0x0A
#define TX_DATA7      0x0B
#define TX_CHECKSUM   0x0C

#define DATA_RX       0x0D

//#define CHCKSUM       0x0E

//bit TRANSMISSAO;


//Define os pinos para o LIN_SOFT
#define RX_SOFT     PORTBbits.RB2  //Int2 
#define TX_SOFT     LATCbits.LATC6 //=1LATC.//LATCbits.RC6 //RC6/TX/CK
#define CS_LIN      PORTBbits.RB5
#define VLR_TM0     180            // 179 -> 54,88us 180-> 53,9us
#define BIT_TM0     110            // 170 -> 66,00us 
#define LED_LIFE    PORTBbits.RB7


unsigned int   ui_ContadorMeioBitTime=0;
unsigned char  ucByte_RX , ucLinAction=0x00;
unsigned char  ucByteToTX, ucLinBitCount;
unsigned char  ucByteSizeToTX,ucByteSizeToRX;
unsigned char  LIN_ERROR_FLAGS;

bit b_ByteToSend;
void LinEngine(unsigned char * pFrameLin);
#endif	/* XC_HEADER_TEMPLATE_H */

