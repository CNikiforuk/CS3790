#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include "bufio.h"
extern int errno;
STREAM *Sopen(char *path, char *dir)
{
	STREAM *z;
	int fd, flags;
	switch(dir[0]) {
		case 'r' :
		flags = O_RDONLY;
		break;
		case 'w' :
		flags = O_WRONLY | O_CREAT | O_TRUNC;
		break;
		default :
		errno = SEINVAL;
		return ( NULL);
	}
	if (( fd = open(path,flags,0666)) == -1 )
	return ( NULL);
	if (( z = (STREAM *)malloc(sizeof(STREAM))) == NULL ) {
		errno = SENOMEM;
		return (NULL);
	}
	z->fd = fd;
	z->dir = dir[0];
	z->total = z-> next = 0;
	return (z);
}
static BOOLEAN readbuf(STREAM *z)
{
	switch ( z->total = read(z->fd,z->buf,sizeof(z->buf))) {
		case -1 :
		return ( FALSE);
		case 0 :
		errno = 0;
		return (FALSE);
		default:
		z->next = 0;
		return (TRUE);
	}
}
static BOOLEAN writebuf(STREAM *z)
{
	int n, total;
	total = 0;
	while (total < z->next) {
		if ((n= write(z->fd,&z->buf[total], z->next -total)) == -1 )
		return ( FALSE);
		total += n;
	}
	z->next = 0;
	return ( TRUE);
}
int Sgetc(STREAM *z)
{
	int c;
	if ( z->next >= z-> total && !readbuf(z)) return ( -1);
	return ( z->buf[z->next++] & 0377);
}
BOOLEAN sputc( STREAM *z, char c)
{
	z->buf[z->next++] = c;
	if (z->next >= sizeof(z->buf)) return (writebuf(z));
	return(TRUE);
}
BOOLEAN Sclose(STREAM *z)
{
	int fd;
	if ( z->dir == 'w' && !writebuf(z)) return FALSE;
	fd = z->fd;
	free(z);
	return ( close(fd) != -1);
}


