/*
 * File:   LinEngine.c
 * Author: LUIZCARLOSDASILVA
 *
 * Created on 15 de Fevereiro de 2020, 16:35
 */


#include <xc.h>
#include "Config.h"
#include "LinEngine.h"

unsigned int   ucCount=0;
unsigned char  ucByteRecebido=0;
unsigned char  ucCountRX=0;
unsigned char  ucDATA_CNT,ucValorCheckSum,ucMsgToTX[10]=0;

bit LinErro;

void ByteToTXs( unsigned char ucByteToSend)
{
    if(!LinEngineBusy)
    {
        TX_SOFT=DOMINANTE;
        TMR0L=VLR_TM0;
        b_MeioBitTime=FALSE;
        LinEngineBusy=TRUE;
        ucByteToTX=ucByteToSend;
        ucLinBitCount = 0;
        ucByteSizeToTX--;
    }
         
}

void LinEngine(unsigned char * pFrameLin)
{
    unsigned char IndexByteToTX=0x00;
    
    //Caso o valor de pFrameLin[0] seja maior doque 0xF, isso indica que 
    //o nible alto desse byte tem um valor, valor esse que corresponde 
    //a quantidade de bytes a serem recebidos, incluido o CHECKSUM,
    //proveniente do slave, no caso o PGA450-Q1.
    if(pFrameLin[0]>0xF)
    {
       ucByteSizeToTX =  (pFrameLin[0] & 0x0F);
       ucByteSizeToRX = ((pFrameLin[0]>>4) & 0x0F);
    }
    else//Somente Transmissao.
       ucByteSizeToTX = (pFrameLin[0] & 0x0F);

    
    LIN_ERROR_FLAGS=FALSE;// Rever essas flag, pois nao esta apropriadamente
                          // estabelecida seus bits.   
    ucLinAction=0;
      while(!LIN_ERROR_FLAGS)
      {
        switch (ucLinAction)
        {
            case START_FRAME:
                
                TX_SOFT=RECESSIVO;
                INTCON3bits.INT2IE=FALSE;
                TX_SOFT=DOMINANTE;
                TMR0L=VLR_TM0;
                INTCONbits.TMR0IF = 0;
                INTCONbits.TMR0IE = 1;
                LIN_ERROR_FLAGS=0x00;
                TRANSMISSAO=TRUE;
                ucLinBitCount=0;
                ui_ContadorMeioBitTime=0x00;
                ucByteToTX=0x00;
                while(ui_ContadorMeioBitTime<9);
                ucLinAction++;
                LinEngineBusy=FALSE;
            break;
            
            case BREAK:
               
                if(!LinEngineBusy)
                    ucLinBitCount=0;
            break;
            
            case SYNCH:
                ByteToTXs(0x55);
            break;
           
            case PID:
                ByteToTXs(pFrameLin[1]);
            break;
            
            case TX_DATA0:
                ByteToTXs(pFrameLin[2]);
            break;
            
            case TX_DATA1:
                ByteToTXs(pFrameLin[3]);
            break;
                
            case TX_DATA2:
                ByteToTXs(pFrameLin[4]);
            break;
                
            case TX_DATA3:
                ByteToTXs(pFrameLin[5]);
            break;
            
            case TX_DATA4:
                ByteToTXs(pFrameLin[6]);
            break;
            
            case TX_DATA5:
                ByteToTXs(pFrameLin[7]);
            break;
            
            case TX_DATA6:
                ByteToTXs(pFrameLin[8]);
            break;
            
            case TX_DATA7:
                ByteToTXs(pFrameLin[9]);
            break;
            
            case TX_CHECKSUM:
                ByteToTXs(pFrameLin[10]);
            break;
                

            case DATA_RX:
            
             //O codigo abaixo é somente experiencia
//             LinEngineBusy=FALSE;
//             TX_SOFT=RECESSIVO;
//             INTCONbits.TMR0IF = 0;
//             INTCONbits.TMR0IE = 0;
//             __delay_ms(20);
//             INTCONbits.TMR0IF = 0;
//             INTCONbits.TMR0IE = 1;
//             ucLinAction=0;
             LIN_ERROR_FLAGS=TRUE;// somente experiencia.
             //Até aqui é a experiencia.
                
//                if(!LinEngineBusy)
//                    {
//                        TX_SOFT=DOMINANTE;
//                        TMR0L=VLR_TM0;
//                        b_MeioBitTime=FALSE;
//                        LinEngineBusy=TRUE;
//                        ucByteToTX=pFrameLin[3];
//                        ucLinBitCount = 0;
//                        
//                       // ucByteSizeToTX--;
//                    }
                /*
                TX_SOFT=RECESSIVO;
                INTCONbits.TMR0IF = 0;
                INTCONbits.TMR0IE = 0;
                
                ucLinBitCount = 0;
                TRANSMISSAO=FALSE;
                ucByteRecebido=0x00;
                INTCON3bits.INT2IF=0;
                INTCON3bits.INT2IE=1;
                ucByte_RX=0;
                ucDATA_CNT=9;
//                do
//                {
//                    //LIGA UM TIME-OUT Timer1
//                    INTCON3bits.INT2IF=0;
//                    INTCON3bits.INT2E=1;
//                }while(ucByte_RX < ucDATA_CNT);
                
                //INTCON3bits.INT2IF=0;
                //INTCON3bits.INT2IE=1;
               // delay_ms(10);
                INTCONbits.TMR0IF = 0;
                INTCONbits.TMR0IE = 0;
                ImprimeTela=TRUE;


                */
            break;
            
             case CHCKSUM: //(pFrameLin[3]&0x0F);
            
             TX_SOFT=RECESSIVO;
             INTCONbits.TMR0IF = 0;
             INTCONbits.TMR0IE = 0;
             __delay_ms(7);
             INTCONbits.TMR0IF = 0;
             INTCONbits.TMR0IE = 1;
             LIN_ERROR_FLAGS=TRUE;// somente experiencia.
             ucLinAction=0;
//                 if(!LinEngineBusy)
//                {
//                    TX_SOFT=DOMINANTE;
//                    TMR0L= VLR_TM0;
//                    //INTCONbits.TMR0IE = 1;
//                    b_MeioBitTime=FALSE;
//                    LinEngineBusy=TRUE;
//                    ucByteToTX=pFrameLin[3];//CheckSum
//                    ucLinBitCount = 0;
//                 }
                 break;
                 
//                 if(!LinEngineBusy)
//                {
//                    TX_SOFT=DOMINANTE;
//                    TMR0L= VLR_TM0;
//                   // INTCONbits.TMR0IE = 1;
//                    b_MeioBitTime=FALSE;
//                    LinEngineBusy=TRUE;
//                    ucByteToTX=ucValorCheckSum;
//                    ucLinBitCount = 0;
//                }
//                 
////                TX_SOFT=RECESSIVO;
////                INTCONbits.TMR0IF = 0;
////                INTCONbits.TMR0IE = 0;
////                delay_ms(20);
////                INTCONbits.TMR0IF = 0;
////                INTCONbits.TMR0IE = 1;
////                ucLinAction=0;
//            break;
            
            default:
                //Verefico se o valor do TickTime do Timer0 nao eh
                //maior que um certo valor, se for Desligo o Timer Zero.
               // delay_ms(10);
//                INTCONbits.TMR0IF = 0;
//                INTCONbits.TMR0IE = 0;
//                ImprimeTela=TRUE;
                LIN_ERROR_FLAGS=TRUE;// somente experiencia.
                ucLinAction=0;
            break;
        }
      }
}