#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctype.h>

typedef int pipe_t[2];

int main(int argc, char **argv){
	
	int pid;
	int pidFiglio, ritorno, status;
	int fd;
	int Q;
	int q, j, i;
	pipe_t *piped;
	char ch;
	long int cur;
	int k = 0;
	int nr, nw;
	char ok;

	if(argc < 3){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	Q = argc - 1;

	/*allochiamo la memoria per le pipe*/
	piped = (pipe_t *)malloc(Q*sizeof(pipe_t));
	if(piped == NULL){
		printf("errore nella malloc\n");
		exit(2);
	}

	/*creiamo le pipe*/
	for (i = 0; i < Q; i++){
		if(pipe(piped[i]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(3);
		}
	}

	/*creiamo i figli*/
	for (q = 0; q < Q; q++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(4);
		}
		if (pid == 0){
			/*codice del figlio*/
			/*chiudiamo i lati della pipe che non servono*/
			for (j = 0; j < Q; j++){
				if(j != q){
					close(piped[j][0]);
				}
				if(j != (q+1)%Q){ //tengo il collegamento solo con la pipe avanti
					close(piped[j][1]);
				}
			}

			/*ora apro il file associato*/
			if ((fd = open(argv[q+1], O_RDONLY)) < 0){
				printf("errore nella open\n");
				exit(Q);
			}
			cur = 0L;
			while(read(fd, &ch, 1)){
				if(cur == (q + k * Q)){
					nr = read(piped[q][0], &ok, sizeof(char));
					if(nr != sizeof(char)){
						printf("errore nella read\n");
						exit(Q+1);
					}
					printf("il figlio di indice %d e pid %d ha letto il carattere %c\n", q, getpid(), ch);
					nw = write(piped[(q+1)%Q][1], &ok, sizeof(char));
					if(nw != sizeof(char)){
						printf("errore nella write\n");
						exit(Q+2);
					}
					k++;
				}
				cur++;
			}
			exit(q);
		}
	}
	/*codice del padre*/
	/*chiudiamo i lati della pipe che non servono*/
	for (i = 1; i < Q; i++){
		close(piped[i][0]);
		close(piped[i][1]);
	}

	if((nw = write(piped[0][1], &ok, sizeof(char))) != sizeof(char)){
		printf("errore nella write\n");
		exit(7);
	}
	close(piped[0][1]);

	/*il padre aspetta i figli*/
	for (i = 0; i < Q; i++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(8);
		}
		if((status & 0xFF) != 0){
			printf("il figlio con pid %d e' terminato in modo anomali\n", pidFiglio);
		}else{
			ritorno = (int)((status >> 8) & 0xFF);
			printf("il figlio con pid %d ha ritornato %d\n", pidFiglio, ritorno);
		}
	}
	exit(0);
}
