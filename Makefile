CC = gcc -Wall -Wextra -std=c99

BIN = bin
OBJ = obj
SRC = src
HEADERS = headers

all: $(BIN)/prog_d

$(OBJ)/%.o: $(SRC)/%.c $(HEADERS)/%.h
	$(CC) -g -lm -c $< -o $@

$(OBJ)/main.o: $(SRC)/main.c
	$(CC) -g -lm -c $< -o $@

$(OBJ)/%.o : research_10/experiments/%.c research_10/experiments/%.h
	$(CC) -g -lm -c $< -o $@

$(BIN)/prog_d: $(OBJ)/distribution.o $(OBJ)/environnement.o $(OBJ)/particules.o $(OBJ)/interface.o $(SRC)/experiments.c
	$(CC) -g $^ -o $@ -lm

PBM_FILES = $(shell find . -name "*.pbm" | sed 's/.*/"&"/')

clean:
	rm $(OBJ)/*.o || true
	rm $(BIN)/prog_d || true
	rm -f $(PBM_FILES) || true
	rm  test_*.txt || true
