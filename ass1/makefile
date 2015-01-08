CC = g++
SRC = ass1.c
OBJ = ass1.o

TC = tar -cvf
TAR = ass.tar

ass1: ass1.o
	$(CC) ass1.o -o $@
ass1.o:
	$(CC) -c $(SRC) 
clean: 
	rm -f $(OBJ) $(TAR) ass1
tar:
	$(TC) $(TAR) ./
