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


//const char var[6144];
//const int var2[998];
//const char vailogo[10][614];    
//const char array[2][3000]; - 74
const char texto[496] =  {
	'N', 'o', 'w', ',', ' ', 'a', 'f', 't', 'e', 'r', ' ', 't', 'h', 'e', ' ', 'e', 'n', 's', 'u', 'i', 'n', 'g', ' ', 'a', 'v', 'a', 'l', 'a', 'n', 'c', 'h', 'e', ' ', 'o', 'f', ' ', 's', 'e', 'q', 'u', 'e', 'l', 's', ',', ' ', 'p', 'r', 'e', 'q', 'u', 'e', 'l', 's', ' ', 'a', 'n', 'd', ' ', 's', 'i', 'd', 'e', ' ', 's', 't', 'o', 'r', 'i', 'e', 's', ',', ' ', 't', 'h', 'e', ' ', 's', 'a', 'g', 'a', ' ', 'o', 'f', ' ', 'K', 'r', 'a', 't', 'o', 's', ' ', 's', 't', 'a', 'r', '-', 'c', 'r', 'o', 's', 's', 'e', 'd', ' ', 'l', 'i', 'f', 'e', ' ', 'h', 'a', 's', ' ', 'b', 'e', 'c', 'o', 'm', 'e', ' ', 'a', 's', ' ', 't', 'a', 'n', 'g', 'l', 'e', 'd', ' ', 'a', 'n', 'd', ' ', 'c', 'o', 'm', 'p', 'l', 'e', 'x', ' ', 'a', 's', ' ', 't', 'h', 'e', ' ', 'm', 'y', 't', 'h', 'i', 'c', ' ', 't', 'a', 'l', 'e', 's', ' ', 't', 'h', 'a', 't', ' ', 'i', 'n', 's', 'p', 'i', 'r', 'e', 'd', ' ', 't', 'h', 'e', ' ', 'w', 'o', 'r', 'l', 'd', ' ', 'h', 'e', 's', ' ', 'i', 'n', 'h', 'a', 'b', 'i', 't', 'e', 'd', '.', ' ', 'A', 'n', 'd', ' ', 't', 'h', 'o', 'u', 'g', 'h', ' ', 't', 'h', 'e', ' ', 'u', 'p', 'c', 'o', 'm', 'i', 'n', 'g', ' ', 'P', 'l', 'a', 'y', 'S', 't', 'a', 't', 'i', 'o', 'n', ' ', '4', ' ', 'f', 'o', 'l', 'l', 'o', 'w', '-', 'u', 'p', ' ', 'd', 'o', 'e', 's', ' ', 'i', 't', 's', ' ', 'b', 'e', 's', 't', ' ', 't', 'o', ' ', 'l', 'e', 'a', 'v', 'e', ' ', 't', 'h', 'e', ' ', 'l', 'y', 'r', 'e', 's', ' ', 'a', 'n', 'd', ' ', 'w', 'i', 'n', 'g', 'e', 'd', ' ', 'c', 'h', 'a', 'r', 'i', 'o', 't', 's', ' ', 'o', 'f', ' ', 'f', 'a', 'n', 't', 'a', 's', 'y', ' ', 'a', 'n', 'c', 'i', 'e', 'n', 't', ' ', 'G', 'r', 'e', 'e', 'c', 'e', ' ', 'i', 'n', ' ', 't', 'h', 'e', ' ', 'r', 'e', 'a', 'r', 'v', 'i', 'e', 'w', ' ', 'm', 'i', 'r', 'r', 'o', 'r', ',', ' ', 'h', 'e', 'r', 'e', 's', ' ', 'a', ' ', 'q', 'u', 'i', 'c', 'k', ' ', 'p', 'r', 'i', 'm', 'e', 'r', ' ', 'o', 'n', ' ', 'a', 'l', 'l', ' ', 't', 'h', 'e', ' ', 'm', 'a', 'j', 'o', 'r', ' ', 'r', 'e', 'v', 'e', 'l', 'a', 't', 'i', 'o', 'n', 's', ' ', 'f', 'r', 'o', 'm', ' ', 't', 'h', 'e', ' ', 'p', 'r', 'e', 'v', 'i', 'o', 'u', 's', ' ', 'g', 'a', 'm', 'e', 's', ',', ' ', 'a', 'n', 'd', ' ', 'h', 'o', 'w', ' ', 't', 'h', 'e', 'y', ' ', 's', 'h', 'a', 'p', 'e', 'd', ' ', 't', 'h', 'e', ' ', 'a', 'n', 'g', 'r', 'y', ' ', 'a', 'n', 't', 'i', 'h', 'e', 'r', 'o', ' ', 'a', 't', ' ', 't', 'h', 'e', ' ', 'h', 'e', 'l', 'm', '.', ' ', 'G', 'a', 'b', 'r', 'i', 'e', 'l', ' ', 'P', 'a', 'l', 'h', 'a', 'r', 'e', 's'
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


// Recebe uma mensagem (Array de chars) e um inteiro chave. Realiza a cifragem do texto, e escreve o resultado em um
// arquivo. Retorna 1 se a abertura e escrita do arquivo não for feita da maneira correta.  
void encripta_mensagem() {
    
    int tamanho_msg = 496;
    int linha_matriz;
    int col_matriz;
    int inversor = -1;
    int linha = 0;
    int col = 0;
    int indice_cifrado = 0;
    int chave = 2;
    
    static char rail_matriz[2][496];

    for(linha_matriz = 0; linha_matriz < chave; ++linha_matriz)
        for(col_matriz = 0; col_matriz < tamanho_msg; ++col_matriz)
            rail_matriz[linha_matriz][col_matriz] = '\n';

    for(linha_matriz = 0; linha_matriz < tamanho_msg; ++linha_matriz){
        rail_matriz[linha][col++] = texto[linha_matriz];

        if(linha == 0 || linha == chave-1)
            inversor= inversor * (-1);

        linha = linha + inversor;
    }
     
    for(linha_matriz = 0; linha_matriz < chave; ++linha_matriz)
        for(col_matriz = 0; col_matriz < tamanho_msg; ++col_matriz)
            if(rail_matriz[linha_matriz][col_matriz] != '\n'){
                //texto[indice_cifrado++] = rail_matriz[linha_matriz][col_matriz];
                UART_Escrever(rail_matriz[linha_matriz][col_matriz]);
            }
}



int main(void) {
    UART_iniciar();
    //while(1){
        		
        UART_Escrever_Texto("mensagem original\n: ");
        UART_Escrever_Texto(texto);
        UART_Escrever_Texto("\r\r\r");
        UART_Escrever_Texto("Mensagem Cifrada: \n");
        encripta_mensagem();
      
        UART_Escrever_Texto("\nFim da mensagem\n\n");
    //}
}

