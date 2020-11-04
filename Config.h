
#ifndef __CONFIG_H
#define	__CONFIG_H

#include <xc.h>  

extern void l_init_hw(void); 

#define FALSE       0
#define TRUE        1
#define DOMINANTE   0
#define RECESSIVO   1

bit b_MeioBitTime,FlgNegativo;
bit b_BitLeitura,b_BitEscrita, b_BitLeitura_;
bit LinEngineBusy,END_BIT,TRANSMISSAO;
bit ImprimeTela,startOfByte;
bit b_diagnostico;
  

//*******************************************************************

#define _XTAL_FREQ 48000000

#endif	/* XC_HEADER_TEMPLATE_H */

