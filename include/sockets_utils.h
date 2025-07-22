#ifndef SOCKETS_UTILS_H
#define SOCKETS_UTILS_H

#include <netinet/in.h>  // Para as estruturas sockaddr_in e sockaddr_in6
#include <stdio.h>       // Para printf, perror
#include <stdlib.h>      // Para funções de alocação e exit
#include <string.h>      // Para manipulação de strings
#include <unistd.h>      // Para close
#include <arpa/inet.h>   // Para inet_pton e inet_ntop

// Constantes a serem usadas nas trocas de mensagens para saber o tipo da ação
#define START 0
#define MOVE 1
#define MAP 2
#define HINT 3
#define UPDATE 4
#define WIN 5
#define RESET 6
#define EXIT 7

// Definição da estrutura Action para troca de mensagens
typedef struct Action {
    int type;
    int moves[100];
    int board[10][10];
} Action;

// Funções de inicialização e comunicação
int create_server_socket(const char *address_family, int port);
int create_client_socket(const char *address_family, const char *ip, int port);

ssize_t send_action(int sockfd, struct Action *act);
ssize_t receive_action(int sockfd, struct Action *act);

// Funções auxiliares para fechar sockets
void close_socket(int sockfd);

#endif // SOCKETS_UTILS_H