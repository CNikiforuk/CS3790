CC = g++
SRC = *.c
OBJ = *.o
ARGc = -Wall -Wunused -ansi -pedantic -ggdb -lpthread -std=c++11
ARGo = -lpthread -lm

TC = tar -cvf
TAR = ass3.tar

make: clean all
all:
	$(CC) -o ass3 $(ARGc) $(SRC) $(ARGo)
clean: 
	rm -f $(OBJ) $(TAR) ass3
tar:
	$(TC) $(TAR) ./
