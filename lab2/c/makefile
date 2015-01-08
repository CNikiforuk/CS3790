CC = gcc
SRC = lab2.c
OBJ = lab2.o
ARGc = -Wall -Wunused -ansi -pedantic -ggdb -lpthread -std=c99
ARGo = -lpthread -lm

TC = tar -cvf
TAR = ass.tar

make: clean all
all:
	$(CC) -o lab2 $(ARGc) $(SRC) $(ARGo)
clean: 
	rm -f $(OBJ) $(TAR) lab2
tar:
	$(TC) $(TAR) ./
