/*******************************************************************************
 * File:  PGA450-Q1EVM-S - FATEC SANTO ANDRE
 * Processor: PIC18F4550
 * Compiler: MPLAB X v3.65 + XC8
 * Company: FATEC Santo Andre
 * Author: Luiz Carlos da Silva
 * Email: luiz.silva193@fatec.sp.gov.br
 * Orientador Prof. Weslley Medeiros Torres.
 * Created on 12 de Dezembro de 2019, as 20:01hs
 * *****************************************************************************
 * File Description: Este programa objetiva o interfaceamento com o EVM
 * PGA450-Q1EVM-S da Texas Instruments e tem como objetivo ser o código
 * "LinMaster" do projeto de BSD (Blind Spot Detector), do meu TCC do 
 * curso de "Tecnologia em Eletrônica Automotiva". 
 * ****************************************************************************/


#include <pic18f4550.h>
#include "Config.h"
#include "displayLCD.h"
#include "LinEngine.h"

//Define outros pinos de uso geral
#define Botao_B1    PORTEbits.RE0 
#define Botao_B2    PORTEbits.RE1
#define Botao_B3    PORTEbits.RE2
#define IO_BUZZER   PORTCbits.RC2
//#define IO_LED      PORTAbits.RA2

bit bErroEscrita,b_CheckSumOK;
bit bModo_Distancia,bLatch;


unsigned char ucMsgToRX[10]=0;
unsigned char ucByteRecebido=0;
unsigned char ucBotaoPressionado=0;

//******************************************************************************
//O protocolo implementado para a Engine LIN é o seguinte: o primeiro byte
//do array a ser transmitido Byte[0], contem as informações de 
//quantos bytes devem ser transmitidos, ou transmitidos e recebidos.
//Por exemplo: No nibble BAIXO, é a quantidade a ser transmitida (incluido
//o Synch),e no nibble ALTO, é a quantidade a ser recebida. Quando na 
//transmissão, o último byte deverá ser o CHECKSUM, ja calculado:

//Exemplo:
//ucMsgSetThresHoldLongRange [4] = {0x04,0x11,0x03,0xEB};
//Na transmissão do frame acima: existem 4 bytes há serem transmitidos,
//incluindo o synch, ou seja, o 0x55, o 0x11(PID), o 0x00 (Um único byte) e o 
//0xEE (CheckSum) que já foi previamente calculado. Como o nible ALTO do byte[0]
//contém o valor zero, não é esperado nenhum byte há ser recebido.

//Exemplo:
//ucMsgGetTimeOfFligh[2] = {0x32,0xE2};
//Já no frame acima, existem dois bytes a serem transmitidos, o 0x55 e o 
//0xE2 (PID). Mas é esperado 3 bytes na recepção, incluindo o CheckSum que o
//Slave(PGA40-Q1) enviará.

// Dados validos para comandar o SOC, a ser melhorado.
                    //			0 - Listen mode ;
                    //			1 - Short Distance Detection;
			        //          2 - Media Distance Detection;
                    //			3 - Long Distance Detection;
//******************************************************************************
unsigned char ucMsgSetThresHoldListenMode  [4] = {0x04,0x11,0x00,0xEE};//ListenMode
unsigned char ucMsgSetThresHoldShortRange  [4] = {0x04,0x11,0x01,0xED};//ShortRange
unsigned char ucMsgSetThresHoldMediaRange  [4] = {0x04,0x11,0x02,0xEC};//MediaRange
unsigned char ucMsgSetThresHoldLongRange   [4] = {0x04,0x11,0x03,0xEB};//LongRange

unsigned char ucMsgGetTimeOfFligh[2] = {0x32,0xE2};

unsigned int uiValorADC=0;



