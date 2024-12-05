#ifndef MAZE_LOGICS_H
#define MAZE_LOGICS_H

#define MAX_MAZE_ROWS 10
#define MAX_MAZE_COLS 10

// Representação das movimentações
#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4

// Representação para identificar celulas do labirinto descobertas pelo jogador
#define DISCOVERED 1
#define UNDISCOVERED 0

// Representação dos elementos do labirinto no servidor
#define INVALID -1
#define SERVER_WALL 0
#define SERVER_EMPYT 1
#define SERVER_ENTRY 2
#define SERVER_EXIT 3
#define SERVER_UNDISCOVERED 4
#define SERVER_PLAYER 5

// Representação dos elementos do labirinto no cliente (Forma textual)
#define CLIENT_WALL '#'
#define CLIENT_EMPYT '_'
#define CLIENT_ENTRY '>'
#define CLIENT_EXIT 'X'
#define CLIENT_UNDISCOVERED '?'
#define CLIENT_PLAYER '+'

// Struct cirado para representar o labirinto textualmente
typedef struct {
    int nrows;
    int ncols;
    char grid[MAX_MAZE_ROWS][MAX_MAZE_COLS];
} Textual_Maze;

// Struct criado para representar o Labirinto numericamente
typedef struct {
    int nrows;
    int ncols;
    int grid[MAX_MAZE_ROWS][MAX_MAZE_COLS];
} Numerical_Maze;

// Representa o labirinto
typedef struct {
    int nrows;
    int ncols;
    int grid[MAX_MAZE_ROWS][MAX_MAZE_COLS];
    int discovered_cells[MAX_MAZE_ROWS][MAX_MAZE_COLS];
    int player_x;
    int player_y;
} Maze;

// Inicializa o labirinto
void initialize_maze(Maze *maze, int nrows, int ncols, int **input_maze);

// Move o jogador no labirinto
// Retorna 1 se o movimento for bem sucedido e 0 caso contrário
int move_player(Maze *maze, int direction, int* valid_moves);

// Retorna 1 caso o jogador tenha vencido e 0 caso contrário
int check_win_condition(const Maze *maze);

// Retorna uma lista dos movimentos válidos em forma de inteiros
void avaliate_valid_moves(const Maze *maze, int* valid_moves);

// Retorna o labirinto na visão do jogador na forma inteira
void represent_maze_vision(const Maze *maze, Numerical_Maze *player_maze_vision);

// Retorna o labirinto para o cliente na forma textual
void translate_maze_vision(const Numerical_Maze *maze, Textual_Maze *player_maze_vision);

// Descobre novas celulas do labirinto por causa da nova posição do jogador
void discover_cells(Maze* maze);

#endif // MAZE_LOGICS_H