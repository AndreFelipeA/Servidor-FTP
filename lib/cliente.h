#ifndef CLIENTE_H
#define CLIENTE_H

#include <stdio.h>

#include "kermit.h"
#include "fila.h"

void inicializa_cliente();
int enviar_mensagem(ambiente *amb , char *mensagem);
int le_mensagem(ambiente *amb, fila *f);

#endif