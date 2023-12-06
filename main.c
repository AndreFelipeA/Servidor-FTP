#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lib/socket.h"
#include "lib/kermit.h"
#include "lib/servidor.h"
#include "lib/fila.h"
#include "lib/cliente.h"

#define SERVIDOR 1
#define CLIENTE  2

int tipo_ambiente();

int main()
{
    int socket = conexao_socket("lo");

    int tipo_amb = tipo_ambiente();

    ambiente amb;
    inicializa_ambiente(&amb, socket, NULL);

    if (tipo_amb == SERVIDOR)
    {
        pacote p;
        
        espera_pacote(&amb, &p);
        espera_pacote(&amb, &p);
    }
    else
    {

        int leitura;
        printf("Digite 1 para enviar mensagem\n");
        printf("Digite 2 para enviar arquivo\n");
        scanf("%d", &leitura);

        if(leitura == 1)
        {
            pacote *p = cria_pacote(TIPO_INI_TRANS, 0, "abcde", NULL);
            enviar_pacote(&amb, p);
            // espera_pacote(&amb, p);

            fila *f = inicia_fila(&amb);
            if(!f) return ERRO;
            le_mensagem(&amb,f);
            imprime_fila(f);
            // enviar_pacote(&amb, f->head->pacote);
            
            node *no = f->head;
            while( no != NULL){
                enviar_pacote(&amb, no->pacote);
                no = no->prox;
            }
        }

        //enviar_mensagem(socket, "teste", NULL);
        //printf("%d",enviar_mensagem(socket, "ola", NULL));
    }
    return 0;
}

int tipo_ambiente()
{
    int escolha = 0;
    while(escolha != SERVIDOR && escolha != CLIENTE)
    {
        system("clear");
        printf("Digite o tipo de ambiente:\n");
        printf("1) Servidor\n");
        printf("2) Cliente\n");
        scanf("%d", &escolha);
    }
    return escolha;
}