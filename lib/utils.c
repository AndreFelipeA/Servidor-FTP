#include "utils.h"

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>

FILE* cria_log(char *nome_arquivo)
{
    time_t now = time(NULL) ;
    struct tm tm_now ;
    localtime_r(&now, &tm_now) ;
    char time[21];
    strftime(time, sizeof(time), "%Y_%m_%d_%H:%M:%S", &tm_now);

    char arquivo[ strlen(LOG_FOLDER) + strlen(nome_arquivo) + 26 ];
    snprintf(arquivo, sizeof(arquivo), "%s/%s_%s.log", LOG_FOLDER, nome_arquivo, time);

    return fopen(arquivo, "w");
}

void finaliza_log(FILE *log)
{
    fclose(log);
}

void mensagem_log(FILE *log, int tipo,char *mensagem, ...)
{
    if(log)
    {
        char msg_log[strlen(mensagem) + 15];
        va_list arg;
        va_start(arg, mensagem);

        if(tipo == SUCESSO)
            snprintf(msg_log,  sizeof(msg_log) ,"[SUCESSO] %s", mensagem);
        else if(tipo == AVISO)
            snprintf(msg_log,  sizeof(msg_log) ,"[AVISO] %s", mensagem);
        else if(tipo == ERRO)
            snprintf(msg_log,  sizeof(msg_log) ,"[ERRO] %s", mensagem);
        else
            snprintf(msg_log,  sizeof(msg_log) ,"[MENSAGEM] %s", mensagem);
        
        vfprintf(log, msg_log, arg);

        va_end(arg);
    }
}