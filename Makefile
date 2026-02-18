CC = gcc -Wall -Wextra -std=c99

BIN = bin
OBJ = obj
SRC = src
HEADERS = headers

all: $(BIN)/prog_a

$(OBJ)/%.o: $(SRC)/%.c $(HEADERS)/%.h
	$(CC) -c $< -o $@

$(BIN)/prog_a: $(OBJ)/main.o $(OBJ)/distribution.o $(OBJ)/environnement.o $(OBJ)/particules.o $(OBJ)/interface.o
	$(CC) $< -o $@ 

clean:
	rm $(OBJ)/*.o
	rm $(BIN)/prog_a
