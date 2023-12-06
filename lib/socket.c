#include "socket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <linux/if.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>

struct sockaddr_ll addr;
struct packet_mreq mr;
struct ifreq ir;

int conexao_socket(char *device)
{
  int socket_fd;
  struct ifreq ir;

  socket_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL)); /*cria socket*/
  if (socket_fd == -1)
  {
    perror("Error to open a socket");
    exit(1);
  }

  memset(&ir, 0, sizeof(struct ifreq)); /*dispositivo eth0*/
  memcpy(ir.ifr_name, device, strlen(device));
  if (ioctl(socket_fd, SIOCGIFINDEX, &ir) == -1)
  {
    perror("Error to assign a device");
    exit(1);
  }

  memset(&addr, 0, sizeof(struct sockaddr_ll)); /*IP do dispositivo*/
  addr.sll_family = AF_PACKET;
  addr.sll_protocol = htons(ETH_P_ALL);
  addr.sll_ifindex = ir.ifr_ifindex;
  if (bind(socket_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_ll)) == -1)
  {
    perror("Error to bind");
    exit(1);
  }

  memset(&mr, 0, sizeof(mr)); /*Modo Promiscuo*/
  mr.mr_ifindex = ir.ifr_ifindex;
  mr.mr_type = PACKET_MR_PROMISC;
  if (setsockopt(socket_fd, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr, sizeof(mr)) == -1)
  {
    perror("Error to set an option to socket");
    exit(1);
  }

  // Seta procedimentos send e recv como não bloqueantes (auxilia para timeout)
  fcntl(socket_fd, F_SETFL, O_NONBLOCK);

  return socket_fd;
}

int enviar_pacote(ambiente *amb,pacote *p)
{
    printf("Enviando pacote ... \n");
    int resp = send(amb->socket, p, sizeof(*p), 0);
    if(resp < 0)
        mensagem_log(amb->log, ERRO, "Não foi possível enviar pacote para o socket!");
    else if (resp == 0)
        mensagem_log(amb->log, ERRO, "Conexão com o socket finalizada!");

    amb->ult_crc_env = p->crc;
    
    return resp;
}

int receber_pacote(ambiente *amb, pacote *p)
{
    struct sockaddr_ll xx;
    socklen_t saddr_size = sizeof(struct sockaddr_ll);

    int resp = recvfrom(amb->socket, p, sizeof(*p), 0, (struct sockaddr *)&xx, &saddr_size);

    if(p->crc == amb->ult_crc_receb || p->crc == amb->ult_crc_env) return 0;

    amb->ult_crc_receb = p->crc;
    return resp;
}

int envia_resposta(ambiente *amb,int tipo, int sequencia,char *mensagem)
{
    pacote *p = cria_pacote(TIPO_ACK, sequencia, "Pacote recebido", amb->log);
    if(enviar_pacote(amb, p) < 1) return ERRO;

    free(p);
    
    return SUCESSO;
}

/*
  todo: Implementar timeout e verificar se funcao funciona
*/
int esperar_resposta(ambiente *amb, pacote *p)
{
  time_t init, agora;
  time(&init);

  int resp;
    while(1)
    {
        resp = receber_pacote(amb, p);
        if (resp > 0)
        {
            if(valida_pacote(p) && valida_crc(p))
            {
                if(p->tipo == TIPO_ACK)  break;
                if(p->tipo == TIPO_NACK) break;
            }
        }
        time(&agora);
        if(init + amb->timeout < agora) return ERRO;
    }

    return SUCESSO;
}

