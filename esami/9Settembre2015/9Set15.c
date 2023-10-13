#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef int pipe_t[2];

int main(int argc, char **argv){
	int *pid;
	int  pidFiglio, ritorno, status;
	int N;
	int *confronto;
	int i, j;
	pipe_t *pipe_pf;
	pipe_t *pipe_fp;
	int fd;
	char ch, c;
	char token = 'v';


	if(argc < 4){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	
	N = argc - 2; //teniamo conto solo delle gerarchie passate, il file ci servira in seguito

	if((pid = (int *)malloc(N*sizeof(int))) == NULL){
		printf("errore nella malloc\n");
		exit(9);
	}

	if((confronto = (int *)malloc(N*sizeof(int))) == NULL){
		printf("errore nella malloc\n");
		exit(10);
	}

	for(i = 0; i < N; i++){
		confronto[i] = 1;
	}

	/*allochiamo la memoria per le pipe*/
	pipe_pf = (pipe_t *)malloc(N*sizeof(pipe_t));
	pipe_fp = (pipe_t *)malloc(N*sizeof(pipe_t));

	if((pipe_fp == NULL) || (pipe_pf == NULL)){
		printf("errore nella malloc\n");
		exit (2);
	}

	/*creazione pipe*/
	for(i = 0; i < N; i++){
		if (pipe(pipe_pf[i]) < 0){
			printf("errore nella creazione pipe\n");
			exit(3);
		}
		if(pipe(pipe_fp[i]) < 0){
			printf("errore nella creazione pipe\n");
			exit(4);
		}
	}

	/*apertura file passato come ultimo argomento*/
	if((fd = open(argv[argc - 1], O_RDONLY)) < 0){
		printf("errore in apertura file\n");
		exit(5);
	}

	printf("DEBUG: sono il processo padre con pid %d e sto per generare %d processi figli\n", getpid(), N);

	/*generiamo i processi figli*/

	for (i = 0; i < N; i++){
		if ((pid[i] = fork()) < 0){
			printf("errore nella fork\n");
			exit(6);
		}
		if(pid == 0){
			/*codice figlio*/
			printf("DEBUG: sono il figlio con pid %d e indice %d associato al file %s\n", getpid(), i, argv[i + 1]);
			/*chiudo le pipe che il figlio non usa*/
			for(j = 0; j < N; j++){
				/* 0 --> std input, 1 --> std output*/
				close(pipe_fp[j][0]);
				close(pipe_pf[j][1]);
				if (i != j){
					close(pipe_fp[j][1]);
					close(pipe_pf[j][0]);
				}
			}
			/*apro il file associato*/
			if((fd = open(argv[i + 1], O_RDONLY)) < 0){
				printf("errore in apertura file\n");
				exit(-1);	
			}

			/*ora devo leggere il file, ma solo dopo aver ricevuto dal padre OK*/
			while(read(pipe_pf[i][0], &token, 1)){
				if(token == 't'){
					break;
				}
				read(fd, &ch, 1);
				write(pipe_fp[i][1], &ch, 1);
			}
			exit(0);
			/*fine codice figlio*/
		}
	}
	/*codice padre*/
	/*chiudo i lati delle pipe che non usa il padre*/
	for(i = 0; i < N; i++){
		close(pipe_pf[i][0]);
		close(pipe_fp[i][1]);
	}

	while(read(fd, &c, 1)){
		for(i = 0; i < N; i++){
			if(confronto[i]){
				write(pipe_pf[i][1], &token, 1);
				read(pipe_fp[i][0], &c, 1);
				if(ch != c){
					confronto[i] = 0;
				}
			}
		}
	}

	for(i = 0; i < N; i++){
		if(!confronto[i]){
			if(kill(pid[i], SIGKILL) == -1){
				printf("figlio con pid %d non esiste qq quindi e' gia terminato\n", pid[i]);
			}
		}else{
			token='t';
			write(pipe_pf[i][1], &token, 1);
		}
	}

	/*padre aspetta i figli*/
	for(i = 0; i < N; i++){
		if((pidFiglio =wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(7);
		}
		if((status & 0xFF) != 0){
			printf("figlio con pid %d terminato in modo anomalo\n", pidFiglio);
		}else{
			ritorno = ((status >> 8) & 0xFF);
			printf("il figlio con pid %d e indice %d ha ritornato %d\n", pidFiglio, i, ritorno);
			for(j = 0; j < N; j++){
				if(pid[j] == pidFiglio){
					printf("figlio con pid %d ha verificato che il file %s e' uguale al file %s\n", pidFiglio, argv[j+1], argv[i+1]);
				}
			}
		}
	}
	exit(0);
}
