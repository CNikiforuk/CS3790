/*
	Author: Carlos Nikiforuk
	Assignment #2

	Description: Memory simulation. Random size and duration requests created.

	Expected Use: ./ass3 <choice> <rand seed>

	choice 0 is first fit
	choice 1 is best fit
	choice 2 is worst fit
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define MIN_LEASE 40
#define MAX_LEASE 70
#define MIN_SIZE 50
#define MAX_SIZE 350
#define TIME_LIMIT 1000
#define MEMORY_SIZE 1000
#define REQUEST_INTERVAL 10

typedef struct range range;
typedef struct freeNode freeNode;
typedef struct allocNode allocNode;
typedef struct memSim memSim;

struct memSim{
	int memory; 						//available memory
	int success;						//mem requests satisfied
	int fails;							//mem requests unsatisfied
	
	int stats_size[2] = {MAX_SIZE, 0};	//min and max size
	int stats_lease[2] = {MAX_LEASE, 0};//min and max lease
	double avg_size;					//avg size, maintained as a moving average
	double avg_lease;					//avg lease, maintained as a moving average
	
	freeNode *freeList;		  			//freeList pointer
	allocNode *allocList;				//allocList pointer
	
	memSim(int mem=MEMORY_SIZE, freeNode *f = NULL, allocNode *a = NULL) : memory(mem), success(0), fails(0), freeList(f), allocList(a) {}
	
};

struct range{
	int start;		// start address
	int size;		// size of range

	range(int s=0, int x=10) : start(s), size(x) {}	
};

struct freeNode{
	range hole;
	freeNode *next;	// pointer to next node

	freeNode(range r, freeNode *p=NULL) : hole(r), next(p) {}	
};


struct allocNode{
	range allocated;
	int leaseExpiry;	// time at which this block will be returned to free list
	allocNode *next;	// pointer to next node

	allocNode(range r, int s=0, allocNode *p=NULL) : allocated(r), leaseExpiry(s), next(p) {}
};

/*==================================================*/
void allocDump(allocNode *p){
//Dumps allocList to output using printf, allocList pointer expected as param
/*==================================================*/
	printf("Allocate:\n");
	if(p == NULL) printf("EMPTY!\n");
	while(p != NULL){
		printf("	start: %3d size: %3d expiry: %4d \n", p->allocated.start, p->allocated.size, p->leaseExpiry);
		p = p->next;
	}
}

/*==================================================*/
void freeDump(freeNode *p){
//Dumps freeList to output using printf, freeList pointer expected as param
/*==================================================*/
	printf("Hole:\n");
	while(p != NULL){
		printf("	start: %3d size: %3d \n", p->hole.start, p->hole.size);
		p = p->next;
	}
}

/*==================================================*/
freeNode* findHole(int choice, freeNode *p, int size){
//Finds hole depending on parameter choice, which chooses different fit algorithms.
//int choice, pointer to freeList to search, and size of memory needed as parameters.
/*==================================================*/

	if(choice == 0){		//first fit
		while (p->next != 0 && p->hole.size < size){ 
					p = p->next;
				}
		return p;
	}
	else if(choice == 1){	//best fit
		int difference = MEMORY_SIZE+1;
		freeNode *bfit = p;
		while (p->next != 0){
			p = p->next;
			if ((p->hole.size - size) < difference && (p->hole.size - size) >= 0) {
				difference = p->hole.size-size; 
				bfit = p;
			}
		}
		return bfit;
	}
	else {					//worst fit
		int difference = size;
		freeNode *wfit = p;
		while (p->next != 0){
			p = p->next;
			if ((p->hole.size - size) > difference && (p->hole.size - size) >= 0) {
				difference = p->hole.size-size; 
				wfit = p;
			}
		}
		return wfit;
	}
}

/*==================================================*/
void addAlloc(allocNode *p, int start, int size, int lease){ //allocNode a has start, size, lease
//Adds node to allocList, expects pointer add new node at. Use with findHole function
/*==================================================*/

	allocNode *tmp = p->next;
	p->next = (allocNode*)malloc( sizeof(struct allocNode));
	p = p->next;
	p->allocated.start = start;
	p->allocated.size = size;
	p->leaseExpiry = lease;
	p->next = tmp;
}
/*==================================================*/
void addFree(freeNode *p, int size){
//Adjusts freeList when mem is allocated, requires freeList pointer and mem size
//Also creates a freeNode size 0, same location as the allocNode to prepare for mem release
/*==================================================*/

	freeNode *fswap = p->next;
	p->next = (freeNode*)malloc( sizeof(struct freeNode));
	p->next->next = fswap;
	p->next->hole.size = p->hole.size - size;
	p->next->hole.start = p->hole.start + size;								
	p -> hole.size = 0;
}
/*==================================================*/
void joinFree(freeNode *p){
//Joins adjacent free memory, expects pointer to freeList
/*==================================================*/

	p = p->next; //get past dummy head
	while(p->next != 0){
		if(p->hole.size > 0 && p->next->hole.size > 0){	
			p->hole.size += p->next->hole.size;
			p->next = p->next->next;
			continue;
		}
		p = p->next;
		if(p == NULL) break;
	}
}

