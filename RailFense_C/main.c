/*

 *  Sobre o algoritmo e seu desenvolvimento:
    Algoritmo implementado para plataforma ARM/Linux, na mat�ria de Sistemas Embarcados, com o objetivo inicial de
    ser compilado atrav�s do Cross Compiler e ser executado no ARM de uma raspberry Pi 3 (utilizando o modelo Host/
    Target).

    Esse algoritmo encripta uma mensagem  utilizando a cifra de Rail Fense com rails = 10, que utiliza a estrat�gia das cifras
    de transposi��o. Esta estrat�gia consiste em alterar a ordem dos caracteres sem alterar o tamanho da mensagem.
    Portanto tem-se um texto de entrada que � lido de um arquivo, e o texto cifrado � escrito em um arquivo de sa�da.
    Por padr�o passamos um texto de 3000 caracteres para atender os requisitos do projeto. A entrada pode ser
    modificada apenas alterando o arquivo, sem precisar compilar novamente.

 *  Autores : Gabriel Andrade Palhares e Guilherme Araujo da Silva

 *

 *  Copyright (C) 2018 Gabriel Andrade Palhares <gaapalhares@gmail.com>

 *  Copyright (C) 2018 Guilherme Araujo da Silva  <guiaszul@gmail.com> *

 *  Data de Atualiza��o : 01 de outubro de 2018

 *  Modo de uso:
        Ap�s inserir a mensagem em um arquivo "texto_claro.txt" basta gerar o compilado e executar com o arquivo na
        mesma pasta do compilado. Assim ser� gerado arquivos "texto_cifra_rail_fense.txt" com a mensagem cifrada.

 *  Restri��o de uso:
        Os valores de entrada devem ser inseridos em um arquivo .txt com o t�tulo de "texto_claro.txt" para um bom
        funcionamento.


    C�digo usado como refer�ncia: https://www.thecrazyprogrammer.com/2017/09/rail-fence-cipher-program-in-c.html
*/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>


//Nesta struct ser� armazenado as mensagens em seus 3 estados: claro, cifrado e decifrado.
typedef struct rfMensagem {

    char texto[3000];
    char cifrado[3000];
    char decifrado[3000];
    char decifrado2[3000];

} Mensagem;


// Fun��o: Realiza a cifragem do texto, e escreve o resultado em um arquivo.
// Entrada: Recebe um ponteiro para a struct Mensagem e um inteiro chave.
// Retorna: Retorna 1 se a abertura e escrita do arquivo n�o for feita da maneira correta.
//          Caso n�o tenha erros, a fun��o ir� gravar a mensagem cifrada na struct para ser passada ao arquivo.
int encripta_mensagem( Mensagem* mensagem, int chave) {

    int tamanho_msg = strlen(mensagem -> texto);
    int linha_matriz;
    int col_matriz;
    int inversor = -1;
    int linha = 0;
    int col = 0;
    int indice_cifrado = 0;
    char rail_matriz[chave][tamanho_msg];
    FILE *texto_cifra_rail_fense;

    //Constr�i a matriz e preenche suas posi��es com '\n'
    // Matriz[numero da chave passada por parametro][tamanho da mensagem]
    for(linha_matriz = 0; linha_matriz < chave; ++linha_matriz)
        for(col_matriz = 0; col_matriz < tamanho_msg; ++col_matriz)
            rail_matriz[linha_matriz][col_matriz] = '\n';

    //Percorre a matriz e distribui os caracteres em zig-zag
    for(linha_matriz = 0; linha_matriz < tamanho_msg; ++linha_matriz){
        rail_matriz[linha][col++] = mensagem->texto[linha_matriz];

        if(linha == 0 || linha == chave-1)
            inversor= inversor * (-1);

        linha = linha + inversor;
    }

    //printf("\n\nMensagem Cifrada: \n\n");
    //Percorre a nova matriz linha por linha e caso o caractere lido seja diferente de '\n' ser� armazenado
    //na string da struct(mensagem -> cifrado) onde poder� ser reutilizado.
    for(linha_matriz = 0; linha_matriz < chave; ++linha_matriz)
        for(col_matriz = 0; col_matriz < tamanho_msg; ++col_matriz)
            if(rail_matriz[linha_matriz][col_matriz] != '\n'){
                mensagem -> cifrado[indice_cifrado++] = rail_matriz[linha_matriz][col_matriz];
            }
    //Ap�s a convers�o o valor cifrado � printado no terminal para fins de visualiza��o.
    printf("\n %s \n", mensagem -> cifrado);

    //grava��o do mensagem cifrado no arquivo
    texto_cifra_rail_fense = fopen("texto_cifra_rail_fense.txt", "wb");
    int result = fputs(mensagem -> cifrado, texto_cifra_rail_fense);
    if(result == EOF){
        printf("Erro na leitura de arquivo");
        return 1;
    }

    fclose(texto_cifra_rail_fense);

}


