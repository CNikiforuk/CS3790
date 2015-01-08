CC = g++
SRC = asm.cpp
OBJ = asm.o

TC = tar -cvf
TAR = asm.tar

make: clean prog
prog: $(OBJ)
	$(CC) $(OBJ) -o $@
asm.o:
	$(CC) -c $(SRC)
clean: 
	rm -f $(OBJ) $(TAR) prog
tar:
	$(TC) $(TAR) ./
