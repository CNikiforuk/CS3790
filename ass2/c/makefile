CC = gcc
SRC = *.c
OBJ = *.o
ARGc = -Wall -Wunused -ansi -pedantic -ggdb -lpthread -std=c99
ARGo = -lpthread -lm

TC = tar -cvf
TAR = ass2.tar

make: clean all
all:
	$(CC) -o ass2 $(ARGc) $(SRC) $(ARGo)
clean: 
	rm -f $(OBJ) $(TAR) ass2
tar:
	$(TC) $(TAR) ./