//------------------------------------------------------------------------------
void interrupt high_priority Interrupcoes(void)
{
   if( INTCON3bits.INT2IE &&  INTCON3bits.INT2IF)
    {
        TMR0L= VLR_TM0;
        INTCONbits.TMR0IF = 0;
        INTCONbits.TMR0IE = 1;
        END_BIT=FALSE;
        INTCON3bits.INT2IF=0;
        INTCON3bits.INT2IE=0;
        TRANSMISSAO = FALSE;
        b_MeioBitTime=TRUE;
        startOfByte=TRUE;
        ucByteRecebido=0x00;
        ucLinBitCount=0;
    }
  //ucBotaoPressionado++;
    
    //Esta interrupção ocorre a cada 26us com o TMR0L=180 PRE=4 CicloMaq=12MHz
    //Que é a metade do BitTime de uma LIN com baud de 19.2Kbps
    if(INTCONbits.TMR0IF && INTCONbits.TMR0IE)
    {
        INTCONbits.TMR0IF=0;
        ui_ContadorMeioBitTime++;
        //*********************************************************************
        //              TRANSMISSAO DOS BYTES ENVIADOS PELO LIN_MASTER (PIC)
       
        if(TRANSMISSAO)
        {
          if(b_MeioBitTime)
            {
              if(ucLinBitCount < 8)
                {
                    LinEngineBusy=TRUE;
                    b_BitEscrita = (ucByteToTX >> ucLinBitCount) & 0x01;
                    
                    b_BitEscrita ? TX_SOFT = RECESSIVO :
                                   TX_SOFT = DOMINANTE ;
                    ucLinBitCount++;
                    END_BIT=FALSE;
                }
                else   //Todos os Bits dessa sequencia foram transmitidos
                {
                    if(!END_BIT)
                    {
                        ucLinAction++;
                        TX_SOFT=RECESSIVO;
                        END_BIT=TRUE;
                        
                        if(!ucByteSizeToTX)
                            ucLinAction=DATA_RX;//experiencias...................
                    }
                    else
                    {
                        LinEngineBusy=FALSE;
                        END_BIT=FALSE;
                    }
                }
            }
            else
            {   //Esse bit abaixo "bErroEscrita", será parte de um dos 8 bits
                //da variavel de Erro, aqui neste código nomeada como:
                //LIN_ERROR_FLAGS Hoje 05/11/2020.
                if(RX_SOFT != b_BitEscrita) //OBS: LER ESCREVER LAT PORT
                             bErroEscrita=TRUE;
                
            }
        }
        //*********************************************************************
        //              RECEPCAO DOS BYTES ENVIADOS PELO LIN_SLAVE (PGA450-Q1)
        else
        {
            if(b_MeioBitTime)
            {
                if((ucLinBitCount < 8) && !startOfByte) 
                {
                    if(RX_SOFT)
                    ucByteRecebido+=(RECESSIVO << ucLinBitCount);
                    ucLinBitCount++;
                }
                else
                {
                   if(!startOfByte)
                    {
                        ucMsgToRX[ucByte_RX]=ucByteRecebido;
                        ucByte_RX++;
                        INTCONbits.TMR0IE =0;//Desliga a Interrupcao do TMR0.
                        INTCON3bits.INT2IF=0;//Limpo a Flag da Interr. EXT2.
                        INTCON3bits.INT2IE=1;//LIGO a Interrupcao do EXT2.
                    }
                    startOfByte=FALSE;
                }
            }
        }
        //*********************************************************************
        b_MeioBitTime = ~b_MeioBitTime;
       
        if(!END_BIT)
             TMR0L= VLR_TM0 +TMR0L; //Valor do estouro + resíduo 
        else   
             TMR0L= BIT_TM0 +TMR0L;
    }    
}


//******************************************************************************
//                          Funcao  CalculaCheckSum
//******************************************************************************
unsigned char CalculaCheckSum(unsigned char * pArrayToTx,
                              unsigned char ucByteCount)
{
    unsigned char ucCount;
    union { 
            unsigned int   u16;
            unsigned char  u8[2];
          }Sum;
                    
          Sum.u16=0x0000;
          
          for(ucCount=0;ucCount<ucByteCount;ucCount++)
          {
              Sum.u16+=(unsigned int)(*pArrayToTx);//lllllllllllllllllllllllllllllllllllllllllllllllllll
              pArrayToTx++;
          }

          return   (Sum.u8[0]!=0) ?
                   Sum.u8[1]+Sum.u8[0]:
                   Sum.u8[1];
}
//******************************************************************************
//        Funcao  ConverteExibeTimeofFlightToDistancia(ToF) em Distancia
//******************************************************************************

void ConverteExibeTimeofFlightToDistancia(void)
{
      unsigned long int ulDistanciaMedida;
      unsigned int uiDistanciaMedida;
            
      ulDistanciaMedida =(unsigned long int)
                      TimeOfFlight.u16*3431;  //343,1 metros/segundos  @20C 
      ulDistanciaMedida =(unsigned long int)
                      (ulDistanciaMedida/200);
      uiDistanciaMedida =(unsigned int)
                     (ulDistanciaMedida/1000);

      PosicaoCursorLCD(2,1);
      EscreveFraseRamLCD("ToF: ");
      EscreveInteiroLCD(TimeOfFlight.u16);
     
      PosicaoCursorLCD(2,12);
      unsigned int  uiNum = uiDistanciaMedida;
      unsigned char ucDec, ucUnd;
                   
      if(uiDistanciaMedida>99)
        {
            uiNum=uiDistanciaMedida/100;
            EscreveCaractereLCD(uiNum+0x30);
            uiNum=uiDistanciaMedida%100;
        }
      else 
        {
            EscreveCaractereLCD(0x30);
        }
                   
        EscreveCaractereLCD(',');  
                   
        ucDec=uiNum/10;
        EscreveCaractereLCD(ucDec+0x30);
        ucUnd=uiNum%10;
        EscreveCaractereLCD(ucUnd+0x30);
        EscreveCaractereLCD('m');
        // ImprimeTela=FALSE;
}


