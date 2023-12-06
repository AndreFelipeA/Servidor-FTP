#ifndef FILA_H
#define FILA_H

#include "kermit.h"


typedef struct node
{
    struct node *prox;
    struct pacote *pacote;
}node;

typedef struct fila
{
    struct node *head;
    struct node *tail;
}fila;

fila *inicia_fila(ambiente *amb);
int adiciona_pacote(fila *f, pacote *p, FILE *log);
pacote *remove_pacote(fila *f);
void imprime_fila(fila *f);

#endif