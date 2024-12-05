#include "sockets_utils.h"

// Função para criar o socket do servidor (suporta IPv4 e IPv6)
int create_server_socket(const char *address_family, int port) {
    int sockfd;
    struct sockaddr_storage server_addr; // Pode armazenar tanto IPv4 quanto IPv6

    // Escolhe o tipo de endereço dependendo do tipo informado (IPv4 ou IPv6)
    if (strcmp(address_family, "v4") == 0) {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)&server_addr;
        ipv4->sin_family = AF_INET;
        ipv4->sin_addr.s_addr = INADDR_ANY;
        ipv4->sin_port = htons(port);
    } else if (strcmp(address_family, "v6") == 0) {
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)&server_addr;
        ipv6->sin6_family = AF_INET6;
        ipv6->sin6_addr = in6addr_any;
        ipv6->sin6_port = htons(port);
    } else {
        fprintf(stderr, "Tipo de endereço inválido! Use 'v4' ou 'v6'.\n");
        exit(EXIT_FAILURE);
    }

    // Cria o socket
    sockfd = socket(server_addr.ss_family, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Erro ao criar socket");
        exit(EXIT_FAILURE);
    }

    // Associa o socket ao endereço
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erro ao associar socket");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Coloca o socket em modo de escuta
    if (listen(sockfd, 5) < 0) {
        perror("Erro ao colocar o socket em escuta");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    return sockfd;
}

// Função para criar o socket do cliente (suporta IPv4 e IPv6)
int create_client_socket(const char *address_family, const char *ip, int port) {
    int sockfd;
    struct sockaddr_storage server_addr; // Pode armazenar tanto IPv4 quanto IPv6

    // Escolhe o tipo de endereço dependendo do tipo informado (IPv4 ou IPv6)
    if (strcmp(address_family, "v4") == 0) {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)&server_addr;
        ipv4->sin_family = AF_INET;
        ipv4->sin_port = htons(port);
        if (inet_pton(AF_INET, ip, &ipv4->sin_addr) <= 0) {
            perror("Erro no endereço IPv4");
            exit(EXIT_FAILURE);
        }
    } else if (strcmp(address_family, "v6") == 0) {
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)&server_addr;
        ipv6->sin6_family = AF_INET6;
        ipv6->sin6_port = htons(port);
        if (inet_pton(AF_INET6, ip, &ipv6->sin6_addr) <= 0) {
            perror("Erro no endereço IPv6");
            exit(EXIT_FAILURE);
        }
    } else {
        fprintf(stderr, "Tipo de endereço inválido! Use 'v4' ou 'v6'.\n");
        exit(EXIT_FAILURE);
    }

    // Cria o socket
    sockfd = socket(server_addr.ss_family, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Erro ao criar socket");
        exit(EXIT_FAILURE);
    }

    // Conecta ao servidor
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erro ao conectar");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    return sockfd;
}

// Função para enviar a estrutura "action"
ssize_t send_action(int sockfd, struct action *act) {
    ssize_t bytes_sent = send(sockfd, act, sizeof(struct action), 0);
    if (bytes_sent < 0) {
        perror("Erro ao enviar dados");
    }
    return bytes_sent;
}

// Função para receber a estrutura "action"
ssize_t receive_action(int sockfd, struct action *act) {
    ssize_t bytes_received = recv(sockfd, act, sizeof(struct action), 0);
    if (bytes_received < 0) {
        perror("Erro ao receber dados");
    }
    return bytes_received;
}

// Função para fechar o socket
void close_socket(int sockfd) {
    if (close(sockfd) < 0) {
        perror("Erro ao fechar socket");
    }
}