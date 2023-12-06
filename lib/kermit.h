#ifndef __KERMIT_H__
#define __KERMIT_H__

#include "utils.h"

/*
    Codigo marcador de inicio do pacote
*/

#define MARCADOR_INICIO (0x7e)

/*
    Codigos do campo TIPO do pacote
*/

#define TIPO_TEXTO     (0x0)
#define TIPO_MIDIA     (0x1)
#define TIPO_ACK       (0x2)
#define TIPO_NACK      (0x3)
#define TIPO_ERRO      (0x4)
#define TIPO_INI_TRANS (0X5) // Inicio Transmissao
#define TIPO_FIM_TRANS (0x6) // Fim Transmissao

/*
    Codigos de erro do pacote
*/

#define ERRO_DIR (0x0) // Diretorio inexistente
#define ERRO_PER (0x1) // Sem permissao
#define ERRO_ESP (0x2) // Espaco insuficiente

/*
    Bits do pacote
*/

#define TAMANHO_MARCADOR   8
#define TAMANHO_TIPO       6
#define TAMANHO_SEQUENCIA  4
#define TAMANHO_TAMANHO    6
#define TAMANHO_DATA      64
#define TAMANHO_CRC        8

/*
    Time out
*/

#define TIMEOUT 10

/*
    Estrutura do pacote
*/

typedef char byte_t;

typedef struct pacote
{
    byte_t marcador : TAMANHO_MARCADOR;
    byte_t tipo : TAMANHO_TIPO;
    byte_t sequencia : TAMANHO_SEQUENCIA;
    byte_t tamanho : TAMANHO_TAMANHO;
    byte_t data[TAMANHO_DATA + 1];
    byte_t crc : TAMANHO_CRC;
}pacote;

pacote* cria_pacote(int tipo, int sequencia, char* data, FILE* log);
void imprime_pacote(pacote *p, int bin);
int valida_pacote(pacote *p);
int valida_crc(pacote *p);

typedef struct seq
{
    byte_t recebido : TAMANHO_SEQUENCIA;
    byte_t enviado  : TAMANHO_SEQUENCIA;
} seq;

typedef struct ambiente
{
    int transmitindo;
    int socket;
    int timeout; // em segundos
    seq sequencia;
    FILE *log;
    byte_t ult_crc_env : TAMANHO_CRC;
    byte_t ult_crc_receb : TAMANHO_CRC;
}ambiente;

int inicializa_ambiente(ambiente *amb, int socket, FILE *log);

#endif