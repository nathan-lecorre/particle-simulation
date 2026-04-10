CC = gcc -Wall -Wextra -std=c99

BIN = bin
OBJ = obj
SRC = src
HEADERS = headers

all: $(BIN)/prog_a

$(OBJ)/%.o: $(SRC)/%.c $(HEADERS)/%.h
	$(CC) -g -lm -c $< -o $@

$(OBJ)/main.o: $(SRC)/main.c
	$(CC) -g -lm -c $< -o $@

$(OBJ)/%.o : research_10/experiments/%.c research_10/experiments/%.h
	$(CC) -g -lm -c $< -o $@

$(BIN)/prog_a: $(OBJ)/main.o $(OBJ)/distribution.o $(OBJ)/environnement.o $(OBJ)/particules.o $(OBJ)/interface.o $(OBJ)/optimization.o
	$(CC) -g $^ -o $@ -lm

PBM_FILES = $(shell find . -name "*.pbm" | sed 's/.*/"&"/')

clean:
	rm $(OBJ)/*.o || true
	rm $(BIN)/prog_a || true
	rm -f $(PBM_FILES) || true
