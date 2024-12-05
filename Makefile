# Diretórios
SRC_DIR = src
OBJ_DIR = builds
INC_DIR = include
BIN_DIR = bin

#Arquivos de código fonte
CLIENT_SRC = $(SRC_DIR)/client.c $(SRC_DIR)/maze_logics.c $(SRC_DIR)/sockets_utils.c
SERVER_SRC = $(SRC_DIR)/server.c $(SRC_DIR)/maze_logics.c $(SRC_DIR)/sockets_utils.c

#Arquivos objeto
CLIENT_OBJ = $(CLIENT_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
SERVER_OBJ = $(SERVER_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

#Binários
CLIENT_BIN = $(BIN_DIR)/client
SERVER_BIN = $(BIN_DIR)/server

#Compilador e Flags
CC = gcc
CFLAGS = -wall -g -I$(INC_DIR)

# Alvo Padrão
all: $(CLIENT_BIN) $(SERVER_BIN)

#Compilação cliente
$(CLIENT_BIN): $(CLIENT_OBJ)
	$(CC) $(CLIENT_OBJ) -o $(CLIENT_BIN)

#compilação servidor
$(SERVER_BIN): $(SERVER_OBJ)
	$(CC) $(SERVER_OBJ) -o $(SERVER_BIN)

#Compilar arquivos de código em objetos
$(OBJ_DIR)/%.o = $(SRC_DIR)/%.c 
	$(CC) $(CFLAGS) -c $< -o $@

#Regra para limpar os arquivos gerados
clean: 
	rm -rf $(OBJ_DIR)/*.o $(CLIENT_BIN) $(SERVER_BIN)

#Regra para criar o diretório de build
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

#Regra para executar os binários do cliente e servidor
run_client: %(CLIENT_BIN)
	./$(CLIENT_BIN)

run_server: $(SERVER_BIN)
	./$(SERVER_BIN)


.PHONY: all clean