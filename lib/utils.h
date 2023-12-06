#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define ERRO    0
#define SUCESSO 1
#define AVISO   2

#define LOG_FOLDER "logs"

FILE* cria_log(char *nome_arquivo);
void finaliza_log(FILE *log);
void mensagem_log(FILE *log, int tipo,char *mensagem, ...);

#endif
