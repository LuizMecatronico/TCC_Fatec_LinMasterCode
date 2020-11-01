/******************************************************************************
 * Nome do Arquivo:	displayLCD.c
 * Dependencias:	Veja arquivo "displayLCD.h"
 * Processador:		PIC18F4550
 * Opção de Clock:	HS 20MHz (externo) - CPU: 48MHz
 * Compilador:		C18 
 *
 * Autor:			
 *
 * Data:			v1   - 08/Set/2014 - Ported to XC8 compiler
 *                  v1.1 - 19/Jul/2016 - Changed to CPUFatec v2.0
 *
 *		No projeto da placa o display LCD é ligado às portas
 *		digitais diversas do PIC 18F4550. A placa utiliza
 *		os seguintes pinos para comunicação com o display LCD:
 *			PIC18F4550(pino):: Display LCD
 *				RD4 (27)  	::	  D4
 *				RD5 (28)  	::	  D5
 *				RD6 (29)  	::	  D6
 *				RD7 (30)  	::	  D7
 *				RD1 (20)  	::	  RS
 *				RD0 (19)  	::	  E
 *				RD2 (21)  	::	  R/W
 *	Notas:
 *	- Essa bliblioteca de funçoes pode ser utilizado com qualquer LCD 
 *  16x2 com controladores equivalentes como, o controlador Hitachi 
 *  HD44780 LCD controller.
 *	- O usuário deve definir os seguintes itens:
 *		-- O tipo de interface de comunicação do LDC com o PIC (4 ou 8-bits):
 *		Se for o modo 4-bit, utilizar o upper nibble (nibble é metade de um Byte)
 *		, ou seja, enviar primeiro os 4 bits mais significativos (upper nibble)
 *		e depois os 4 bits menos sgnificativos (lower nibble).
 *
 *		-- Cursor e/ou Blink:
 *		O usuário poderá definir se quer visualizar cursor, blink
 *		ou nenhum dos dois.
 *
 *		-- O sincronismo da comunicação com o LCD:
 *		As temporizações entre o envio de comandos, não foi utilizado
 *		o bit de Busy do LCD, mas sim, através de funções de atraso.
 *****************************************************************************/
#ifndef __DISPLAYLCD_H
#define __DISPLAYLCD_H

/** I N C L U D E S **********************************************************/
#include "Config.h"	

/** D E F I N E S ************************************************************/

// Define dos pinos de DADOS do LCD ligados no PIC18F4550

#define D4	PORTDbits.RD4
#define D5	PORTDbits.RD5
#define D6	PORTDbits.RD6
#define D7	PORTDbits.RD7

// Define do TRIS dos pinos de DADOS do LCD ligados no PIC18F4550
#define TRIS_D4	TRISDbits.RD4
#define TRIS_D5	TRISDbits.RD5
#define TRIS_D6	TRISDbits.RD6
#define TRIS_D7	TRISDbits.RD7

// Define dos pinos de CONTROLE do LCD ligados no PIC18F4550
#define RS_PIN	PORTDbits.RD1
#define E_PIN	PORTDbits.RD0
#define RW_PIN	PORTDbits.RD2

// Define os TRIS dos pinos de CONTROLE do LCD ligados no PIC18F4550
#define TRIS_RS  	TRISDbits.RD1
#define TRIS_E   	TRISDbits.RD0
#define TRIS_RW	 	TRISDbits.RD2

/*** D E F I N E S   D E   C O N F I G U R A C A O ***************************/


/******************************************************************************
 * Define:		LimpaDisplay()
 * Descrição: 	Remove todos os caracteres do display e retorna a posição do
 *				cursor para a linha 1 e coluna 1
 *****************************************************************************/
#define LimpaDisplay()  	EscreveComandoLCD(0b00000001)


/******************************************************************************
 * Define:		CursorHome()
 * Descrição: 	Retorna a posição do cursor para a linha 1 e coluna 1, mas
 *				sem remover todos os caracteres do display.
 *****************************************************************************/
#define CursorHome()  	EscreveComandoLCD(0b00000010)


/******************************************************************************
 * Define:		LigaDisplay()
 * Descrição: 	Liga a tela do display e desliga o cursor do visor.
 *****************************************************************************/
#define LigaDisplay()  		EscreveComandoLCD(0b00001100)


