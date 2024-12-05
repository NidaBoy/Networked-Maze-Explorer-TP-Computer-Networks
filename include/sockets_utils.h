#ifndef SOCKETS_UTILS_H
#define SOCKETS_UTILS_H

#include <netinet/in.h>  // Para as estruturas sockaddr_in e sockaddr_in6
#include <stdio.h>       // Para printf, perror
#include <stdlib.h>      // Para funções de alocação e exit
#include <string.h>      // Para manipulação de strings
#include <unistd.h>      // Para close
#include <arpa/inet.h>   // Para inet_pton e inet_ntop

// Definição da estrutura "action"
struct action {
    int type;
    int moves[100];
    int board[10][10];
};

// Funções de inicialização e comunicação
int create_server_socket(const char *address_family, int port);
int create_client_socket(const char *address_family, const char *ip, int port);

ssize_t send_action(int sockfd, struct action *act);
ssize_t receive_action(int sockfd, struct action *act);

// Funções auxiliares para fechar sockets
void close_socket(int sockfd);

#endif // SOCKETS_UTILS_H