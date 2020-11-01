/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  
//#include <pic18f4550.h>

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

#define CHCKSUM       0x0E



//bit TRANSMISSAO;


//Define os pinos para o LIN_SOFT
#define RX_SOFT     PORTBbits.RB2 //Int2 
#define TX_SOFT     LATCbits.LATC6//=1LATC.//LATCbits.RC6 //RC6/TX/CK
#define CS_LIN      PORTBbits.RB5
#define VLR_TM0     180   // 179 -> 54,88us 180-> 53,9us
#define BIT_TM0     110   // 170 -> 66,00us 
#define LED_LIFE    PORTBbits.RB7


unsigned int   ui_ContadorMeioBitTime=0;
unsigned char  ucByte_RX , ucLinAction=0x00;
unsigned char  ucByteToTX, ucLinBitCount;
unsigned char  ucByteSizeToTX,ucByteSizeToRX;
unsigned char  LIN_ERROR_FLAGS;

bit b_ByteToSend;

void LinEngine(unsigned char * pFrameLin);
// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

