/*
 *  Sobre o algoritmo e seu desenvolvimento:
    Algoritmo implementado para plataforma ARM/Linux, na matéria de Sistemas Embarcados, com o objetivo inicial de
    ser compilado através do Cross Compiler e ser executado no ARM de uma raspberry Pi 3 (utilizando o modelo Host/
    Target).
    Esse algoritmo encripta uma mensagem  utilizando a cifra de Rail Fense com rails = 10, que utiliza a estratégia das cifras
    de transposição. Esta estratégia consiste em alterar a ordem dos caracteres sem alterar o tamanho da mensagem.
    Portanto tem-se um texto de entrada que é lido de um arquivo, e o texto cifrado é escrito em um arquivo de saída.
    Por padrão passamos um texto de 3000 caracteres para atender os requisitos do projeto. A entrada pode ser
    modificada apenas alterando o arquivo, sem precisar compilar novamente.
 *  Autores : Gabriel Andrade Palhares e Guilherme Araujo da Silva
 *
 *  Copyright (C) 2018 Gabriel Andrade Palhares <gaapalhares@gmail.com>
 *  Copyright (C) 2018 Guilherme Araujo da Silva  <guiaszul@gmail.com> *
 *  Data de Atualização : 01 de outubro de 2018
 *  Modo de uso:
        Após inserir a mensagem em um arquivo "texto_claro.txt" basta gerar o compilado e executar com o arquivo na
        mesma pasta do compilado. Assim será gerado arquivos "texto_cifra_rail_fense.txt" com a mensagem cifrada.
 *  Restrição de uso:
        Os valores de entrada devem ser inseridos em um arquivo .txt com o título de "texto_claro.txt" para um bom
        funcionamento.
    Código usado como referência: https://www.thecrazyprogrammer.com/2017/09/rail-fence-cipher-program-in-c.html
*/

// CONFIG1
#pragma config FEXTOSC = OFF    // External Oscillator mode selection bits (Oscillator not enabled)
#pragma config RSTOSC = HFINT32 // Power-up default value for COSC bits (HFINTOSC with OSCFRQ= 32 MHz and CDIV = 1:1)
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled; i/o or oscillator function on OSC2)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (FSCM timer enabled)

// CONFIG2
#pragma config MCLRE = ON       // Master Clear Enable bit (MCLR pin is Master Clear function)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config LPBOREN = OFF    // Low-Power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = ON       // Brown-out reset enable bits (Brown-out Reset Enabled, SBOREN bit is ignored)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (VBOR) set to 1.9V on LF, and 2.45V on F Devices)
#pragma config ZCD = OFF        // Zero-cross detect disable (Zero-cross detect circuit is disabled at POR.)
#pragma config PPS1WAY = ON     // Peripheral Pin Select one-way control (The PPSLOCK bit can be cleared and set only once in software)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will cause a reset)

// CONFIG3
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF       // WDT operating mode (WDT Disabled, SWDTEN is ignored)
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)

// CONFIG4
#pragma config WRT = OFF        // UserNVM self-write protection bits (Write protection off)
#pragma config SCANE = available// Scanner Enable bit (Scanner module is available for use)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low Voltage programming enabled. MCLR/Vpp pin function is MCLR.)

// CONFIG5
#pragma config CP = OFF         // UserNVM Program memory code protection bit (Program Memory code protection disabled)
#pragma config CPD = OFF        // DataNVM code protection bit (Data EEPROM code protection disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _XTAL_FREQ 32000000

