#include "kermit.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char crc(pacote *p);
int valida_crc(pacote *p);
void hexParaBin(char* str, int bits);


pacote* cria_pacote(int tipo, int sequencia, char* data, FILE* log)
{
    pacote *p;
    p = (pacote *)malloc(sizeof(pacote));
    if(!p)
    {
        mensagem_log(log, ERRO, "Erro ao alocar memória do pacote.");
        return NULL;
    }

    unsigned int data_size;
    data_size =  strlen(data);

    if(data_size > TAMANHO_DATA)
    {
        mensagem_log(log, ERRO, "Tamanho máximo de data é %d, %d recebido.", TAMANHO_DATA, data_size);
        return NULL;
    }

    p->marcador  = MARCADOR_INICIO;
    p->tipo      = tipo;
    p->sequencia = sequencia;
    p->tamanho   = data_size;
    strcpy(p->data, data);
    p->crc       = crc(p);

    return p;
}

int inicializa_ambiente(ambiente *amb, int socket, FILE *log)
{
    if(!amb) return ERRO;

    amb->transmitindo       = 0;
    amb->sequencia.enviado  = 0;
    amb->sequencia.recebido = 0;
    amb->socket             = socket;
    amb->log                = log;
    amb->ult_crc_env        = 0;
    amb->ult_crc_receb      = 0;

    return SUCESSO;
}

char crc(pacote *p)
{

    char data[4 + p->tamanho];

    data[0] = p->marcador; data[1] = p->tipo; data[2] = p->sequencia; data[3] = p->tamanho;
    for(int i = 0; i < p->tamanho; ++i)
        data[i + 4] = p->data[i];


    const char gen = 0xFF;
    char c = 0;
    for(int i=0; i < 4 + p->tamanho; ++i){
        c = c ^ data[i];
        for(int j=0; j<8; ++j){
        if((c & 0x80)!=0)
            c = (char) ((c << 1) ^ gen);
        else
            c = c << 1 ;
        }
    }
    return c;
}

int valida_crc(pacote *p)
{
    return p->crc == crc(p);
}

int valida_pacote(pacote *p)
{
    if(!p) return ERRO;
    return p->marcador == MARCADOR_INICIO;
}

void hexParaBin(char* str, int bits)
{
 
    int i = 0;

    char bin[bits + 1];
    bin[0] = '\0';
 
    while (str[i]) {
 
        switch (str[i]) {
            case '0':
                strcat(bin,"0000");
                break;
            case '1':
                strcat(bin,"0001");
                break;
            case '2':
                strcat(bin,"0010");
                break;
            case '3':
                strcat(bin,"0011");
                break;
            case '4':
                strcat(bin,"0100");
                break;
            case '5':
                strcat(bin,"0101");
                break;
            case '6':
                strcat(bin,"0110");
                break;
            case '7':
                strcat(bin,"0111");
                break;
            case '8':
                strcat(bin,"1000");
                break;
            case '9':
                strcat(bin,"1001");
                break;
            case 'A':
            case 'a':
                strcat(bin,"1010");
                break;
            case 'B':
            case 'b':
                strcat(bin,"1011");
                break;
            case 'C':
            case 'c':
                strcat(bin,"1100");
                break;
            case 'D':
            case 'd':
                strcat(bin,"1101");
                break;
            case 'E':
            case 'e':
                strcat(bin,"1110");
                break;
            case 'F':
            case 'f':
                strcat(bin,"1111");
                break;
            default:
                return;
            }
        ++i;
    }

    if(bits - (4*i))
        printf("%0*i", bits - (4*i), 0);
    printf("%s", bin);
}

void imprime_pacote(pacote *p, int bin)
{

    printf("%*s|%*s|%*s|%*s|%*s|%*s\n", TAMANHO_MARCADOR,"MARCADOR", 
                                        TAMANHO_TIPO,"TIPO", 
                                        TAMANHO_SEQUENCIA,"SEQ", 
                                        TAMANHO_TAMANHO,"TAM",
                                        10*p->tamanho, "DATA",
                                        TAMANHO_CRC, "CRC"
                                        );

    printf("%*x|%*x|%*x|%*x|", TAMANHO_MARCADOR,p->marcador, 
                               TAMANHO_TIPO,p->tipo, 
                               TAMANHO_SEQUENCIA,p->sequencia, 
                               TAMANHO_TAMANHO,p->tamanho);

    for(int i = 0; i < p->tamanho; ++i)
            printf("(%*x)", 8,*(p->data + i));

    printf("|%*x\n", TAMANHO_CRC,p->crc);

    if(bin)
    {
        char buff[10];

        sprintf(buff, "%x", p->marcador);
        hexParaBin(buff, TAMANHO_MARCADOR);
        printf("|");

        sprintf(buff, "%x", p->tipo);
        hexParaBin(buff, TAMANHO_TIPO);
        printf("|");

        sprintf(buff, "%x", p->sequencia);
        hexParaBin(buff, TAMANHO_SEQUENCIA);
        printf("|");

        sprintf(buff, "%x", p->tamanho);
        hexParaBin(buff, TAMANHO_TAMANHO);
        printf("|");

        for(int i = 0; i < p->tamanho; ++i)
        {
            sprintf(buff, "%x", *(p->data + i));
            printf("(");
            hexParaBin(buff, 8);
            printf(")");
        }
        printf("|");

        sprintf(buff, "%x", p->crc);
        hexParaBin(buff, TAMANHO_CRC);

        printf("\n");
    }
}
