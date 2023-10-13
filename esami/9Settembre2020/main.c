#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

typedef int pipe_t[2];

int main(int argc, char **argv){
	int pid;
	int pidFiglio;
	int ritorno = 0;
	int status;
	int Q;
	int q, j;
	int fd;
	int nr, nw;
	int L;
	pipe_t *pipe_fp;
	pipe_t *pipe_pf;

	if(argc < 5){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	L = atoi(argv[2]);
	if(L <= 0){
		printf("errore nel secondo parametro\n");
		exit(2);
	}

	/*controlliamo che dal terzo parametro siano tutti caratteri (singoli)*/
	for(i = 3; i < argc; i++){
		if(strlen(argv[i]) != 1){
			printf("errore in un parametro\n");
			exit(3);
		}
	}

	Q = argc - 3;
	/*allochiamo memoria per le pipe*/
	pipe_fp = (pipe_t *)malloc(Q*sizeof(pipe_t));
	pipe_pf = (pipe_t *)malloc(Q*sizeof(pipe_t));
	if((pipe_pf == NULL) || (pipe_fp == NULL)){
		printf("errore nella malloc\n");
		exit(4);
	}

	/*creiamo le pipe*/
	for(q = 0; q < Q; q++){
		if(pipe(pipe_pf[q]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(5);
		}
		if(pipe(pipe_fp[q]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(6);
		}
	}

	/*creazione figli*/
	for(q = 0; q < Q; q++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(7);
		}
		if(pid == 0){
			/*codice figlio*/
			
		}
	}
	/*codice padre*/

		
	

	exit(0);
}
