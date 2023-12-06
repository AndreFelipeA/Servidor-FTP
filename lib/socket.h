#ifndef SOCKET_H
#define SOCKET_H

#include "kermit.h"

int conexao_socket(char *device);
int envia_resposta(ambiente *amb,int tipo, int sequencia,char *mensagem);
int esperar_resposta(ambiente *amb, pacote *p);
int enviar_pacote(ambiente *amb,pacote *p);
int receber_pacote(ambiente *amb, pacote *p);

#endif