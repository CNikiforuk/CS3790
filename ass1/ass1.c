/*
#################--Description--#################
Parent monitor program. Forks a child and executes a script specified by user input.

#################--Expected use--#################
Run program in terminal to begin. Give path of executable script to begin that process.
Tested script was repeat.sh, which takes 1 parameter of a string to repeatedly output.

#################-----Author-----#################
Carlos Nikiforuk, reliant on Franco Carlacci's initial example.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

enum status { on, off };
typedef enum status status;
typedef bool boolean;

unsigned short i = 0;	//PID array index
int n = 0;		//number of children
int pid[5];		//PID array containing child PID's
status flag = on;

struct sigaction newhandler, oldhandler;
sigset_t sig;


//###########################--Functions--###############################//

void setblock( int fd, boolean block)
{
   static int blockf, nonblockf;
   static boolean first = true;

   int flags;

   if (first) {
      first = false;
      if (( flags = fcntl(fd,F_GETFL,0)) == -1)
      {
         fprintf(stderr," fcntl - error \n");
         exit(1);
      }
     blockf = flags & ~ O_NDELAY;
     nonblockf = flags | O_NDELAY;
  }
  if ( fcntl(fd,F_SETFL, block ? blockf : nonblockf) == -1 )
      {
         fprintf(stderr," fcntl2 - error \n");
         exit(1);
      }
}

void next( int code)
/*
Stops the current process, increments the index
i in the PID array and begins the next process.
*/
{
  if(n>0){
  kill(pid[i++],SIGSTOP);
  if (i%n==0) i = 0;
  kill(pid[i],SIGCONT);
  }
}


int myfork(const char * prog)
/*
Forks a child process, and begins executing the 
inputted executable with the passed string.
*/
{
	char par[2]="0";
	pid[n] = fork();
	if(pid[n]==0){
		par[0] = '0'+ n;
		execl("/bin/sh", "sh", prog, par, NULL);		
	}
	n++;
}

//###########################--Main--###############################//

int main()
{
    char mess[80];
    int fd;
    int numch;
	
	fd = open("/dev/tty",O_RDWR);
	setblock(fd,false);
	
	sigemptyset(&sig);  /* clear the signal mask */
	newhandler.sa_handler = next;
	newhandler.sa_mask = sig;
	newhandler.sa_flags = 0;

	if ( sigaction(SIGALRM,&newhandler,&oldhandler) == -1 )
	{
		printf("1 - can't install signal handler \n");
		exit(-1);
	}
	while (1) {
		alarm(3);
		pause();
		switch  ( numch = read(fd,mess,80))  {
		case -1 :
		case 0 :
		break;
		default: 
		fprintf(stderr," %d   <%s>\n",numch,mess);
		mess[numch-1] = '\0';		
		myfork(mess); /* call myfork to fork a child */
		}
		fprintf(stderr," in parent\n");
	}
}
