#include "cliente.h"
#include "socket.h"
#include "fila.h"
#include <string.h>
#include <stdlib.h>


int iniciar_transmissao(ambiente *amb, char *msg);

int enviar_mensagem(ambiente *amb , char *mensagem)
{
    char pacote_msg[7];
    sprintf(pacote_msg, "tipo:%c", TIPO_TEXTO);
    //if (!iniciar_transmissao(amb, pacote_msg)) return ERRO;

    return SUCESSO;
}

int le_mensagem(ambiente *amb, fila *f)
{
    // char buffer[TAMANHO_DATA + 1];

    pacote *p;
    
    int sequencia = 1;
    int ch;
    int scape = 0;
    // while (((ch = getchar()) != EOF) && (ch != '\n')) {} // limpa buffer

    // while(fgets(buffer, TAMANHO_DATA + 1, stdin))
    // {

    //     p = cria_pacote(TIPO_TEXTO,sequencia, buffer, amb->log);

    //     adiciona_pacote(f, p, amb->log);
    //     // printf("Pacote (%d) adicionado (%s) \n",sequencia, p->data);

    //     if(buffer[strlen(buffer) - 1] == '\n') break;

    //     if(sequencia == TAMANHO_SEQUENCIA)
    //         sequencia = 1;
    //     else
    //         sequencia += 1;

    //     while (((ch = getchar()) != EOF) && (ch != '\n')) {} // limpa buffer
    // }

    char *linha = malloc(64*sizeof(char));
    while (((ch = getchar()) != EOF) && (ch != '\n')) {} // limpa buffer
    while(1 && scape == 0){
        
        fgets(linha,64,stdin);
        if(linha[0] != 27)
        {
            linha = strtok(linha,"\n");
            p = cria_pacote(TIPO_TEXTO,sequencia, linha, amb->log);
            adiciona_pacote(f, p, amb->log);
            linha[0]= '\0';

            if(sequencia == TAMANHO_SEQUENCIA)
                sequencia = 1;
            else
                sequencia += 1;
        }

        scape = 1;
    }



    /*
    while(f->head)
    {
        imprime_pacote(remove_pacote(f), 0);
    }
    */
    

    return SUCESSO;
}

/*
int iniciar_transmissao(ambiente *amb, char *msg)
{
    pacote *init = cria_pacote(TIPO_INI_TRANS, 0, msg, amb->log);
    if(enviar_pacote(amb, init) < 1) return ERRO;

    pacote p;
    int resp;
    do
    {
        resp = receber_pacote(amb, &p);
        if (resp > 0)
        {
            if( && !valida_crc(&p))
            {
                envia_resposta(amb, TIPO_NACK, amb->sequencia.recebido, "Pacote invalido");
            }
        }
    }
    while(valida_pacote(&p) && valida_crc(&p) && p.tipo == TIPO_ACK);

    return SUCESSO;
}

*/