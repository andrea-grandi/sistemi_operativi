#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

typedef int pipe_t[2];

int main(int argc, char **argv){
	int pid;
	int pidFiglio;
	int ritorno = 0;
	int status;
	int fd;
	int B;
	int b, a;
	char *buff;
	pipe_t *piped;
	int nr, nw;
	int B, L;

	if(argc != 4){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	B = atoi(argv[2]);
	L = atoi(argv[3]);
	if((B <= 0) || (L <= 0)){
		printf("errore numeri non positivi\n");
		exit(3);
	}

	buff = (char *)malloc(B*sizeof(char));
	if(buff == NULL){
		printf("errore nella malloc\n");
		exit(2);
	}

	sprintf(



