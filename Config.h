
#ifndef __CONFIG_H
#define	__CONFIG_H

#include <xc.h>  

extern void l_init_hw(void); 


#define   BUS_WARN_TIME  d'25000'


#define   LINRX          RCREG
#define   LINTX          TXREG
#define   LINBRG         SPBRG
#define   LINIF          PIR1,RCIF
#define   LINIE          PIR1,RCIE

#define   LINTXSETUP_V   b'00100100'
#define   LINRXSETUP_V   b'10010000'
#define   LINTXSETUP     TXSTA
#define   LINRXSETUP     RCSTA
#define   RXPIN          LATC,RX
#define   TXPIN          LATC,TX
#define   RXPINDIR       TRISC,RX
#define   TXPINDIR       TRISC,TX

#define   L_BUSY         LIN_STATE_FLAGS,0
#define   L_TXRX         LIN_STATE_FLAGS,1
#define   L_RBACK        LIN_STATE_FLAGS,2
#define   L_BREAK        LIN_STATE_FLAGS,3
#define   L_SYNC         LIN_STATE_FLAGS,4
#define   L_ID           LIN_STATE_FLAGS,5
#define   L_DATA         LIN_STATE_FLAGS,6
#define   L_CHKSM        LIN_STATE_FLAGS,7

#define   L_SLEEP        LIN_STATUS_FLAGS,0
#define   L_RWAKE        LIN_STATUS_FLAGS,1

#define   LE_BIT         LIN_ERROR_FLAGS,0
#define   LE_CHKSM       LIN_ERROR_FLAGS,1
#define   LE_FTO         LIN_ERROR_FLAGS,2
#define   LE_BTO         LIN_ERROR_FLAGS,3



#define FALSE       0
#define TRUE        1
#define DOMINANTE   0
#define RECESSIVO   1

bit b_MeioBitTime,FlgNegativo;
bit b_BitLeitura,b_BitEscrita, b_BitLeitura_;
bit LinEngineBusy,END_BIT,TRANSMISSAO;
bit ImprimeTela,startOfByte;
bit b_diagnostico;
    
//LIN_DATA_COUNT           ; Reset the message data counter
//LIN_STATUS_FLAGS         ; Clear all flags
//LIN_STATE_FLAGS
//LIN_ERROR_FLAGS

//#define ERRO_ESCRITA     
//; *******************************************************************

#define _XTAL_FREQ 48000000
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
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

