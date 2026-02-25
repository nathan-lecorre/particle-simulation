CC = gcc -Wall -Wextra -std=c99

BIN = bin
OBJ = obj
SRC = src
HEADERS = headers

all: $(BIN)/prog_a

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) -g -lm -c $< -o $@

$(BIN)/prog_a: $(OBJ)/main.o $(OBJ)/distribution.o $(OBJ)/environnement.o $(OBJ)/particules.o $(OBJ)/interface.o
	$(CC) -g $^ -o $@ -lm

clean:
	rm $(OBJ)/*.o
	rm $(BIN)/prog_a
	rm *.pbm
