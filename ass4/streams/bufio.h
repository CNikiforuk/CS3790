#define SENOMEM 1001
#define SEINVAL 1002
#define BUFSIZE 512
typedef enum {FALSE, TRUE } BOOLEAN;
typedef struct {
	int fd;
	char dir;
	int total;
	int next;
	char buf[BUFSIZE];
} STREAM;
STREAM *Sopen(char *, char *);
int Sgetc(STREAM *);
BOOLEAN Sputc(STREAM *,char);
BOOLEAN Sclose(STREAM *);
