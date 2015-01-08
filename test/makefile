CC = g++
BIN = test
SRC = *.cpp
OBJ = *.o
ARGc = -std=c++11
ARGo =

TC = tar -cvf
TAR =  CS3790-5030337-$(BIN).tar

all: clean $(BIN)
$(BIN):
	$(CC) -o $@ $(ARGc) $(SRC) $(ARGo)
clean: 
	rm -f $(OBJ) $(TAR) $(BIN)
tar: clean
	$(TC) $(TAR) ./