// Entrada:  Recebe como par�metro uma mensagem (texto cifrado) e um inteiro chave.
// Faz: L� o arquivo de sa�da da fun��o encripta_mensagem().
//      Realiza a decifragem do texto para fins de valida��o e escreve o resultado em um arquivo.
// Retorna: Retorna 1 se a abertura e escrita do arquivo n�o forem feitas da maneira correta.

int decripta_mensagem(Mensagem * mensagem, int chave){
    int tamanho_msg = strlen(mensagem->cifrado);
    int linha_matriz;
    int col_matriz;
    int inversor = -1;
    int linha = 0;
    int col = 0;
    int m = 0;
    int indice_decifrado = 0;
    char rail_matriz[chave][tamanho_msg];
    FILE *texto_cifra_rail_fense;
    FILE *texto__rail_fense_decifrado;

    texto_cifra_rail_fense = fopen("texto_cifra_rail_fense.txt", "rb");

    if(texto_cifra_rail_fense == NULL){
        printf("Deu ruim");
        return 1;
    }
    int i, char_temporario;

    i = 0;
    while (i < sizeof(mensagem->cifrado)-1 && EOF !=(char_temporario=fgetc(texto_cifra_rail_fense))){
        mensagem->cifrado[i++] = char_temporario;
    }
    fclose(texto_cifra_rail_fense);


    for(linha_matriz = 0; linha_matriz < chave; ++linha_matriz){
        for(col_matriz = 0; col_matriz < tamanho_msg; ++col_matriz){
            rail_matriz[linha_matriz][col_matriz] = '\n';
        }
    }


    for(linha_matriz = 0; linha_matriz < tamanho_msg; ++linha_matriz){
        rail_matriz[linha][col++] = '*';

        if(linha == 0 || linha == chave-1){
            inversor= inversor * (-1);
        }

        linha = linha + inversor;
    }

    for(linha_matriz = 0; linha_matriz < chave; ++linha_matriz)
        for(col_matriz = 0; col_matriz < tamanho_msg; ++col_matriz)
            if(rail_matriz[linha_matriz][col_matriz] == '*'){
                rail_matriz[linha_matriz][col_matriz] = mensagem->cifrado[m++];
            }

    linha = col = 0;
    inversor = -1;

    printf("\n\nMensagem Decifrada\n\n");

    for(linha_matriz = 0; linha_matriz < tamanho_msg; ++linha_matriz){
        mensagem->decifrado[indice_decifrado++] = rail_matriz[linha][col++];
        if(linha == 0 || linha == chave-1){
            inversor = inversor * (-1);
        }

        linha = linha + inversor;
    }
    printf("\n%s\n", mensagem -> decifrado);

    texto__rail_fense_decifrado = fopen("texto__rail_fense_decifrado.txt", "wb");
    int result = fputs( mensagem->decifrado, texto__rail_fense_decifrado);
    if(result == EOF){
        printf("Deu ruim");
        return 1;
    }

    fclose(texto__rail_fense_decifrado);
}



