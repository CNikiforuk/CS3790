#include <stdio.h>
#include <ctime>
#include <fstream>

std::ifstream infile("op.dat");
char str[80];

int main (void){

int x = -1;
int y = 5;
int val = 0;
char op;

while (infile >> x >> op >> y)
{
	switch(op)
	{
		case '+': __asm__ ( "addl %%ebx, %%eax;" : "=a" (val) : "a" (x) , "b" (y) );
		break;
		case '-': __asm__ ( "subl %%ebx, %%eax;" : "=a" (val) : "a" (x) , "b" (y) );
		break;
		default : printf("Unkown Operation: "); val = 0;
	}

    printf("%d %c %d = %d \n", x, op, y, val);
}


}
