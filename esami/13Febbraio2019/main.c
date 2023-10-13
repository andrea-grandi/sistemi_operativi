#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctype.h>

typedef int pipe_t[2];

typedef struct{
	long int MAX_LINEE;
	int INDEX;
}s;

int main(int argc, char **argv){
	int *pid;
	int pidFiglio, ritorno, status;
	int fd;
	int M;
	pipe_t *piped;
	long int lung;
	int i, j;
	s letto;
	int nr, nw;
	char ch;


	if(argc < 2){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	M = argc - 1;

	/*  M --> numero di file passati  */

	/*alloco memoria per la pipe*/
	piped = (pipe_t *)malloc(M*sizeof(pipe_t));
	if(piped == NULL){
		printf("errore nella malloc\n");
		exit(2);
	}

	/*creo la pipe*/
	for(i = 0; i < M; i++){
		if(pipe(piped[i]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(3);
		}
	}

	/*alloco i pid*/

	if((pid = (int *)malloc(M*sizeof(int))) == NULL){
		printf("errore nella malloc\n");
		exit(4);
	}

	/*genero i figli*/

	for (i = 0; i < M; i++){
		if((pid[i] = fork()) < 0){
			printf("errore nella fork\n");
			exit(5);
		}
		if(pid[i] == 0){
			/*codice figlio*/
			/*chiudo i lati della pipe che non servono*/
			for (j = 0; j < M; j++){
				if(i != j){
					close(piped[j][1]);
				}
				if((i == 0) || (j != i-1)){
					close(piped[j][0]);
				}
			}

			/*apriamo il file*/
			if((fd = open(argv[i+1], O_RDONLY)) < 0){
				printf("errore in apertura file\n");
				exit(-1);
			}

			lung = 0L;

			while(read(fd, &ch, 1) != 0){
				if(ch == '\n'){
					lung++;
				}
			}

			if(i == 0){
				letto.INDEX = 0;
				letto.MAX_LINEE = lung;
			}else{
				nr = read(piped[i-1][0], &letto, sizeof(s));
				if(nr != sizeof(s)){
					printf("errore nella lettura\n");
					exit(-1);
				}

				if(letto.MAX_LINEE < lung){
					letto.INDEX = i;
					letto.MAX_LINEE = lung;
				}
			}

			nw = write(piped[i][1], &letto, sizeof(s));
			if(nw != sizeof(s)){
				printf("errore nella scrittura\n");
				exit(-1);
			}
			exit(i);
		}
	}
	/*codice padre*/
	/*chiudiamo i lati della pipe che non servono*/
	for(i = 0; i < M; i++){
		close(piped[i][1]);
		if(i != M-1){
			close(piped[i][0]);
		}
	}

	/*il padre deve leggere la struct dall'ultimo figlio*/
	nr = read(piped[M-1][0], &letto, sizeof(s));
	if(nr != sizeof(s)){
		printf("errore nella lettura\n");
		exit(6);
	}
	printf("il figlio di indice %d e pid %d ha trovato il numero massimo di linee %ld nel file %s\n", letto.INDEX, pid[letto.INDEX], letto.MAX_LINEE, argv[letto.INDEX + 1]);

	/*il padre aspetta i figli*/
	for(i = 0; i < M; i++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(10);
		}
		if((status & 0xFF) != 0){
			printf("il figlio con pidd %d ha terminato in modo anomalo\n", pidFiglio);
		}
		else{
			ritorno = (int)((status >> 8) & 0xFF);
			printf("il figlio con pid %d ha ritornato %d\n", pidFiglio, ritorno);
		}
	}
	exit(0);
}