//const char var[6144];
//const int var2[998];
//const char vailogo[10][614];    
//const char array[2][3000]; - 74
//const char texto[496] =  {
//	'N', 'o', 'w', ',', ' ', 'a', 'f', 't', 'e', 'r', ' ', 't', 'h', 'e', ' ', 'e', 'n', 's', 'u', 'i', 'n', 'g', ' ', 'a', 'v', 'a', 'l', 'a', 'n', 'c', 'h', 'e', ' ', 'o', 'f', ' ', 's', 'e', 'q', 'u', 'e', 'l', 's', ',', ' ', 'p', 'r', 'e', 'q', 'u', 'e', 'l', 's', ' ', 'a', 'n', 'd', ' ', 's', 'i', 'd', 'e', ' ', 's', 't', 'o', 'r', 'i', 'e', 's', ',', ' ', 't', 'h', 'e', ' ', 's', 'a', 'g', 'a', ' ', 'o', 'f', ' ', 'K', 'r', 'a', 't', 'o', 's', ' ', 's', 't', 'a', 'r', '-', 'c', 'r', 'o', 's', 's', 'e', 'd', ' ', 'l', 'i', 'f', 'e', ' ', 'h', 'a', 's', ' ', 'b', 'e', 'c', 'o', 'm', 'e', ' ', 'a', 's', ' ', 't', 'a', 'n', 'g', 'l', 'e', 'd', ' ', 'a', 'n', 'd', ' ', 'c', 'o', 'm', 'p', 'l', 'e', 'x', ' ', 'a', 's', ' ', 't', 'h', 'e', ' ', 'm', 'y', 't', 'h', 'i', 'c', ' ', 't', 'a', 'l', 'e', 's', ' ', 't', 'h', 'a', 't', ' ', 'i', 'n', 's', 'p', 'i', 'r', 'e', 'd', ' ', 't', 'h', 'e', ' ', 'w', 'o', 'r', 'l', 'd', ' ', 'h', 'e', 's', ' ', 'i', 'n', 'h', 'a', 'b', 'i', 't', 'e', 'd', '.', ' ', 'A', 'n', 'd', ' ', 't', 'h', 'o', 'u', 'g', 'h', ' ', 't', 'h', 'e', ' ', 'u', 'p', 'c', 'o', 'm', 'i', 'n', 'g', ' ', 'P', 'l', 'a', 'y', 'S', 't', 'a', 't', 'i', 'o', 'n', ' ', '4', ' ', 'f', 'o', 'l', 'l', 'o', 'w', '-', 'u', 'p', ' ', 'd', 'o', 'e', 's', ' ', 'i', 't', 's', ' ', 'b', 'e', 's', 't', ' ', 't', 'o', ' ', 'l', 'e', 'a', 'v', 'e', ' ', 't', 'h', 'e', ' ', 'l', 'y', 'r', 'e', 's', ' ', 'a', 'n', 'd', ' ', 'w', 'i', 'n', 'g', 'e', 'd', ' ', 'c', 'h', 'a', 'r', 'i', 'o', 't', 's', ' ', 'o', 'f', ' ', 'f', 'a', 'n', 't', 'a', 's', 'y', ' ', 'a', 'n', 'c', 'i', 'e', 'n', 't', ' ', 'G', 'r', 'e', 'e', 'c', 'e', ' ', 'i', 'n', ' ', 't', 'h', 'e', ' ', 'r', 'e', 'a', 'r', 'v', 'i', 'e', 'w', ' ', 'm', 'i', 'r', 'r', 'o', 'r', ',', ' ', 'h', 'e', 'r', 'e', 's', ' ', 'a', ' ', 'q', 'u', 'i', 'c', 'k', ' ', 'p', 'r', 'i', 'm', 'e', 'r', ' ', 'o', 'n', ' ', 'a', 'l', 'l', ' ', 't', 'h', 'e', ' ', 'm', 'a', 'j', 'o', 'r', ' ', 'r', 'e', 'v', 'e', 'l', 'a', 't', 'i', 'o', 'n', 's', ' ', 'f', 'r', 'o', 'm', ' ', 't', 'h', 'e', ' ', 'p', 'r', 'e', 'v', 'i', 'o', 'u', 's', ' ', 'g', 'a', 'm', 'e', 's', ',', ' ', 'a', 'n', 'd', ' ', 'h', 'o', 'w', ' ', 't', 'h', 'e', 'y', ' ', 's', 'h', 'a', 'p', 'e', 'd', ' ', 't', 'h', 'e', ' ', 'a', 'n', 'g', 'r', 'y', ' ', 'a', 'n', 't', 'i', 'h', 'e', 'r', 'o', ' ', 'a', 't', ' ', 't', 'h', 'e', ' ', 'h', 'e', 'l', 'm', '.', ' ', 'G', 'a', 'b', 'r', 'i', 'e', 'l', ' ', 'P', 'a', 'l', 'h', 'a', 'r', 'e', 's'
//};
const char texto[3000] =  {
    'N', 'o', 'w', ',', ' ', 'a', 'f', 't', 'e', 'r', ' ', 't', 'h', 'e', ' ', 'e', 'n', 's', 'u', 'i', 'n', 'g', ' ', 'a', 'v', 'a', 'l', 'a', 'n', 'c', 'h', 'e', ' ', 'o', 'f', ' ', 's', 'e', 'q', 'u', 'e', 'l', 's', ',', ' ', 'p', 'r', 'e', 'q', 'u', 'e', 'l', 's', ' ', 'a', 'n', 'd', ' ', 's', 'i', 'd', 'e', ' ', 's', 't', 'o', 'r', 'i', 'e', 's', ',', ' ', 't', 'h', 'e', ' ', 's', 'a', 'g', 'a', ' ', 'o', 'f', ' ', 'K', 'r', 'a', 't', 'o', 's', ' ', 's', 't', 'a', 'r', '-', 'c', 'r', 'o', 's', 's', 'e', 'd', ' ', 'l', 'i', 'f', 'e', ' ', 'h', 'a', 's', ' ', 'b', 'e', 'c', 'o', 'm', 'e', ' ', 'a', 's', ' ', 't', 'a', 'n', 'g', 'l', 'e', 'd', ' ', 'a', 'n', 'd', ' ', 'c', 'o', 'm', 'p', 'l', 'e', 'x', ' ', 'a', 's', ' ', 't', 'h', 'e', ' ', 'm', 'y', 't', 'h', 'i', 'c', ' ', 't', 'a', 'l', 'e', 's', ' ', 't', 'h', 'a', 't', ' ', 'i', 'n', 's', 'p', 'i', 'r', 'e', 'd', ' ', 't', 'h', 'e', ' ', 'w', 'o', 'r', 'l', 'd', ' ', 'h', 'e', 's', ' ', 'i', 'n', 'h', 'a', 'b', 'i', 't', 'e', 'd', '.', ' ', 'A', 'n', 'd', ' ', 't', 'h', 'o', 'u', 'g', 'h', ' ', 't', 'h', 'e', ' ', 'u', 'p', 'c', 'o', 'm', 'i', 'n', 'g', ' ', 'P', 'l', 'a', 'y', 'S', 't', 'a', 't', 'i', 'o', 'n', ' ', '4', ' ', 'f', 'o', 'l', 'l', 'o', 'w', '-', 'u', 'p', ' ', 'd', 'o', 'e', 's', ' ', 'i', 't', 's', ' ', 'b', 'e', 's', 't', ' ', 't', 'o', ' ', 'l', 'e', 'a', 'v', 'e', ' ', 't', 'h', 'e', ' ', 'l', 'y', 'r', 'e', 's', ' ', 'a', 'n', 'd', ' ', 'w', 'i', 'n', 'g', 'e', 'd', ' ', 'c', 'h', 'a', 'r', 'i', 'o', 't', 's', ' ', 'o', 'f', ' ', 'f', 'a', 'n', 't', 'a', 's', 'y', ' ', 'a', 'n', 'c', 'i', 'e', 'n', 't', ' ', 'G', 'r', 'e', 'e', 'c', 'e', ' ', 'i', 'n', ' ', 't', 'h', 'e', ' ', 'r', 'e', 'a', 'r', 'v', 'i', 'e', 'w', ' ', 'm', 'i', 'r', 'r', 'o', 'r', ',', ' ', 'h', 'e', 'r', 'e', 's', ' ', 'a', ' ', 'q', 'u', 'i', 'c', 'k', ' ', 'p', 'r', 'i', 'm', 'e', 'r', ' ', 'o', 'n', ' ', 'a', 'l', 'l', ' ', 't', 'h', 'e', ' ', 'm', 'a', 'j', 'o', 'r', ' ', 'r', 'e', 'v', 'e', 'l', 'a', 't', 'i', 'o', 'n', 's', ' ', 'f', 'r', 'o', 'm', ' ', 't', 'h', 'e', ' ', 'p', 'r', 'e', 'v', 'i', 'o', 'u', 's', ' ', 'g', 'a', 'm', 'e', 's', ',', ' ', 'a', 'n', 'd', ' ', 'h', 'o', 'w', ' ', 't', 'h', 'e', 'y', ' ', 's', 'h', 'a', 'p', 'e', 'd', ' ', 't', 'h', 'e', ' ', 'a', 'n', 'g', 'r', 'y', ' ', 'a', 'n', 't', 'i', 'h', 'e', 'r', 'o', ' ', 'a', 't', ' ', 't', 'h', 'e', ' ', 'h', 'e', 'l', 'm', '.', ' ', 'A', 's', ' ', 'r', 'e', 'v', 'e', 'a', 'l', 'e', 'd', ' ', 'i', 'n', ' ', '2', '0', '0', '7', 's', ' ', 'G', 'o', 'd', ' ', 'o', 'f', ' ', 'W', 'a', 'r', ' ', '2', ' ', 'a', 'n', 'd', ' ', 't', 'h', 'e', ' ', '2', '0', '1', '0', ' ', 'P', 'l', 'a', 'y', 'S', 't', 'a', 't', 'i', 'o', 'n', ' ', 'P', 'o', 'r', 't', 'a', 'b', 'l', 'e', ' ', 'e', 'n', 't', 'r', 'y', ' ', 'G', 'o', 'd', ' ', 'o', 'f', ' ', 'W', 'a', 'r', ':', ' ', 'G', 'h', 'o', 's', 't', ' ', 'o', 'f', ' ', 'S', 'p', 'a', 'r', 't', 'a', ',', ' ', 'K', 'r', 'a', 't', 'o', 's', ' ', 'w', 'a', 's', ' ', 'b', 'o', 'r', 'n', ' ', 'i', 'n', ' ', 't', 'h', 'e', ' ', 'G', 'r', 'e', 'e', 'k', ' ', 'c', 'i', 't', 'y', '-', 's', 't', 'a', 't', 'e', ' ', 'o', 'f', ' ', 'S', 'p', 'a', 'r', 't', 'a', ',', ' ', 't', 'h', 'e', ' ', 'd', 'e', 'm', 'i', 'g', 'o', 'd', ' ', 's', 'p', 'a', 'w', 'n', ' ', 'o', 'f', ' ', 'o', 'n', 'e', ' ', 'o', 'f', ' ', 'Z', 'e', 'u', 's', ' ', 'm', 'a', 'n', 'y', ' ', 'd', 'a', 'l', 'l', 'i', 'a', 'n', 'c', 'e', 's', ' ', 'w', 'i', 't', 'h', ' ', 'm', 'o', 'r', 't', 'a', 'l', ' ', 'w', 'o', 'm', 'e', 'n', '.', ' ', 'H', 'e', ' ', 'w', 'a', 's', ' ', 'b', 'r', 'e', 'd', ' ', 'f', 'o', 'r', ' ', 'b', 'a', 't', 't', 'l', 'e', ' ', 'i', 'n', ' ', 't', 'h', 'a', 't', ' ', 'i', 'n', 'f', 'a', 'm', 'o', 'u', 's', 'l', 'y', ' ', 'b', 'e', 'l', 'l', 'i', 'c', 'o', 's', 'e', ' ', 's', 'o', 'c', 'i', 'e', 't', 'y', ' ', 'a', 'l', 'o', 'n', 'g', 's', 'i', 'd', 'e', ' ', 'h', 'i', 's', ' ', 'b', 'r', 'o', 't', 'h', 'e', 'r', ',', ' ', 'D', 'e', 'i', 'm', 'o', 's', ',', ' ', 'k', 'n', 'o', 'w', 'i', 'n', 'g', ' ', 'n', 'o', 't', 'h', 'i', 'n', 'g', ' ', 'o', 'f', ' ', 'h', 'i', 's', ' ', 'O', 'l', 'y', 'm', 'p', 'i', 'a', 'n', ' ', 'h', 'e', 'r', 'i', 't', 'a', 'g', 'e', '.', ' ', 'E', 'a', 'r', 'l', 'y', ' ', 'i', 'n', ' ', 'h', 'i', 's', ' ', 'a', 'd', 'o', 'l', 'e', 's', 'c', 'e', 'n', 'c', 'e', ',', ' ', 'Z', 'e', 'u', 's', ' ', 'l', 'e', 'a', 'r', 'n', 'e', 'd', ' ', 'o', 'f', ' ', 'a', ' ', 'p', 'r', 'o', 'p', 'h', 'e', 'c', 'y', ' ', 't', 'h', 'a', 't', ' ', 's', 't', 'a', 't', 'e', 'd', ' ', 't', 'h', 'a', 't', ' ', 'o', 'n', 'e', ' ', 'o', 'f', ' ', 'h', 'i', 's', ' ', 'c', 'h', 'i', 'l', 'd', 'r', 'e', 'n', ' ', 'w', 'o', 'u', 'l', 'd', ' ', 'e', 'v', 'e', 'n', 't', 'u', 'a', 'l', 'l', 'y', ' ', 's', 'l', 'a', 'y', ' ', 'h', 'i', 'm', ' ', '\xe2', '\x80', '\x94', ' ', 'j', 'u', 's', 't', ' ', 'a', 's', ' ', 'h', 'e', ' ', 'h', 'i', 'm', 's', 'e', 'l', 'f', ' ', 'h', 'a', 'd', ' ', 'm', 'u', 'r', 'd', 'e', 'r', 'e', 'd', ' ', 'h', 'i', 's', ' ', 'f', 'a', 't', 'h', 'e', 'r', ',', ' ', 't', 'h', 'e', ' ', 'T', 'i', 't', 'a', 'n', ' ', 'C', 'r', 'o', 'n', 'o', 's', ',', ' ', 't', 'o', ' ', 'u', 's', 'h', 'e', 'r', ' ', 'i', 'n', ' ', 't', 'h', 'e', ' ', 'e', 'r', 'a', ' ', 'o', 'f', ' ', 't', 'h', 'e', ' ', 'g', 'o', 'd', 's', '.', ' ', 'Z', 'e', 'u', 's', ' ', 's', 'e', 'n', 't', ' ', 't', 'w', 'o', ' ', 'o', 'f', ' ', 'h', 'i', 's', ' ', 'c', 'h', 'i', 'l', 'd', 'r', 'e', 'n', ',', ' ', 'A', 't', 'h', 'e', 'n', 'a', ' ', 'a', 'n', 'd', ' ', 'A', 'r', 'e', 's', ',', ' ', 't', 'o', ' ', 'f', 'i', 'n', 'd', ' ', 't', 'h', 'e', ' ', 'f', 'a', 't', 'e', 'd', ' ', 'o', 'f', 'f', 's', 'p', 'r', 'i', 'n', 'g', ' ', 'a', 'n', 'd', ' ', 'k', 'i', 'l', 'l', ' ', 't', 'h', 'e', 'm', '.', ' ', 'E', 'v', 'e', 'n', 't', 'u', 'a', 'l', 'l', 'y', ',', ' ', 't', 'h', 'e', ' ', 't', 'w', 'o', ' ', 's', 'i', 'b', 'l', 'i', 'n', 'g', 's', ' ', 'd', 'e', 't', 'e', 'r', 'm', 'i', 'n', 'e', 'd', ' ', 't', 'h', 'a', 't', ' ', 'D', 'e', 'i', 'm', 'o', 's', ' ', 'w', 'a', 's', ' ', 't', 'h', 'e', ' ', 'l', 'i', 'k', 'e', 'l', 'y', ' ', 'c', 'a', 'n', 'd', 'i', 'd', 'a', 't', 'e', ' ', 'd', 'u', 'e', ' ', 't', 'o', ' ', 's', 't', 'r', 'a', 'n', 'g', 'e', ' ', 'm', 'a', 'r', 'k', 'i', 'n', 'g', 's', ' ', 'o', 'n', ' ', 'h', 'i', 's', ' ', 'b', 'o', 'd', 'y', ',', ' ', 'a', 'n', 'd', ' ', 't', 'h', 'e', 'y', ' ', 's', 't', 'o', 'l', 'e', ' ', 'h', 'i', 'm', ' ', 'a', 'w', 'a', 'y', ' ', 'f', 'r', 'o', 'm', ' ', 'K', 'r', 'a', 't', 'o', 's', ' ', 'a', 'n', 'd', ' ', 'd', 'e', 'l', 'i', 'v', 'e', 'r', 'e', 'd', ' ', 'h', 'i', 'm', ' ', 't', 'o', ' ', 'T', 'h', 'a', 'n', 'a', 't', 'o', 's', ',', ' ', 't', 'h', 'e', ' ', 'g', 'o', 'd', ' ', 'o', 'f', ' ', 'd', 'e', 'a', 't', 'h', ',', ' ', 'g', 'i', 'v', 'i', 'n', 'g', ' ', 'K', 'r', 'a', 't', 'o', 's', ' ', 'a', ' ', 'd', 'i', 's', 't', 'i', 'n', 'c', 't', 'i', 'v', 'e', ' ', 's', 'c', 'a', 'r', ' ', 'o', 'v', 'e', 'r', ' ', 'h', 'i', 's', ' ', 'e', 'y', 'e', ' ', 'w', 'h', 'e', 'n', ' ', 'h', 'e', ' ', 't', 'r', 'i', 'e', 'd', ' ', 't', 'o', ' ', 's', 't', 'o', 'p', ' ', 't', 'h', 'e', 'm', '.', 'A', 's', ' ', 'h', 'e', ' ', 'g', 'r', 'e', 'w', ' ', 'o', 'l', 'd', 'e', 'r', ',', ' ', 'K', 'r', 'a', 't', 'o', 's', ' ', 'm', 'a', 'r', 'r', 'i', 'e', 'd', ' ', 'a', 'n', 'd', ' ', 'f', 'a', 't', 'h', 'e', 'r', 'e', 'd', ' ', 'a', ' ', 'c', 'h', 'i', 'l', 'd', ',', ' ', 'a', 's', ' ', 'r', 'e', 'c', 'o', 'u', 'n', 't', 'e', 'd', ' ', 'i', 'n', ' ', 't', 'h', 'e', ' ', 'b', 'a', 'c', 'k', 's', 't', 'o', 'r', 'y', ' ', 't', 'o', ' ', 't', 'h', 'e', ' ', 'o', 'r', 'i', 'g', 'i', 'n', 'a', 'l', ' ', 'G', 'o', 'd', ' ', 'o', 'f', ' ', 'W', 'a', 'r', '.', ' ', 'H', 'i', 's', ' ', 'a', 'p', 'p', 'e', 't', 'i', 't', 'e', ' ', 'f', 'o', 'r', ' ', 'g', 'l', 'o', 'r', 'y', ' ', 'a', 'n', 'd', ' ', 'm', 'e', 'r', 'c', 'i', 'l', 'e', 's', 's', ' ', 't', 'a', 'c', 't', 'i', 'c', 's', ' ', 'i', 'n', ' ', 'b', 'a', 't', 't', 'l', 'e', ' ', 'w', 'o', 'n', ' ', 'h', 'i', 'm', ' ', 'a', 'c', 'c', 'l', 'a', 'i', 'm', ' ', 'f', 'r', 'o', 'm', ' ', 'h', 'i', 's', ' ', 's', 'u', 'p', 'e', 'r', 'i', 'o', 'r', 's', ' ', 'a', 'n', 'd', ' ', 'f', 'e', 'a', 'r', ' ', 'f', 'r', 'o', 'm', ' ', 'h', 'i', 's', ' ', 'l', 'e', 'g', 'i', 'o', 'n', 's', ' ', 'o', 'f', ' ', 'f', 'o', 'e', 's', ',', ' ', 'a', 'n', 'd', ' ', 'o', 'v', 'e', 'r', ' ', 't', 'i', 'm', 'e', ' ', 'h', 'e', ' ', 'g', 'a', 't', 'h', 'e', 'r', 'e', 'd', ' ', 'h', 'i', 's', ' ', 'o', 'w', 'n', ' ', 'a', 'r', 'm', 'y', ' ', 'u', 'n', 'd', 'e', 'r', ' ', 't', 'h', 'e', ' ', 'b', 'a', 'n', 'n', 'e', 'r', ' ', 'o', 'f', ' ', 'S', 'p', 'a', 'r', 't', 'a', '.', ' ', 'H', 'o', 'w', 'e', 'v', 'e', 'r', ',', ' ', 'e', 'v', 'e', 'n', ' ', 'g', 'r', 'e', 'a', 't', ' ', 'a', 'r', 'm', 'i', 'e', 's', ' ', 'l', 'o', 's', 'e', ' ', 'b', 'a', 't', 't', 'l', 'e', 's', ',', ' ', 'a', 'n', 'd', ' ', 'h', 'i', 's', ' ', 'e', 'v', 'e', 'n', 't', 'u', 'a', 'l', 'l', 'y', ' ', 'f', 'o', 'u', 'n', 'd', ' ', 'i', 't', 's', 'e', 'l', 'f', ' ', 'o', 'v', 'e', 'r', 'p', 'o', 'w', 'e', 'r', 'e', 'd', ' ', 'b', 'y', ' ', 't', 'h', 'e', ' ', 'B', 'a', 'r', 'b', 'a', 'r', 'i', 'a', 'n', 's', ' ', 'f', 'r', 'o', 'm', ' ', 't', 'h', 'e', ' ', 'n', 'e', 'b', 'u', 'l', 'o', 'u', 's', ' ', 'E', 'a', 's', 't', '.', ' ', 'W', 'i', 't', 'h', ' ', 'd', 'e', 'f', 'e', 'a', 't', ' ', 'i', 'n', ' ', 's', 'i', 'g', 'h', 't', ',', ' ', 'K', 'r', 'a', 't', 'o', 's', ' ', 's', 'w', 'o', 'r', 'e', ' ', 'f', 'e', 'a', 'l', 't', 'y', ' ', 't', 'o', ' ', 'A', 'r', 'e', 's', ',', ' ', 't', 'h', 'e', ' ', 's', 'a', 'm', 'e', ' ', 'g', 'o', 'd', ' ', 'o', 'f', ' ', 'w', 'a', 'r', ' ', 't', 'h', 'a', 't', ' ', 'h', 'a', 'd', ' ', 't', 'a', 'k', 'e', 'n', ' ', 'h', 'i', 's', ' ', 'b', 'r', 'o', 't', 'h', 'e', 'r', ',', ' ', 'i', 'n', ' ', 'o', 'r', 'd', 'e', 'r', ' ', 't', 'o', ' ', 't', 'u', 'r', 'n', ' ', 't', 'h', 'e', ' ', 't', 'i', 'd', 'e', ' ', 'o', 'f', ' ', 't', 'h', 'e', ' ', 'b', 'a', 't', 't', 'l', 'e', '.', ' ', 'I', 'm', 'p', 'r', 'e', 's', 's', 'e', 'd', ' ', 'b', 'y', ' ', 'h', 'i', 's', ' ', 'h', 'a', 'l', 'f', ' ', 'b', 'r', 'o', 't', 'h', 'e', 'r', 's', ' ', 'r', 'u', 't', 'h', 'l', 'e', 's', 's', 'n', 'e', 's', 's', ',', ' ', 'A', 'r', 'e', 's', ' ', 'a', 'l', 'l', 'o', 'w', 'e', 'd', ' ', 'h', 'i', 'm', ' ', 'i', 'n', 't', 'o', ' ', 'h', 'i', 's', ' ', 's', 'e', 'r', 'v', 'i', 'c', 'e', ',', ' ', 'a', 'n', 'd', ' ', 'b', 'e', 's', 't', 'o', 'w', 'e', 'd', ' ', 'u', 'p', 'o', 'n', ' ', 'K', 'r', 'a', 't', 'o', 's', ' ', 't', 'h', 'e', ' ', 'B', 'l', 'a', 'd', 'e', 's', ' ', 'o', 'f', ' ', 'C', 'h', 'a', 'o', 's', ',', ' ', 'a', ' ', 's', 'e', 't', ' ', 'o', 'f', ' ', 's', 'w', 'o', 'r', 'd', 's', ' ', 'f', 'o', 'r', 'g', 'e', 'd', ' ', 'i', 'n', ' ', 't', 'h', 'e', ' ', 'u', 'n', 'd', 'e', 'r', 'w', 'o', 'r', 'l', 'd', ' ', 't', 'h', 'a', 't', ' ', 'b', 'e', 'c', 'a', 'm', 'e', ' ', 'l', 'i', 't', 'e', 'r', 'a', 'l', 'l', 'y', ' ', 'c', 'h', 'a', 'i', 'n', 'e', 'd', ' ', 't', 'o', ' ', 'h', 'i', 's', ' ', 'f', 'o', 'r', 'e', 'a', 'r', 'm', 's', ' ', 'a', 's', ' ', 'a', ' ', 'r', 'e', 'm', 'i', 'n', 'd', 'e', 'r', ' ', 'o', 'f', ' ', 'h', 'i', 's', ' ', 'o', 'a', 't', 'h', '.', ' ', 'W', 'i', 't', 'h', ' ', 't', 'h', 'i', 's', ' ', 'n', 'e', 'w', 'f', 'o', 'u', 'n', 'd', ' ', 'p', 'o', 'w', 'e', 'r', ',', ' ', 'K', 'r', 'a', 't', 'o', 's', ' ', 'd', 'e', 'f', 'e', 'a', 't', 'e', 'd', ' ', 't', 'h', 'e', ' ', 'e', 'n', 't', 'i', 'r', 'e', ' ', 'o', 'p', 'p', 'o', 's', 'i', 'n', 'g', ' ', 'f', 'o', 'r', 'c', 'e', ' ', 'b', 'y', ' ', 'h', 'i', 'm', 's', 'e', 'l', 'f', ',', ' ', 'r', 'e', 'v', 'e', 'l', 'i', 'n', 'g', ' ', 'i', 'n', ' ', 't', 'h', 'e', ' ', 'b', 'l', 'o', 'o', 'd', 's', 'h', 'e', 'd', '.', 'A', 's', ' ', 't', 'h', 'e', ' ', 'g', 'o', 'd', ' ', 'o', 'f', ' ', 'w', 'a', 'r', 's', ' ', 'l', 'o', 'y', 'a', 'l', ' ', 's', 'u', 'b', 'j', 'e', 'c', 't', ',', ' ', 'K', 'r', 'a', 't', 'o', 's', ' ', 'r', 'o', 'a', 'm', 'e', 'd', ' ', 'a', 'r', 'o', 'u', 'n', 'd', ' ', 't', 'h', 'e', ' ', 'c', 'o', 'u', 'n', 't', 'r', 'y', 's', 'i', 'd', 'e', ',', ' ', 'k', 'i', 'l', 'l', 'i', 'n', 'g', ' ', 's', 'o', 'l', 'd', 'i', 'e', 'r', 's', ' ', 'a', 'n', 'd', ' ', 'i', 'n', 'n', 'o', 'c', 'e', 'n', 't', 's', ' ', 'i', 'n', 'd', 'i', 's', 'c', 'r', 'i', 'm', 'i', 'n', 'a', 't', 'e', 'l', 'y', '.', ' ', 'S', 't', 'i', 'l', 'l', ',', ' ', 'h', 'i', 's', ' ', 'w', 'i', 'f', 'e', ' ', 'a', 'n', 'd', ' ', 'c', 'h', 'i', 'l', 'd', ' ', 't', 'i', 'e', 'd', ' ', 'h', 'i', 'm', ' ', 't', 'o', ' ', 't', 'h', 'e', ' ', 'r', 'e', 'a', 'l', 'm', ' ', 'o', 'f', ' ', 'e', 'v', 'e', 'r', 'y', 'd', 'a', 'y', ' ', 'p', 'e', 'o', 'p', 'l', 'e', ',', ' ', 'a', 'n', 'd', ' ', 'A', 'r', 'e', 's', ' ', 's', 'a', 'w', ' ', 'f', 'i', 't', ' ', 't', 'o', ' ', 'h', 'a', 't', 'c', 'h', ' ', 'a', ' ', 'p', 'l', 'a', 'n', ' ', 't', 'o', ' ', 's', 'e', 'v', 'e', 'r', ' ', 't', 'h', 'a', 't', ' ', 't', 'h', 'r', 'e', 'a', 'd', ',', ' ', 't', 'e', 'l', 'e', 'p', 'o', 'r', 't', 'i', 'n', 'g', ' ', 't', 'h', 'e', ' ', 'S', 'p', 'a', 'r', 't', 'a', 'n', 's', ' ', 'f', 'a', 'm', 'i', 'l', 'y', ' ', 'i', 'n', 't', 'o', ' ', 'a', ' ', 'n', 'e', 'a', 'r', 'b', 'y', ' ', 't', 'e', 'm', 'p', 'l', 'e', ' ', 'd', 'u', 'r', 'i', 'n', 'g', ' ', 'o', 'n', 'e', ' ', 'o', 'f', ' ', 'h', 'i', 's', ' ', 'r', 'a', 'm', 'p', 'a', 'g', 'e', 's', ' ', 't', 'h', 'r', 'o', 'u', 'g', 'h', ' ', 'a', ' ', 'v', 'i', 'l', 'l', 'a', 'g', 'e', '.', ' ', 'T', 'o', 'o', ' ', 'd', 'r', 'u', 'n', 'k', ' ', 'o', 'n', ' ', 'h', 'i', 's', ' ', 'r', 'a', 'g', 'e', ' ', 't', 'o', ' ', 'r', 'e', 'c', 'o', 'g', 'n', 'i', 'z', 'e', ' ', 'h', 'i', 's', ' ', 'o', 'w', 'n', ' ', 'l', 'o', 'v', 'e', 'd', ' ', 'o', 'n', 'e', 's', ',', ' ', 'K', 'r', 'a', 't', 'o', 's', ' ', 'e', 'n', 't', 'e', 'r', 'e', 'd', ' ', 't', 'h', 'e', ' ', 't', 'e', 'm', 'p', 'l', 'e', ' ', 'a', 'n', 'd', ' ', 's', 'l', 'a', 'u', 'g', 'h', 't', 'e', 'r', 'e', 'd', ' ', 'e', 'v', 'e', 'r', 'y', 'o', 'n', 'e', ' ', 'i', 'n', ' ', 's', 'i', 'g', 'h', 't', '.', ' ', 'O', 'n', 'c', 'e', ' ', 'h', 'e', ' ', 'r', 'e', 'a', 'l', 'i', 'z', 'e', 'd', ' ', 'h', 'i', 's', ' ', 't', 'e', 'r', 'r', 'i', 'b', 'l', 'e', ' ', 'm', 'i', 's', 't', 'a', 'k', 'e', ',', ' ', 'K', 'r', 'a', 't', 'o', 's', ' ', 'b', 'r', 'o', 'k', 'e', ' ', 'h', 'i', 's', ' ', 'o', 'a', 't', 'h', ' ', 't', 'o', ' ', 'A', 'r', 'e', 's', ' ', 'a', 'n', 'd', ' ', 's', 'w', 'o', 'r', 'e', ' ', 'r', 'e', 'v', 'e', 'n', 'g', 'e', '.', ' ', 'T', 'h', 'e', ' ', 'v', 'i', 'l', 'l', 'a', 'g', 'e', ' ', 'o', 'r', 'a', 'c', 'l', 'e', ',', ' ', 'w', 'h', 'o', ' ', 't', 'r', 'i', 'e', 'd', ' ', 't', 'o', ' ', 'w', 'a', 'r', 'n', ' ', 'h', 'i', 'm', ',', ' ', 'c', 'u', 'r', 's', 'e', 'd', ' ', 'h', 'i', 'm', ' ', 't', 'o', ' ', 'b', 'e', 'a', 'r', ' ', 't', 'h', 'e', ' ', 'a', 's', 'h', 'e', 's', ' ', 'o', 'f', ' ', 't', 'h', 'e', ' ', 'f', 'a', 'm', 'i', 'l', 'y', ' ', 't', 'h', 'a', 't', ' ', 'h', 'e', ' ', 'm', 'u', 'r', 'd', 'e', 'r', 'e', 'd', ' ', 'o', 'n', ' ', 'h', 'i', 's', ' ', 's', 'k', 'i', 'n', ' ', 'f', 'o', 'r', 'e', 'v', 'e', 'r', 'y'
};