// Fun��o: Realiza a cifragem do texto, e escreve o resultado em um arquivo.
// Entrada: Recebe um ponteiro para a struct Mensagem e um inteiro chave.
// Retorna: Retorna 1 se a abertura e escrita do arquivo n�o for feita da maneira correta.
//          Caso n�o tenha erros, a fun��o ir� gravar a mensagem cifrada na struct para ser passada ao arquivo.
int encripta_mensagem_modo_2( Mensagem* mensagem, int chave) {

    int tamanho_msg = strlen(mensagem -> texto);
    int linha_matriz;
    int col_matriz;
    int inversor = -1;
    int linha = 0;
    int col = 0;
    int indice_cifrado = 0;
    char rail_matriz[chave][tamanho_msg];
    FILE *texto_cifra_rail_fense;

    int jump = 18;
    int next = 0;
    int item_go = 0;
    int index = 0;
    int last = tamanho_msg;
    int i=1;

    for(item_go = 0 ; item_go<=chave; item_go++){
        mensagem->decifrado2[index] = mensagem->texto[item_go];
        next = item_go + jump;
        int i2;
        for(i2= i; i2 < tamanho_msg; i2++){
            mensagem->decifrado2[i2] = mensagem->texto[next];
            next = next + jump;
            if(next >= tamanho_msg+1){
                printf("MORRI!%d BOA!", i2);
                break;
            }
            i = i2;

        }
        jump = jump - 2;
        next = 0;
        if(jump == 0){jump = 18;}
    }
    mensagem->decifrado2[0] = mensagem->texto[0];



    printf("\n\n\n\n\n\n\n\n\n\nMensagem Cifrada MODO 2: \n\n");

    printf("\n %s \n", mensagem->decifrado2);


    printf("OOOOI eu sou o %c prazer", mensagem->decifrado2[0]);

    printf("\n\n\n\n\n\n\n\n\n\n");



}




// Entrada:
// Faz:
// Sa�da:
void copia_mensagem(char msg_encriptada[], Mensagem* nova_mensagem) {
    int linha_matriz;
    int len_msg = strlen(msg_encriptada);

    for(linha_matriz = 0; linha_matriz < len_msg; linha_matriz++) {
        nova_mensagem -> texto[linha_matriz] = msg_encriptada[linha_matriz];
    }
}

void inicia_texto_claro(Mensagem * msg_original) {

    int i, char_temporario;
    char msg_texto[3000] = {'\0'};
    FILE *texto_claro;
    texto_claro = fopen("texto_claro.txt", "rb");

    if(texto_claro == NULL){
        printf("Deu ruim");
        return 1;
    }

    i = 0;
    while (i < sizeof(msg_texto) - 1 && EOF !=(char_temporario=fgetc(texto_claro))) {
        msg_texto[i++] = char_temporario;
    }
    fclose(texto_claro);

    copia_mensagem(msg_texto, msg_original);
    //printf("\n\n Mensagem Original: \n\n");
    //printf("\n%s\n", msg_original -> texto);
}

int main(){
    //Inicializa struct rfMensagem
    	//variaveis de tempo
	clock_t init_count,end_count;
    double period;
    Mensagem * msg = (Mensagem *)malloc(sizeof(Mensagem));

    inicia_texto_claro(msg);
    encripta_mensagem(msg, 10);
    decripta_mensagem(msg, 10);
    encripta_mensagem_modo_2(msg, 10);
    }
    //end_count = clock();

    //qqperiod = ( (end_count - init_count) / (CLOCKS_PER_SEC/1000.0) );

    //printf("\n\n\n\n\n\nAAAAAAAAAAAAAAAAAAA tempo coletado = %f\n\n\n\n\n\n\n\n", period);
    //period = period/1000.0;
    //printf("\n\n\n\n\n\nAAAAAAAAAAAAAAAAAAA tempo coletado = %f\n\n\n\n\n\n\n\n", period);
    //decripta_mensagem(msg, 10);
