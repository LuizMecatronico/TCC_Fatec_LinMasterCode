/*
 * File:   Config.c
 * Author: LUIZCARLOSDASILVA
 *
 * Created on 15 de Fevereiro de 2020, 18:36
 */


#include <xc.h>
#include "Config.h"

void ConfigHw(void) {
    
    TRISB = 0x00;           //Define PB como saídas e entradas de interrupção
    TRISBbits.RB2=1;        //RX_Soft IN                     //           obs
    TRISE = 0xFF;           //Define como entradas (Botões)
    TRISC = 0x00;           //Define como saídas
    TRISAbits.TRISA0 = 1;   //Entrada AN0
    TRISAbits.TRISA1 = 0;   //Saida AN1
    TRISAbits.TRISA2 = 0;
    
    LATA = 0x00;
    // = 0xFF;     //Carrega FF para apagar os LEDs
    LATC = 0x00;
    LATD = 0x00;
    LATE = 0x00;
    
    
    
    //-------------------------------------------------------------------------
    //PG262/430 Do manual do PIC18F4550
    //PCFG3:PDFG0  [AN12 -> AN0]
    //0000--> Todos analogicos
    //1111--> Todos Digitais
    //1110--> Somente AN0
    //1101--> Somente AN0 e AN1
    ADCON1bits.PCFG3 = 1;
    ADCON1bits.PCFG2 = 1;
    ADCON1bits.PCFG1 = 1;
    ADCON1bits.PCFG0 = 0;

    ADCON2bits.ADFM=1;    
    ADCON1bits.VCFG0= 0;  
    ADCON1bits.VCFG1=0;   
    
    ADCON2bits.ADCS0 = 0; 
    ADCON2bits.ADCS1 = 1;
    ADCON2bits.ADCS2 = 1;
    
    ADCON2bits.ACQT0 = 1; //Seleciona tempo de aquisição (2,66 us)//OBS LUIZ
    ADCON2bits.ACQT1 = 0;
    ADCON2bits.ACQT2 = 0;
    ADCON0bits.ADON = 1;  //Ativar módulo de ADC
    
    ADCON0bits.CHS0=0;    //Seleciona AN0 para ligar no S/H
    ADCON0bits.CHS1=0;
    ADCON0bits.CHS2=0;
    ADCON0bits.CHS3=0;
    
    T0CONbits.TMR0ON = 1;     //Habilita o Timer 0;
    T0CONbits.T08BIT = 1;     //Habilitar no modo 8 bits
    T0CONbits.T0CS   = 0;     //Usa o oscilador interno como clock
    T0CONbits.T0SE   = 0;     // Irrelevante na seleção interna
    T0CONbits.PSA    = 0;     //Ativa o Prescaler para dividir o sinal de clock
    T0CONbits.T0PS   = 0b001; //Prescaler divisor por 4
    
    
    
    //Configura a Interrupçao do Timer 0
    
    INTCONbits.TMR0IE = 1;// Habilita Timer 0 pelo flag de overflow
    
    //Habilita interrupção exerna 0 e 1
    //Pino B0 e B1 são entrdas e pinos B[2-7] são saídas
    
    INTCONbits.RBIE = 0;
    INTCONbits.INT0E = 0;
    INTCON3bits.INT1IE = 0;
    
    INTCON3bits.INT2IF=0;
    INTCON3bits.INT2IE=0;
    
    
    INTCONbits.GIE=1;
    
    
    //Interrupcao Externado 02
    //INT2IP: INT2 External Interrupt Priority bit
    //1 = High priority
    //0 = Low priority
    INTCON3bits.INT2IP=1;// Interrupcao Externa Alta Prioridade.
    

    //INTEDG2: External Interrupt 2 Edge Select bit
    //1 = Interrupt on rising edge
    //0 = Interrupt on falling edge
    INTCON2bits.INTEDG2=0;// Interrupcao on the falling edge
    
    //INTCON=0xA0;
    //CONbits.TMR0ON = 0;     //Desabilita o Timer 0;
}