//Funcao para iniciar a configuracao para a comunicacao
//Entrada: um long -> a frequencia de baud rate
//Saida: nada

void UART_iniciar(){
    ANSELCbits.ANSC7 = 0;   //Desativa função analógica no pino RC7
    ANSELCbits.ANSC6 = 0;   //Desativa função analógica no pino RC6
    RC6PPS = 0x10;          //Configura pino RC6 como TX
    RXPPSbits.RXPPS = 0x17; //Configura pino RC7 como RX
    TX1STAbits.BRGH = 0;    //Necessário para configurar o baud rate da EUSART
    BAUDCON1bits.BRG16 = 0; //Configura registrado BAUDCON1 como 8 bits
    SPBRG = 51;             //Configura baud rate para 9600bps
    RC1STAbits.SPEN = 1;    //Habilita EUSART
    RC1STAbits.CREN = 1;    //Habilita recebimento EUSART (RX)
    TX1STAbits.TXEN = 1;    //Habilita transmissão EUSART (TX)
    TX1STAbits.SYNC = 0;    //Configura EUSART para modo assíncrono
}

void UART_Escrever(char c){
    while(!TX1STAbits.TRMT);
    TXREG = c;
}

void UART_Escrever_Texto(char* texto){
    int index;
    for(index=0; texto[index]!='\0'; index++)
        UART_Escrever(texto[index]);
}