/******************************************************************************
 * Define:		DesligaDisplay()
 * Descrição: 	Desliga a tela do display, mas não remove os caracteres da  
 *				memória, nem apaga o backlight.
 *****************************************************************************/
#define DesligaDisplay()  	EscreveComandoLCD(0b00001000)


/******************************************************************************
 * Define:		LigaCursor()
 * Descrição: 	Faz com que o cursor apareça na tela, e se a mesma estiver
 *				desligada, será ligada.
 *****************************************************************************/
#define LigaCursor()		EscreveComandoLCD(0b00001110)


/******************************************************************************
 * Define:		DesligaCursor()
 * Descrição: 	Faz com o cursor suma da tela e se a mesma estiver
 *				desligada, será ligada.
 *****************************************************************************/
#define DesligaCursor()		EscreveComandoLCD(0b00001100)


/******************************************************************************
 * Define:		PiscaCursor()
 * Descrição: 	Faz com que o cursor apareça no LCD e fique piscando. Se a 
 *				tela estiver desligada, será ligada.
 *****************************************************************************/
#define PiscaCursor()		EscreveComandoLCD(0b00001111)


/******************************************************************************
 * Define:		DeslocaCursorDir()
 * Descrição: 	Faz com que o cursor desloque na tela uma posição à direita
 *****************************************************************************/
#define DeslocaCursorDir()		EscreveComandoLCD(0b00010100)


/******************************************************************************
 * Define:		DeslocaCursorEsq()
 * Descrição: 	Faz com que o cursor desloque na tela uma posição à esquerda
 *****************************************************************************/
#define DeslocaCursorEsq()		EscreveComandoLCD(0b00010000)


/******************************************************************************
 * Define:		DeslocaDisplayDir()	
 * Descrição: 	Faz com que a tela desloque uma posição à direita
 *****************************************************************************/
#define DeslocaDisplayDir()		EscreveComandoLCD(0b00011100)


/******************************************************************************
 * Define:		DeslocaDisplayEsq()	
 * Descrição: 	Faz com que a tela desloque uma posição à esquerda
 *
 *****************************************************************************/
#define DeslocaDisplayEsq()		EscreveComandoLCD(0b00011000)


/**  P R O T O T I P O  D E  F U N C O E S  **********************************/

/******************************************************************************
 * Funcao:		void ConfiguraLCD(void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Configura portas do PIC 18F4550 que estão conectadas ao 
 *	display LCD como saida. Essa rotina é baseada no controlador de LCD 
 *  Hitachi HD44780. Configura os parametros de comunicação com o display,
 *  tais como:
 *	- modo de operação (4 ou 8 data bits)
 *	- tipo de cursor
 *	- tamanho da fonte
 *	Ao final da configuração limpa o display.
 *****************************************************************************/
extern void ConfiguraLCD(void);


/******************************************************************************
 * Funcao:		void EscreveComandoLCD(unsigned char cmd)
 * Entrada:		cmd: comando que será enviado para o LCD
 * Saída:		Nenhuma (void)
 * Descrição:	(Write a command to the LCD)
 *	Escreve um comando para o display de LCD.
 *****************************************************************************/
extern void EscreveComandoLCD(unsigned char cmd);


/******************************************************************************
 * Funcao:		void EnderecoCursor(unsigned char endereco)
 * Entrada:		endereco: endereço da memoria de dados do display que se 
 *				colocar o cursor
 * Saída:		Nenhuma (void)
 * Descrição:	(Set the Display Data RAM Address)
 *	Essa rotina posiciona o cursor no endereço desejado da memoria RAM do 
 *	display LCD. Os comandos de escrita seguintes vão começar a partir da
 *	posição escolhida por esta função
 *****************************************************************************/
extern void EnderecoCursor(unsigned char endereco);


/******************************************************************************
 * Funcao:		void PosicaoCursorLCD(unsigned char linha, unsigned char coluna)
 * Entrada:		linha: número de 1 à 4 da linha em que se deseja deixar o cursor
 *
 *				coluna: número de 1 à 16 coluna linha em que se deseja deixar o cursor
 *
 * Saída:		Nenhuma (void)
 * Descrição:	Essa rotina posiciona o cursor na tela do display, na linha 
 *	e coluna desejados. Os comandos de escrita seguintes vão começar a partir
 *  da posição escolhida por esta função
 *****************************************************************************/
