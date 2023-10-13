#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctype.h>

#define PERM 0644

typedef int pipe_t[2];

int main(int argc, char **argv){
	int pid;
	int pidFiglio, ritorno, status;
	int i, j;
	int fd;
	int N;
	pipe_t *piped;
	int C;
	int nw, nr;
	char *buf;
	char *file_creato;
	int fcreato;
	int prima_meta, seconda_meta;

	if(argc < 3){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	C = atoi(argv[argc - 1]);

	if((C <= 0) || (C % 2 == 0)){
		printf("errore nell'ultimo parametro: non positivo\n");
		exit(2);
	}

	N = argc - 2;

	buf = (char *)malloc(N*sizeof(char));
	if(buf == NULL){
		printf("errore nella malloc\n");
		exit(5);
	}

	/*allochiamo memoria per la pipe*/
	piped = (pipe_t *)malloc(N*sizeof(pipe_t));
	if(piped == NULL){
		printf("errore nella malloc\n");
		exit(3);
	}

	/*creiamo la pipe*/
	for (i = 0; i < N; i++){
		if(pipe(piped[i]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(4);
		}
	}

	/*creiamo i figli*/
	for(i = 0; i < 2*N; i++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(5);
		}
		if(pid == 0){
			/*codice figlio*/
			/*chiudiamo i lati della pipe che non servono*/
			if(i < N){
				/*primo della coppia*/
				for(j = 0; j < N; j++){
					close(piped[j][0]);
					if(i != j){
						close(piped[j][1]);
					}
				}
				if((fd = open(argv[i+1], O_RDONLY)) < 0){
					printf("errore nella open\n");
					exit(-1);
				}
				/*devo leggere la prima meta' del file*/
				prima_meta = lseek(fd, 0L, 2) / C;
				lseek(fd, 0L, 0);
				for(j = 0; j < prima_meta/2; j++){
					read(fd, buf, C);
					nw = write(piped[i][1], buf, C);
					if(nw != C){
						printf("errore nella scrittura\n");
						exit(-1);
					}
				}

			}else{
				/*secondo della coppia*/
				file_creato = (char *)malloc(strlen(argv[i-N+1]) + 11);
				if(file_creato == NULL){
					printf("errore nella malloc\n");
					exit(-1);
				}
				strcpy(file_creato, argv[i-N+1]);
				strcat(file_creato, ".mescolato");
				fcreato = creat(file_creato, PERM);
				if(fcreato < 0){
					printf("errore nella creat\n");
					exit(-1);
				}

				/*chiudiamo le pipe che non servono*/
				for(j = 0; j < N; j++){
					close(piped[j][1]);
					if(j != i-N){
						close(piped[j][0]);
					}
				}
				/*ogni figlio deve aprire il file associato*/
				fd = open(argv[i-N+1], O_RDONLY);
				if(fd < 0){
					printf("errore in apertura file\n");
					exit(-1);
				}
				seconda_meta = lseek(fd, 0L, 2) / C;
				lseek(fd, (long)seconda_meta/2 * C, 0);
				for(j = 0; j < seconda_meta/2; j++){
					read(fd, buf, C);
					write(fcreato, buf, C);
					nr = read(piped[i-N][0], buf, C);
					if(nr != C){
						printf("errore nella lettura\n");
						exit(-1);
					}
					write(fcreato, buf, C);
				}
			}
			exit(seconda_meta/2);			
		}
	}
	/*codice padre*/
	/*chiudiamo i lati della pipe che non servono*/
	for (i = 0; i < N; i++){
		close(piped[i][1]);
		close(piped[i][0]);
	}
	/*attendiamo la terminazione dei figli*/

	for(i = 0; i < 2*N; i++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(8);
		}
		if((status & 0xFF) != 0){
			printf("il figlio con pid %d ha terminato in modo anomalo\n", pidFiglio);
		}else{
			ritorno = (int)((status >> 8) & 0xFF);
			printf("il figlio con pid %d ha ritornato %d\n", pidFiglio, ritorno);
		}
	}
	exit(0);
}
