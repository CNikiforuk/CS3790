CC = gcc
BIN = ass4
SRC = *.c
OBJ = *.o
ARGc = -Wall -Wunused -ansi -pedantic -ggdb -lpthread -std=c99
ARGo = -lpthread -lm

TC = tar -cvf
TAR =  CS3790-5030337-$(BIN).tar

all: clean $(BIN)
$(BIN):
	$(CC) -o $@ $(ARGc) $(SRC) $(ARGo)
clean: 
	rm -f $(OBJ) $(TAR) $(BIN)
tar: clean
	$(TC) $(TAR) ./
