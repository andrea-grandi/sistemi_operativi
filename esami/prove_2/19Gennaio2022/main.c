#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>

typedef int pipe_t[2];

int main(int argc, char **argv){
	int pid;
	int pidFiglio;
	int i, j;
	int N;
	int fd;
	int nr, nw;
	int C;
	pipe_t *piped;
	char *buf;
	int fcreato;
	char *F;
	int nro;

	if(argc < 3){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	C = atoi(argv[argc - 1]);
	if((C <= 0) || (C % 2) == 0){
		printf("errore nell'ultimo parametro\n");
		exit(2);
	}

	N = argc - 2;

	/*allochiamo memoria per la pipe*/
	piped = (pipe_t *)malloc(N*sizeof(pipe_t));
	if(piped == NULL){
		printf("errore nella malloc\n");
		exit(3);
	}

	/*creiamo la pipe*/
	for(i = 0; i < N; i++){
		if(pipe(piped[i]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(4);
		}
	}

	/*allochiamo memoria per il buffer*/
	buf = (char *)malloc((2*N)*sizeof(char));
	if(buf == NULL){
		printf("errore nella malloc\n");
		exit(5);
	}

	/*ciclo creazione figli*/
	for(i = 0; i < 2*N; i++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(6);
		}
		if(pid == 0){
			/*codice figlio*/
			if(i < N){
				/*primi figli*/
				/*chiudiamo i lati della pipe*/
				for(j = 0; j < N; j++){
					close(piped[j][0]);
					if(i != j){
						close(piped[j][1]);
					}
				}
				/*apriamo il file associato*/
				fd = open(argv[i+1], O_RDONLY);
				if(fd < 0){
					printf("errore in apertura file\n");
					exit(-1);
				}

				nro = lseek(fd, 0L, 2) / C;
				lseek(fd, 0L, 0);
				for(j = 0; j < nro/2; j++){
					read(fd, buff, C);
					nw = write(piped[i][1], buff, C);
					if(nw != C){
						printf("errore nella write\n");
						exit(-1);
					}
				}
			}else{
				/*secondi figli*/
				F = (char *)malloc(strlen(argv[i-N+1]) + 11);
				if(F == NULL){
					printf("errore nella malloc\n");
					exit(-1);
				}

				strcpy(F, argv[i-N+1]);
				strcat(F, ".mescolato");
				fcreato = creat(F, 0644);
				if(fcreato < 0){
					printf("errore nella creazione file\n");
					exit(-1);
				}
				/*chiudiamo i lati della pipe*/
				for(j = 0; j < N; j++){
					close(piped[j][1]);
					if(j != i-N){
						close(piped[j][0]);
					}
				}

				fd = open(argv[i-N+1], O_RDONLY);
				if(fd < 0){
					printf("errore in apertura file\n");
					exit(-1);
				}

				nro = lseek(fd, 0L, 2) / C;
				lseek(fd, (long)nro/2*C, 0);
				for(j = 0; j < nro/2; j++){
					read(fd, buff, C);
					write(fcreato, buff, C);
					nr = read(piped[i-N][0], buff, C);
					if(nr != C){
						printf("errore nella read\n");
						exit(-1);
					}
					write(fcreato, buff, C);
				}
			}
			exit(nro/2);
		}
	}
	/*codice padre*/
	/*chiudiamo i lati della pipe che non usiamo*/
	for(i = 0; i < N; i++){
	}

	/*il padre aspetta i figli*/



	exit(0);
}
