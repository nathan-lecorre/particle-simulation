CC = gcc -Wall -Wextra -std=c99

obj="./obj"
src="./src"
bin="./bin"
head="./headers"

all:prog_a

%.o:%.c %.h
	CC -c $< 

prog_a:
	CC  $^ -o $@