//****************************************************************************
//                          PROGRAMA PRINCIPAL
//****************************************************************************

void main(void)
{
//  union{
//        unsigned short u16;
//        unsigned char u8[2];
//       } TimeOfFlight;
   
//  unsigned long int ulDistanciaMedida;
//  unsigned int uiDistanciaMedida;///verificar erro aqui. hj 07/09/2020
  ConfigHw();
   
  TMR0L  = VLR_TM0; //Valor de estouro a cada 26us Aproximadamente.
  CS_LIN = FALSE;    
    
  ConfiguraLCD();
    
  DesligaCursor();
  LimpaDisplay();
  PosicaoCursorLCD(1,1);
  EscreveFraseRamLCD("Projeto TCC BSD ");

  PosicaoCursorLCD(2,1);
  EscreveFraseRamLCD("PGA450-Q1 EVM-S");
  __delay_ms(3000);
    
  LimpaDisplay();
  PosicaoCursorLCD(1,1);
  EscreveFraseRamLCD("MODO: Long Range");
    
  CS_LIN=1;
  LinEngineBusy=FALSE;
  //LED_LIFE=FALSE;
  bModo_Distancia=TRUE;
  //bLatch=FALSE;
  b_diagnostico=FALSE;//*****************************************************
  
  TimeOfFlight.u16=00;
  
  while(1)
   { 
//       //SELECIONA O MODO DE DISTANCIA, SE LONGA OU CURTA
//       if(!(Botao_B1)&& !bLatch)
//       {
//           if((ucBotaoPressionado > 100)&&(!bLatch))
//           {
//                b_diagnostico=!b_diagnostico;//*****************************
//                bModo_Distancia=!bModo_Distancia;
//                ucBotaoPressionado=0x00;
//                bLatch=TRUE;
//                LimpaDisplay();
//           }
//       }
//       else if(Botao_B1&& bLatch)
//       {
//            ucBotaoPressionado=0x00;
//            bLatch=FALSE;
//       }
//       else  ucBotaoPressionado=0x00;
//       //********************************************
        

       unsigned char ucCountRX;
       for(ucCountRX=0 ; ucCountRX<10 ; ucCountRX++)
        
       ucMsgToRX[ucCountRX]=0x00;
        
       if(bModo_Distancia)
           LinEngine(ucMsgSetThresHoldLongRange);
       else   
           LinEngine(ucMsgSetThresHoldShortRange);
        
       //O delay abaixo, é provisório.
       __delay_ms(200);/* Caso comente esse delay aqui, de 200ms, a resposta do Slave ocorre,
                        * mas o "SOC" nao teve tempo de ler a distancia e envia o valor presente
                        * nos registradores no momento, que nos testes feitos foram zero, embora
                        * depois de feita uma atribuicao forçada o soc enviou os valores atribuidos,
                        * oque indica que o mesmo não teve tempo de apurar o tempo de retorno do 
                        * sinal(ToF). Hj 04/11/2020 */
       __delay_ms(800);//Comentar ou remover esse delay aqui.
       
       LinEngine(ucMsgGetTimeOfFligh);
       
       //Armazenamento do PID pra calculo do CheckSum Recebido.
       ucMsgToRX[0]=ucMsgGetTimeOfFligh[1];
 
       //IO_LED=~IO_LED;
       //__delay_ms(300);//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
       //IO_BUZZER=0;

       TimeOfFlight.u8[0] =(unsigned char) ucMsgToRX[2];
       TimeOfFlight.u8[1] =(unsigned char) ucMsgToRX[1];
       
       unsigned char ucCheckSumCalculado = 
                         ~CalculaCheckSum(ucMsgToRX,3);

       
       ucCheckSumCalculado!=ucMsgToRX[3] ?
            b_CheckSumOK=FALSE: 
            b_CheckSumOK=TRUE; 

       if(ImprimeTela)
       {
            if(bModo_Distancia)
            {
            PosicaoCursorLCD(1,1);
            EscreveFraseRamLCD("MODO->Long Range ");
            }
            else
            {
            PosicaoCursorLCD(1,1);
            EscreveFraseRamLCD("MODO->Short Range");
            }
            //TimeOfFlight.u16++;
            //TimeOfFlight.u16=65535; //11,29m 
            //TimeOfFlight.u16=14938; // 2,56m     
            //TimeOfFlight.u16=23456; // 4,02m
            //TimeOfFlight.u16=39457; // 6,76m
            //TimeOfFlight.u16=5798;  // 0,99m
            
            //A função de conversão de microsegundos(us) pra metros, foi 
            //preparada pra converter e exibir a distancia no máximo de 9,99m
            //ou seja, uma unidade inteira(m) e 99 centésimos de metro(cm).
            //Logo um TimeOfFlight que resulte em valores maiores doque
            //9,99 metros não deverá ser convertida e muito menos exibida.
            if(TimeOfFlight.u16 > 58291)// || TimeOfFlight.u16 < 2914)
            { 
                PosicaoCursorLCD(2,1);
                EscreveFraseRamLCD("ToF: ");
                EscreveInteiroLCD(TimeOfFlight.u16);
                PosicaoCursorLCD(2,12);
                EscreveFraseRamLCD("-,--");
            }
            else
            {
                  ConverteExibeTimeofFlightToDistancia();
//                ulDistanciaMedida =(unsigned long int)
//                                TimeOfFlight.u16*3431;//343.1 @20C 
//                ulDistanciaMedida =(unsigned long int)
//                                (ulDistanciaMedida/200);//676,68755
//                uiDistanciaMedida =(unsigned int)
//                                (ulDistanciaMedida/1000);//676,68755
//
//                PosicaoCursorLCD(2,1);
//                EscreveFraseRamLCD("ToF: ");
//                EscreveInteiroLCD(TimeOfFlight.u16);
//       
//                PosicaoCursorLCD(2,12);
//                unsigned int  uiNum = uiDistanciaMedida;
//                unsigned char ucDec, ucUnd;
//                    
//                if(uiDistanciaMedida>99)
//                {
//                    uiNum=uiDistanciaMedida/100;
//                    EscreveCaractereLCD(uiNum+0x30);
//                    uiNum=uiDistanciaMedida%100;
//                }
//                else 
//                {
//                    EscreveCaractereLCD(0x30);
//                }
//                   
//                EscreveCaractereLCD(',');  
//                   
//                ucDec=uiNum/10;
//                EscreveCaractereLCD(ucDec+0x30);
//                ucUnd=uiNum%10;
//                EscreveCaractereLCD(ucUnd+0x30);
//                EscreveCaractereLCD('m');
//                ImprimeTela=FALSE;
            }
        }   
        //VERIFICA SE O TIMER ZERO ESTA CONTANDO
        //if(ui_ContadorMeioBitTime!=
        //      Anterior_ui_ContadorMeioBitTime++)
        
      
        //__delay_ms(1000);//88888888888888888888888888888888888888888888888888888888
        IO_BUZZER=0;
        __delay_ms(100);
        IO_BUZZER=0;
        if(b_diagnostico)
        {
            //Ler entrada analogica AN1.
            TRISAbits.TRISA1 = 1; 
            //-------------------------------------------------------------------------
            //PG262/430 Do manual do PIC18F4550
            //PCFG3:PDFG0  [AN12 -> AN0]
            //0000--> Todos analogicos
            //1111--> Todos Digitais
            //1110--> Somente AN0
            //1101--> Somente AN0 e AN1
            //ADCON1bits.PCFG3 = 1;
            //ADCON1bits.PCFG2 = 1;
            
            ADCON1bits.PCFG1 = 0;
            ADCON1bits.PCFG0 = 1;
            ADCON0bits.CHS=0x01; //Ativa leitura do AN1
            __delay_us(20);
            ADCON0bits.GO =1;
             
            PosicaoCursorLCD(2,11);
            while(ADCON0bits.GO_DONE);
            uiValorADC=(ADRESH<<8) + ADRESL; //O valor lido fica entre 138 e 141.
                                             //Na media de 140.
            EscreveADC10bitsOnLCD(uiValorADC);
            //Caso valor menor doque 100, provavelmente o fusível se encontra
            //aberto, e exibira a seguinte mensagem:
            //Linha 1 "Verifiq. Fusível"
            //Linha 2 " ****ABERTO**** "
        }
        else
         {
            TRISAbits.TRISA1 = 0; 
            ADCON1bits.PCFG1 = 1;
            ADCON1bits.PCFG0 = 0;
            if(b_CheckSumOK)
                LED_STATUS=1;
            else
            LED_STATUS=0;
         }
    
    }
}

                
            
        
    