int salto_pela_formula(int key_of_rail){
    int salto = (2 * key_of_rail) - 2;
    return salto;

}

int novo_salto(int linha, int chave_rail, int tipo){
    int salto = salto_pela_formula(chave_rail);
    if(linha == 0 || linha == chave_rail-1){
        return salto;
    }
    else{
        if(tipo == 1){
            salto = salto - (2* linha);
        }else if(tipo == 2){
            salto = 2 * linha;
        }
        return salto;
    }
}

int cifra_de_rail_tigre() {

    int key = 10;
    int tamanhoMsg = 3000;
    int linha = 0;

    int primeiro_salto = novo_salto(linha,key, 1);
    int segundo_salto = novo_salto(linha,key, 2);
    //int index = 0;
    int p_salto = 0;
    int s_salto = 0;

    //printf("\nInicio - %d\n", tamanhoMsg);
    for(linha=0; linha< key; linha++){

        primeiro_salto = novo_salto(linha,key, 1);
        segundo_salto = novo_salto(linha,key, 2);
        p_salto = primeiro_salto + linha;
        s_salto = p_salto + segundo_salto;

        //mensagem->tigre[index] = mensagem->texto[linha];
        //index ++;
        UART_Escrever(texto[linha]);

        if(p_salto > tamanhoMsg){break;}

        while(1){
            if(p_salto > tamanhoMsg - 1){break;}
            //mensagem->tigre[index] = mensagem->texto[p_salto];
            //index++;
            UART_Escrever(texto[p_salto]);
            if(s_salto > tamanhoMsg - 1){break;}
            //mensagem->tigre[index] = mensagem->texto[s_salto];
            //index++;
            UART_Escrever(texto[s_salto]);

            p_salto = s_salto + primeiro_salto;
            s_salto = p_salto + segundo_salto;
        }
    }
    //printf("\n\n");
    //printf("%s", mensagem->tigre);

    //printf("\n\n");

}


int main(void) {
    TRISB = 0x00;
    UART_iniciar();
    //while(1){
        		
    //    UART_Escrever_Texto("mensagem original\n: ");
    //    UART_Escrever_Texto(texto);
    //    UART_Escrever_Texto("\r\r\r");
    //    UART_Escrever_Texto("Mensagem Cifrada: \n");
    LATBbits.LATB0 = 1;
    cifra_de_rail_tigre();
    LATBbits.LATB0 = 0;
    __delay_ms(50);
    //    UART_Escrever_Texto("\nFim da mensagem\n\n");
    //}

}