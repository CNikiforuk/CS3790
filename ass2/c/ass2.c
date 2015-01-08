/*
	Author: Carlos Nikiforuk
	Assignment #2

	Description: Reader Writer example solution. Random number of r/w.

	Expected Use: ./ass2
	
*/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

void *reader();
void *writer();

int readers;							//keeps track of number of readers

sem_t count;
sem_t sem;


/*==================================================*/
int main(int argc, char *argv[])
//Main
/*==================================================*/
{

	sem_init(&count, 0, 1);
	sem_init(&sem, 0, 1);

	srand (time(NULL));					//initialize rand seed
	int threads = rand() % 5 + 4;

	pthread_t tid[threads];
	pthread_attr_t attr[threads];

	for(int m=0;m<threads;m++)		
	{	
		pthread_attr_init(&attr[m]);			//create readers or writers randomly.

		if((rand() % 5) < 2)pthread_create(&tid[m], &attr[m], &writer, NULL);
		else pthread_create(&tid[m], &attr[m], &reader, NULL);
	}


	for(int m=0;m<threads;m++)
	{	
		pthread_join(tid[m], NULL);
	}

}


/*==================================================*/
void *reader()
//Function executed by reader threads.
/*==================================================*/
{	
	sem_wait(&count);
	readers++;
	if(readers == 1) sem_wait(&sem);	
	sem_post(&count);


	printf("%u: Reading...\n", (int)pthread_self());	
	sleep(rand()%2+1);
	printf("%u: Done reading!\n", (int)pthread_self());
	
	sem_wait(&count);
	readers--;
	if(readers == 0){ sem_post(&sem); printf("\n");}
	sem_post(&count);
	
	pthread_exit(0);
}

/*==================================================*/
void *writer()
//Function executed by writer threads.
/*==================================================*/
{	
	sem_wait(&sem);

	printf("%u: Writing...\n", (int)pthread_self());		
	sleep(rand()%3+1);
	printf("%u: Done writing!\n\n", (int)pthread_self());
	
	sem_post(&sem);
	
	pthread_exit(0);
}

