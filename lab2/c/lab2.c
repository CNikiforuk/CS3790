/*
	Author: Carlos Nikiforuk
	Lab Assignment #2

	Description: Program for using multiple threads to find prime numbers.

	Expected Use: ./lab2 <num primes> <num threads>
	
*/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

enum boolean { false, true };
typedef enum boolean boolean;

boolean isPrime(int n);
void *runner(void *param);

/*==================================================*/
int main(int argc, char *argv[])
//main
/*==================================================*/
{	
	/* check input */
	if(argc != 3 || atoi(argv[2]) < 0 || atoi(argv[1]) < 0)
	{
		fprintf(stderr, "Usage: %s <upper limit> <num threads>\n", argv[0]);
		return -1;
	}

	int numthread; //holds the number of threads to create.
	
	if (atoi(argv[2]) > atoi(argv[1])) 	{ numthread = atoi(argv[1])/2; }
	else 					{ numthread = atoi(argv[2]); }

	pthread_t tid[numthread];
	pthread_attr_t attr[numthread];

	int interval = atoi(argv[1]) / numthread;	//how many numbers for each thread to process
	int remainder = atoi(argv[1]) % numthread;	//remainder for when (num % threads) != 0.
	int iv[numthread+1];				//interval array
	int *ip = iv;

	printf("Searching primes up to %d, using %d threads.\n\n", atoi(argv[1]), numthread);
	
	/* create intervals */
	for(int i=0;i<=numthread;i++)
	{
		iv[i] = i*interval;
		if(i==numthread){iv[i] += remainder;}
	}
	
	
	/* create threads */
	for(int m=0;m<numthread;m++)
	{	
		pthread_attr_init(&attr[m]);
		pthread_create(&tid[m], &attr[m], &runner, ip++);
	}

	for(int m=0;m<numthread;m++)
	{	
		pthread_join(tid[m], NULL);
	}
	
}

/*==================================================*/
boolean isPrime(int n)
//returns true if passed value is a prime number false if it is a composite number.
/*==================================================*/
{
    	 if (n <= 3) { return n > 1; } 
	else if (n % 2 == 0 || n % 3 == 0) { return false;}  
	else 
	{
        for (int i = 5; i < sqrt(n) + 1; i += 6) 
		{
            if (n % i == 0 || n % (i + 2) == 0) { return false; }
        }
        return true;
    }
}

/*==================================================*/
void *runner(void *param)
//Function executed by threads. Expects pointer to array of ints for intervals.
/*==================================================*/
{
	int *start = param;	

	for(int i=*start;i<*(start+1);i++){
		if(isPrime(i)) printf("t%d: %d \n",*start/(*(start+1) - *start), i);
	}
	
	pthread_exit(0);
}

