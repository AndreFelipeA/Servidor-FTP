#include "fila.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

fila *inicia_fila(ambiente *amb)
{
    fila *f = malloc(sizeof(fila));
    if(!f)
    {
        mensagem_log(amb->log, ERRO, "Erro ao alocar memória da fila.");
        return NULL;
    }
    f->head = NULL;
    f->tail = NULL;
    return f;
}


int adiciona_pacote(fila *f, pacote *p, FILE *log)
{
    node *no = malloc(sizeof(node));
    if(!no)
    {
        mensagem_log(log, ERRO, "Erro ao alocar memória do no.");
        return ERRO;
    }
    no->prox = NULL;
    no->pacote = p;

    if(f->head == NULL)
    {
        f->head = no;
        f->tail = no;
    }
    else
    {
        f->tail->prox = no;
        f->tail = no;
    }

    return SUCESSO;
}

pacote *remove_pacote(fila *f)
{
    if(f->head == NULL) return NULL;

    pacote *p = f->head->pacote;

    node *no = f->head;
    f->head = f->head->prox;

    free(no);

    return p;
}


void imprime_fila(fila *f)
{
    node *no = f->head;
    while( no != NULL){
        printf("Pacote[%d] - (%s) \n",no->pacote->sequencia,no->pacote->data);
        no = no->prox;
    }
}