extern void PosicaoCursorLCD(unsigned char linha, unsigned char coluna);


/******************************************************************************
 * Funcao:		void EscreveCaractereLCD(unsigned char data)
 * Entrada:		data: Byte de dados para ser escrito na memoria RAM do LCD
 * Saída:		Nenhuma (void)
 * Descrição:	(Write a Data byte to the LCD)
 *	Essa rotina escreve o byte de caracteres no display a partir da posição
 *	de memoria atual do cursor ou da posição de memória	escolhida com as
 *	funções PosicaoCursorLCD() ou EnderecoCursor().
 *****************************************************************************/
extern void EscreveCaractereLCD(char data);


/******************************************************************************
 * Funcao:		void EscreveInteiroLCD(int valor)
 * Entrada:		valor: numero inteiro com sinal (de -32768 a 32767) a ser escrito no
 *				display.
 *
 * Saída:		Nenhuma (void)
 * Descrição:	Essa rotina escreve o numero inteiro no display a partir da posição
 *	de memoria atual do cursor ou da posição de memória escolhida com as
 *	funções PosicaoCursorLCD() ou EnderecoCursor().
 *****************************************************************************/
//extern void EscreveInteiroLCD(unsigned  int valor);
extern void EscreveInteiroLCD(const unsigned int uiValor);


extern void EscreveADC10bitsOnLCD(const unsigned int uiValor);
/******************************************************************************
 * Funcao:		void EscreveFloatLCD(float valor, char formatacao)
 * Entrada:		valor: numero float com sinal (de -9999.9999 a 9999.9999) que
 *				será escrito no display
 *
 *				formatacao: numero de casas decimais desejadas após a 
 *				virgula (de 0 à 4 casas decimais).
 *
 * Saída:	Nenhuma (void)
 * Descrição:	Essa rotina escreve o numero float no display a partir da posição
 *	atual do cursor com quantas casas decimais forem definidas no
 *  parâmetro formatacao.
 *****************************************************************************/
extern void EscreveFloatLCD(float valor, char formatacao);


/******************************************************************************
 * Funcao:		void EscreveFraseRamLCD(char *buffer)
 * Entrada:		*buffer: apontador para string na memoria de dados(RAM)
 * Saída:		Nenhuma (void)
 * Descrição:	Essa rotina copia uma string terminada com caracter nulo
 *	da memoria de dados do microcontrolador para o display de LCD.
 *	A string apontada por *buffer é copiada no display a partir da posição
 *  de memoria atual do cursor ou da posição de memória
 *  escolhida com as funções PosicaoCursorLCD() ou EnderecoCursor().
 *****************************************************************************/
extern void EscreveFraseRamLCD(char *buffer);


/******************************************************************************
 * Funcao:		void EscreveFraseLCD(const char *buffer)
 * Entrada:		*buffer: apontador para string na memoria de programa(ROM)
 * Saída:		Nenhuma (void)
 * Descrição:	Essa rotina copia uma string terminada com caracter nulo
 *	da memoria de programa do microcontrolador para o display de LCD.
 *	A string apontada por *buffer é copiada no display a partir da
 *	posição de memoria atual do cursor ou da posição de memória escolhida
 *	com as funções PosicaoCursorLCD() ou EnderecoCursor().
 *****************************************************************************/
extern void EscreveFraseLCD(const char *buffer);

extern unsigned char ConverteToHx(unsigned char x);

/******************************************************************************
 * Funcao:		void CriaCaractereLCD(unsigned char endereco, const char *buffer)
 * Entrada:		endereco: Numero de 0 à 7 que indica a posição da CGRAM que o 
 *				caractere será criado.
 *
 *				*buffer: apontador para o vetor de char com tamanho 8 que 
 *				guarda cada linha de desenho do caractere que será criado.
 *
 * Saída:		Nenhuma (void)
 * Descrição:   Essa rotina cria um caractere de usuário em uma das 7 posições
 *	disponiveis na CGRAM do display. O caractere criado tem tamanho 8x5 pixel
 *  (8 linhas e 5 colunas), portanto cada valor do vetor buffer representa
 *  uma linha, onde os 3 bits mais significativos de cada valor é desprezado.
 *****************************************************************************/
extern void CriaCaractereLCD(unsigned char endereco, const char *buffer);


#endif
