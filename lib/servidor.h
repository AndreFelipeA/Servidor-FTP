#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <stdio.h>

#include "kermit.h"

int espera_pacote(ambiente *servidor, pacote *p);
//int espera_inicio_transmissao(ambiente *servidor, pacote *p);

#endif