/*==================================================*/
void releaseMem(freeNode *p, allocNode *q, long clock){
//Release memory when expiry time is reached
//Freelist pointer, allocList pointer, and clock time expected as parameters.
/*==================================================*/

	freeNode *start = p;
	allocNode *astart = q;
	
	while(q->next != NULL && q->next->leaseExpiry == clock){
		q = q->next;
		p = start;
		while(q->allocated.start != p->hole.start){
			p = p->next;
		}
		p->hole.size += q->allocated.size;
		allocNode *tmp = q->next;
		free(q);
		astart->next = tmp;
		q = astart;
		
	}
}

/*==================================================*/
int main(int argc, char *argv[]) {
//Main
/*==================================================*/
	int choice;
	
	if(argc > 1) choice = atoi(argv[1]);
	else choice = 0;
	if(argc > 2) srand(atoi(argv[2]));
	else srand (time(NULL));

	freeNode fhead = freeNode(range(-1, -1), NULL);			//freeList head
	allocNode ahead = allocNode(range(-1,-1), -1, NULL);	//allocList head
	freeNode start = freeNode(range(0, MEMORY_SIZE), NULL);
	memSim sim1 = memSim(MEMORY_SIZE, &fhead, &ahead);
	
	sim1.freeList->next = &start;
	
	
	printf("Allocate Memory between %d %d.\n", sim1.freeList->next->hole.start, sim1.freeList->next->hole.size);
	
	long clock = 0;
	while(++clock <= TIME_LIMIT){
		
		freeNode *ftmp = sim1.freeList;
		allocNode *atmp = sim1.allocList;
		
		if(sim1.allocList->next != NULL && sim1.allocList->next->leaseExpiry == clock){
			releaseMem(sim1.freeList, sim1.allocList, clock);
		}
		
		if(clock % REQUEST_INTERVAL == 0) {	
			int size = rand() % (MAX_SIZE - MIN_SIZE) + MIN_SIZE;
			int lease = (rand() % (MAX_LEASE - MIN_LEASE) + MIN_LEASE);
			if(size < sim1.stats_size[0]) sim1.stats_size[0] = size;
			if(size > sim1.stats_size[1]) sim1.stats_size[1] = size;
			sim1.avg_size -= sim1.avg_size / (sim1.success + sim1.fails + 1);
			sim1.avg_size += size / (sim1.success + sim1.fails + 1);
			
			if(lease < sim1.stats_lease[0]) sim1.stats_lease[0] = lease;
			if(lease > sim1.stats_lease[1]) sim1.stats_lease[1] = lease;
			sim1.avg_lease -= sim1.avg_lease / (sim1.success + sim1.fails + 1);
			sim1.avg_lease += lease / (sim1.success + sim1.fails + 1);
			
			lease += clock;
			
			//ALLOCATE MEM
			
			while(atmp->next != NULL && lease > atmp->next->leaseExpiry){
				atmp = atmp->next;
			}
			ftmp = findHole(choice, sim1.freeList, size);
			if(ftmp->hole.size >= size){
				addFree(ftmp, size);
				addAlloc(atmp, ftmp->hole.start, size, lease);
				printf("	Allocated %d, at start location %d, for %d cycles. \n", size, ftmp->hole.start, lease);
				sim1.success++;
			}
			//IF FAIL JOIN FREE MEM
			else{
				joinFree(sim1.freeList);
				printf("CANNOT ALLOCATE SIZE %d 	Success: %d  Fails: %d \n", size, sim1.success, ++sim1.fails);
			}
			
			freeDump(sim1.freeList->next);
			allocDump(sim1.allocList->next);
			//sleep(4);
		}
		
		
	}
	
	printf("\n\n\n\nDone! \n\n\n");
	freeDump(sim1.freeList->next);
	allocDump(sim1.allocList->next);
	
	printf("\nRequests: %d Satisfied: %d Unsatisfied: %d\n\n", sim1.success+sim1.fails, sim1.success, sim1.fails);
	printf("Size Stats  - min: %3d max: %3d avg: %1.2f\n", sim1.stats_size[0], sim1.stats_size[1], sim1.avg_size);
	printf("Lease Stats - min: %3d max: %3d avg: %1.2f\n", sim1.stats_lease[0], sim1.stats_lease[1], sim1.avg_lease);	
	
}
