#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>

typedef int pipe_t[2];

int main(int argc, char **argv){
	int pid;
	int pidFiglio;
	int ritorno = 0;
	int status;
	int fd;
	int Q;
	int q, j, i;
	int L;
	//int nr, nw;
	char ch;
	pipe_t *pipe_fp;
	pipe_t *pipe_pf;
	int occ;
	char ok;

	if(argc < 5){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	//printf("%c\n", argv[2][0]);

	L = atoi(argv[2]);
	if(L <= 0){
		printf("errore numero non positivo\n");
		exit(2);
	}

	for(q = 3; q < argc; q++){
		if(strlen(argv[q]) != 1){
			printf("errore nel parametro singolo carattere\n");
			exit(7);
		}
	}

	Q = argc - 3;

	/*allochiamo memoria per la pipe*/
	pipe_fp = (pipe_t *)malloc(Q*sizeof(pipe_t));
	pipe_pf = (pipe_t *)malloc(Q*sizeof(pipe_t));
	if((pipe_fp == NULL) || (pipe_pf == NULL)){
		printf("errore nella malloc\n");
		exit(3);
	}

	/*creiamo la pipe*/
	for(q = 0; q < Q; q++){
		if(pipe(pipe_pf[q]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(4);
		}
		if(pipe(pipe_fp[q]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(8);
		}
	}

	/*generazione figli*/
	for(q = 0; q < Q; q++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(5);
		}
		if(pid == 0){
			/*codice figlio*/
			/*chiudo lati della pipe*/
			for(j = 0; j < Q; j++){
				close(pipe_pf[j][1]);
				close(pipe_fp[j][0]);
				if(j != q){
					close(pipe_pf[j][0]);
					close(pipe_fp[j][1]);
				}
			}

			/*apriamo il file associato*/
			fd = open(argv[1], O_RDONLY);
			if(fd < 0){
				printf("errore in apertura file\n");
				exit(-1);
			}

			occ = 0;
			while(read(fd, &ch, 1)){
				if(ch == argv[3+q][0]){
					/*trovata occorrenza*/
					occ++;
				}else{
					if(ch == '\n'){
						read(pipe_pf[q][0], &ok, 1);
						printf("numero di occorrenze del carattere %c e': %d\n", argv[q+3][0], occ);
						write(pipe_fp[q][1], &ok, 1);
						ritorno = occ;
						occ=0;
					}
				}
			}
			exit(ritorno);
		}
	}
	/*codice padre*/
	/*chiudiamo i lati della pipe*/
	for(q = 0; q < Q; q++){
		close(pipe_pf[q][0]);
		close(pipe_fp[q][1]);
	}

	/*il padre deve stampare la linea corrente*/
	for(i = 1; i <= L; i++){
		printf("linea: %d\n", i);
		for(q = 0; q < Q; q++){
			write(pipe_pf[q][1], &ok, 1);
			read(pipe_fp[q][0], &ok, 1);
		}
	}

	/*il padre aspetta i figli*/
	for(q = 0; q < Q; q++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(6);
		}
		if((status & 0xFF) != 0){
			printf("figlio ha terminato in modo anomalo\n");
		}else{
			ritorno = (int)((status >> 8) & 0xFF);
			printf("figlio con pid %d ha ritornato %d\n", pidFiglio, ritorno);
		}
	}
	exit(0);
}

