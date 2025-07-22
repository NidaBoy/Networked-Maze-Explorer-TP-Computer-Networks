#include "maze_logics.h"
#include <string.h>

// Inicializa o labirinto
void initialize_maze(Maze *maze, int nrows, int ncols, int **input_maze){
    maze->nrows = nrows;
    maze->ncols = ncols;
    for(int i = 0; i < MAX_MAZE_ROWS; i++){
        for(int j = 0; j < MAX_MAZE_COLS; j++){
            if (i < nrows && j < ncols){
                maze->grid[i][j] = input_maze[i][j];
                maze->discovered_cells[i][j] = 0;

                if(input_maze[i][j] == SERVER_ENTRY){
                    maze->player_x = i;
                    maze->player_y = j;
                }
                
            } else{
                maze->grid[i][j] = INVALID;
                maze->discovered_cells[i][j] = INVALID;
            }
        }
    }
    discover_cells(maze);
}

// Move o jogador no labirinto
// Retorna 1 se o movimento for bem sucedido e 0 caso contrário
int move_player(Maze *maze, int direction, int* valid_moves){
    switch (direction){  
        case UP: // Cima
            maze->player_x--;
            discover_cells(maze);
            avaliate_valid_moves(maze, valid_moves);
            return 1;

        case RIGHT: // Direita
            maze->player_y++;
            discover_cells(maze);
            avaliate_valid_moves(maze, valid_moves);
            return 1;

        case DOWN: // Baixo
            maze->player_x++;
            discover_cells(maze);
            avaliate_valid_moves(maze, valid_moves);
            return 1;

        case LEFT: // Esquerda
            maze->player_y--;
            discover_cells(maze);
            avaliate_valid_moves(maze, valid_moves);
            return 1;
        default:
            break;
    }

    return 0;
}

// Retorna 1 caso o jogador tenha vencido e 0 caso contrário
int check_win_condition(const Maze *maze){
    if(maze->grid[maze->player_x][maze->player_y] == SERVER_EXIT){
        return 1;
    }
    return 0;
}

// Calcula uma lista dos movimentos válidos em forma de inteiros
void avaliate_valid_moves(const Maze *maze, int* valid_moves){
    int i = 0;
    
    // Limpa o array valid_moves
    for(int i = 0; i < 100; i++){
        valid_moves[i] = 0;
    }

    // Descobrir pra cima
    if((maze->player_x > 0) && (maze->grid[maze->player_x - 1][maze->player_y] == SERVER_EMPYT)){
        valid_moves[i] = UP;
        i++;
    }

    // Descobrir para a direita
    if((maze->player_y < maze->ncols - 1) && (maze->grid[maze->player_x][maze->player_y + 1] == SERVER_EMPYT)){
        valid_moves[i] = RIGHT;
        i++;
    }

    // Descobrir para baixo
    if((maze->player_x < maze->nrows - 1) && (maze->grid[maze->player_x + 1][maze->player_y] == SERVER_EMPYT)){
        valid_moves[i] = DOWN;
        i++;
    }

    // Descobrir para esquerda
    if(maze->player_y > 0 && maze->grid[maze->player_x][maze->player_y - 1] == SERVER_EMPYT){
        valid_moves[i] = LEFT;
        i++;
    }
}

// Faz a representação do labirinto na visão do jogador na forma inteira
void represent_maze_vision(const Maze *maze, Numerical_Maze *player_maze_vision){

    // Flag para saber se o jogo foi vencido
    int is_game_won = check_win_condition(maze);

    // Se o jogo ainda não ter sido vencido ele apresentará regiões não descobertas e a posição do jogador
    for(int i = 0; i < maze->nrows; i++){
        for(int j = 0; j < maze->ncols; j++){

            // Avalia se o jogo ainda não acabou e se a posição atual é a do jogador
            if(!is_game_won && i == maze->player_x && j == maze->player_y){
                player_maze_vision->grid[i][j] = SERVER_PLAYER;
            }else{
                // Avalia se a célula já foi descoberta ou o jogo já foi ganho
                if(maze->discovered_cells[i][j] || is_game_won)
                    player_maze_vision->grid[i][j] = maze->grid[i][j];
                else
                    player_maze_vision->grid[i][j] = SERVER_UNDISCOVERED;
            }
        }
    }
}

// Retorna o labirinto para o cliente na forma textual
void translate_maze_vision(const Numerical_Maze *maze, Textual_Maze *player_maze_vision){
    for(int i = 0; i < maze->nrows; i++){
        for(int j = 0; j < maze->ncols; j++){

            switch (maze->grid[i][j]){

                case SERVER_EMPYT:
                    player_maze_vision->grid[i][j] = CLIENT_EMPYT;
                    break;

                case SERVER_ENTRY:
                    player_maze_vision->grid[i][j] = CLIENT_ENTRY;
                    break;
                
                case SERVER_EXIT:
                    player_maze_vision->grid[i][j] = CLIENT_EXIT;
                    break;
                
                case SERVER_PLAYER:
                    player_maze_vision->grid[i][j] = CLIENT_PLAYER;
                    break;
                
                case SERVER_UNDISCOVERED:
                    player_maze_vision->grid[i][j] = CLIENT_UNDISCOVERED;
                    break;
                
                case SERVER_WALL:
                    player_maze_vision->grid[i][j] = CLIENT_WALL;
                    break;
                
                default:
                    break;
            }
        }
    }
}

// Descobre novas celulas do labirinto por causa da nova posição do jogador
void discover_cells(Maze* maze){

    // Descobrir a própia posição
    maze->discovered_cells[maze->player_x][maze->player_y] = 1;

    // Descobrir pra cima
    if(maze->player_x > 0){
        maze->discovered_cells[maze->player_x - 1][maze->player_y] = 1;
    }

    // Descobrir a diagonal para cima e direita
    if(maze->player_x > 0 && maze->player_y < maze->ncols - 1){
        maze->discovered_cells[maze->player_x - 1][maze->player_y + 1] = 1;
    }

    // Descobrir para a direita
    if(maze->player_y < maze->ncols - 1){
        maze->discovered_cells[maze->player_x][maze->player_y + 1] = 1;
    }

    // Descobrir a diagonal para baixo e direita
    if(maze->player_x < maze->nrows - 1 && maze->player_y < maze->ncols - 1){
        maze->discovered_cells[maze->player_x + 1][maze->player_y + 1] = 1;
    }

    // Descobrir para baixo
    if(maze->player_x < maze->nrows - 1){
        maze->discovered_cells[maze->player_x + 1][maze->player_y] = 1;
    }

    // Descobrir para diagonal para baixo e esquerda
    if(maze->player_x < maze->nrows - 1 && maze->player_y > 0){
        maze->discovered_cells[maze->player_x + 1][maze->player_y - 1] = 1;
    }

    // Descobrir para esquerda
    if(maze->player_y > 0){
        maze->discovered_cells[maze->player_x][maze->player_y - 1] = 1;
    }

    // Descobrir para diagonal para cima e esquerda
    if(maze->player_x > 0 && maze->player_y > 0){
        maze->discovered_cells[maze->player_x - 1][maze->player_y - 1] = 1;
    }
}
