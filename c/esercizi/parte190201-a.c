#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

typedef int pipe_t[2];

int main(int argc, char **argv){

	int pid;
	int pidFiglio;
	int i;
	int N;
	int status, ritorno;
	pipe_t *p;
	int j;
	long int cont;
	int fdr;
	char c;

	if(argc < 4){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	N = argc - 2;

	//controllo caratteri
	for(i = 0; i < N; i++){
		if(strlen(argv[i+2]) != 1){
			printf("errore nella stringa %s: non singolo carattere\n", argv[i+2]);
			exit(2);
		}
	}
		
	p = (pipe_t *)malloc(sizeof(pipe_t) * N);
	if(p == NULL){
		printf("errore nella malloc\n");
		exit(3);
	}

	//creazione pipe
	for(i = 0; i < N; i++){
		if(pipe(p[i]) < 0){
			printf("errore creazione pipe\n");
			exit(4);
		}
	}

	printf("sono il processo padre con pid %d e sto per creare %d figli\n", getpid(), N);
	for(i = 0; i < N; i ++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(5);
		}
		if(pid == 0){
			printf("sono il figlio con pid %d di indice %d associato al carattere %c\n", getpid(), i, argv[i+2][0]);
			
			//chiusura pipe che non usa
			for(j = 0; j < N; j++){
				close(p[j][0]);
				if(j != i){
					close(p[j][1]);
				}
			}

			if((fdr = open(argv[1], O_RDONLY)) < 0){
				printf("errore in apertura file\n");
				exit(-1);
			}

			cont=0;
			while(read(fdr, &c, 1) > 0){
				if(c == argv[i+2][0]){
					cont++;
				}
			}
			write(p[i][1], &cont, sizeof(cont));
			exit(0);
		}		
	}	

	for(j = 0; j < N; j++){
		close(p[j][1]);
	}

	for(i = 0; i < N; i++){
		if(read(p[i][0], &cont, sizeof(cont)) > 0){
			printf("%ld occorrenze del carattere %c nel file %s\n", cont, argv[i+1][0], argv[1]);
		}
	}

	for(i = 0; i < N; i++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(5);
		}
		if((status & 0xFF) != 0){
			printf("figlio con pid %d terminato in modo anomalo\n", pidFiglio);
		}else{
			ritorno = (int)((status >> 8) & 0xFF);
			printf("il figlio con pid %d ha ritornato %d\n", pidFiglio, ritorno);
		}
	}
	exit(0);
}
