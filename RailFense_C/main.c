#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct rfMensagem{
    char texto[3000];
    char cifrado[3000];
    char decifrado[3000];
}Mensagem;

void encryptMsg( Mensagem* mensagem, int chave){
    int size_msg = strlen(mensagem->texto);
    int line_matrix;
    int col_matrix;
    int inversor = -1;
    int row = 0;
    int col = 0;
    int u = 0;
    char railMatrix[chave][size_msg];
    FILE *texto_cifra_rail_fense;

    for(line_matrix = 0; line_matrix < chave; ++line_matrix)
        for(col_matrix = 0; col_matrix < size_msg; ++col_matrix)
            railMatrix[line_matrix][col_matrix] = '\n';

    for(line_matrix = 0; line_matrix < size_msg; ++line_matrix){
        railMatrix[row][col++] = mensagem->texto[line_matrix];

        if(row == 0 || row == chave-1)
            inversor= inversor * (-1);

        row = row + inversor;
    }

//    printf("\n\nMensagem Cifrada: \n\n");

    for(line_matrix = 0; line_matrix < chave; ++line_matrix)
        for(col_matrix = 0; col_matrix < size_msg; ++col_matrix)
            if(railMatrix[line_matrix][col_matrix] != '\n'){
                mensagem->cifrado[u++] = railMatrix[line_matrix][col_matrix];
            }
//    printf("\n%s\n", mensagem->cifrado);


    texto_cifra_rail_fense = fopen("texto_cifra_rail_fense.txt", "wb");
    int result = fputs(mensagem->cifrado, texto_cifra_rail_fense);
    if(result == EOF){
//        printf("Deu ruim");
        return 1;
    }

    fclose(texto_cifra_rail_fense);

}


void addMensagem(char msg1[], Mensagem* mesNew){
    int line_matrix;
    int len_msg = strlen(msg1);
    for(line_matrix = 0; line_matrix< len_msg; line_matrix++){
        mesNew->texto[line_matrix] = msg1[line_matrix];
    }
}

int main(){
    int i,ch;
    FILE *texto_claro;
    char msgTxt[3000] = {'\0'};
    texto_claro = fopen("texto_claro.txt", "rb");

    if(texto_claro == NULL){
        printf("Deu ruim");
        return 1;
    }
    i = 0;
    while (i < sizeof(msgTxt)-1 && EOF !=(ch=fgetc(texto_claro))){
        msgTxt[i++] = ch;
    }
    fclose(texto_claro);

    Mensagem * msg = (Mensagem *)malloc(sizeof(Mensagem));

    int chave = 10;

    addMensagem(msgTxt, msg);
    printf("\n\n Mensagem Original: \n\n");
    printf("\n%s\n", msg->texto);

    encryptMsg(msg, chave);
    decryptMsg(msg, chave);

    return 0;
}
