#ifndef ass4_h
#define ass4_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>


#define DIRSIZE 32
#define FATSIZE 1024
#define BLOCKSIZE 512
#define FILELENGTH 12


//filesystem.mfs, open with streams.
//open get put close, U CAN READ WHOLE FILESYSTEM IN


typedef struct{

        char name[FILELENGTH];
        int start;
        int size;

}direntry;

direntry new_direntry(char name[FILELENGTH], int start, int size){
	direntry p;
	strcpy(p.name, name);
	p.start = start;
	p.size = size;

	return p;
}
 
typedef direntry DIRECTORY[DIRSIZE]; //	A DIRECTORY IS AN ARRAY OF direntries, OF SIZE DIRSIZE
typedef short FAT[FATSIZE];
typedef unsigned char BLOCK[BLOCKSIZE];

typedef struct {

	DIRECTORY d;
	FAT f;
	BLOCK disk[FATSIZE]; 			//array of fatsize # of blocks, size blocksize;

}filesys;


void format(){ //creates initial filesystem

	filesys fs;
	fs.d[0] = new_direntry("free", 0, FATSIZE*BLOCKSIZE);

	for(int i=0;i<FATSIZE;i++){
		fs.f[i] = -2;
	}

	memset(fs.disk, 0, FATSIZE);

	int fd = open("floppy.mfs", O_CREAT | O_WRONLY);
	write (fd, &fs, sizeof(fs));
	close(fd);
	
}


#define OFFSET (DIRSIZE*sizeof(direntry) + FATSIZE*sizeof(short))
#endif
