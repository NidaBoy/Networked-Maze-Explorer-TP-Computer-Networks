#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "maze_logics.h"
#include "sockets_utils.h"
#include "server.h"

// Funções auxiliares
void handle_client(int client_socket, Maze *maze, int rows, int cols);
void load_maze_from_file(const char *filename, Maze *maze, int *rows, int *cols);

// Função principal
int main(int argc, char *argv[]) {
    if (argc != 5 || strcmp(argv[3], "-i") != 0) {
        fprintf(stderr, "Uso: %s <v4|v6> <porta> -i <arquivo_labirinto>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[2]);
    const char *input_file = argv[4];

    // Carrega o labirinto do arquivo
    Maze maze;
    int rows, cols;
    load_maze_from_file(input_file, &maze, &rows, &cols);

    int server_socket = create_server_socket(argv[1], port);

    // Aceita a conexão do cliente
    int client_socket = accept(server_socket, NULL, NULL);

    if (client_socket == -1) {
        perror("Erro ao aceitar conexão");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("client connected\n");

    // Trata a comunicação com o cliente
    handle_client(client_socket, &maze, rows, cols);

    // Fecha os sockets
    close(client_socket);
    close(server_socket);

    return 0;
}

// Função para tratar a comunicação com o cliente
void handle_client(int client_socket, Maze *maze, int rows, int cols) {
    Action action;
    Numerical_Maze numerical_maze;
    Textual_Maze textual_maze;

    while (1) {
        // Recebe a ação do cliente
        if (recv(client_socket, &action, sizeof(action), 0) <= 0) {
            printf("Cliente desconectado.\n");
            break;
        }

        switch (action.type) {
            case START: { // Início do jogo
                printf("starting new game\n");
                avaliate_valid_moves(maze, action.moves);

                // Envia o estado inicial do jogo
                action.type = UPDATE;
                send(client_socket, &action, sizeof(action), 0);
                break;
            }

            case MOVE: { // Movimento no labirinto
                printf("Jogador tenta se mover.\n");
                if (move_player(maze, action.moves[0], action.moves)) {

                    // Verifica vitória
                    if (check_win_condition(maze)) {
                        action.type = WIN;
                        send(client_socket, &action, sizeof(action), 0);
                        printf("Jogador venceu o jogo!\n");
                        break;
                    }

                    // Envia atualização do estado
                    action.type = UPDATE;
                    memcpy(action.board, numerical_maze.grid, sizeof(numerical_maze.grid));
                    send(client_socket, &action, sizeof(action), 0);
                } else {
                    printf("Movimento inválido.\n");
                }
                break;
            }

            case RESET: { // Reinício do jogo
                printf("Reiniciando o jogo.\n");
                initialize_maze(maze, rows, cols, maze->grid);
                avaliate_valid_moves(maze, action.moves);

                // Envia o novo estado inicial
                action.type = UPDATE;
                represent_maze_vision(maze, &numerical_maze);
                memcpy(action.board, numerical_maze.grid, sizeof(numerical_maze.grid));
                send(client_socket, &action, sizeof(action), 0);
                break;
            }

            case EXIT: { // Desconexão
                printf("client disconnected\n");
                close(client_socket);
                return;
            }
            case MAP: {
                represent_maze_vision(maze, &numerical_maze);

                action.type = MAP;
                memcpy(action.board, numerical_maze.grid, sizeof(numerical_maze.grid));
                send(client_socket, &action, sizeof(action), 0);
            }
            default:
                printf("Ação desconhecida recebida.\n");
                break;
        }
    }
}

// Função para carregar o labirinto de um arquivo
void load_maze_from_file(const char *filename, Maze *maze, int *rows, int *cols) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo do labirinto");
        exit(EXIT_FAILURE);
    }

    int input_maze[10][10]; // Array fixo para o labirinto
    *rows = 0;
    *cols = 0;

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file)) {
        int col_count = 0;
        char *token = strtok(buffer, " ");
        while (token) {
            if (col_count >= 10) {
                fprintf(stderr, "Erro: mais de 10 colunas na linha %d\n", *rows + 1);
                fclose(file);
                exit(EXIT_FAILURE);
            }
            input_maze[*rows][col_count++] = atoi(token);
            token = strtok(NULL, " ");
        }

        if (*rows == 0) {
            *cols = col_count; // Define o número de colunas com base na primeira linha
        } else if (col_count != *cols) {
            fprintf(stderr, "Erro: número de colunas inconsistente na linha %d\n", *rows + 1);
            fclose(file);
            exit(EXIT_FAILURE);
        }

        (*rows)++;
        if (*rows > 10) {
            fprintf(stderr, "Erro: mais de 10 linhas no labirinto\n");
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }

    fclose(file);

    // Inicializa o labirinto com os valores lidos
    initialize_maze(maze, *rows, *cols, (int **)input_maze);
}