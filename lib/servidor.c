#include "servidor.h"
#include "socket.h"


int espera_pacote(ambiente *servidor, pacote *p)
{
    int resp;
    do
    {
        resp = receber_pacote(servidor, p);
        if (resp > 0)
        {
            if(valida_pacote(p) && !valida_crc(p))
            {
                envia_resposta(servidor, TIPO_NACK, servidor->sequencia.recebido, "Pacote invalido");
            }
        }
    }
    while(resp < 1 || !valida_pacote(p) || !valida_crc(p));

    imprime_pacote(p, 1);

    return SUCESSO;
}

// int processa_pacote(pacote *p)
// {

// }



