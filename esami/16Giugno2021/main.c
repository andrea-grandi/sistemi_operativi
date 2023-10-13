#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#define PERM 0644

typedef int pipe_t[2];
typedef char L[255];

int main(int argc, char **argv){
	int pid;
	int pidFiglio, ritorno, status;
	int i, j, n, k;
	int fd;
	int fc;
	int nr, nw;
	pipe_t *piped;
	int N;
	L *tutteLinee;
	L linea;

	if(argc < 3){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	N = argc - 1;

	/*allochiamo memoria per le pipe*/
	piped = (pipe_t *) malloc(N*sizeof(pipe_t));
	if(piped == NULL){
		printf("errore nella malloc\n");
		exit(2);
	}

	/*creiamo le pipe*/
	for (i = 0; i < N; i++){
		if ((pipe(piped[i])) < 0){
			printf("errore nella creazione della pipe\n");
			exit(3);
		}
	}

	/*creiamo il file fcreato*/
	if((fc = creat("Grandi", PERM)) < 0){
		printf("errore in creazione file\n");
		exit(4);
	}

	if((tutteLinee = (L *)malloc(N*sizeof(L))) == NULL){
		printf("errore nella malloc\n");
		exit(9);
	}

	printf("DEBUG: sono il padre con pid %d e sto per generare %d processi figli\n", getpid(), N);

	/*creazione figli*/
	for(n = 0; n < N; n++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(5);
		}
		if(pid == 0){
			/*codice figlio*/
			/*chiudiamo i lati delle pipe*/
			for(j = 0; j < N; j++){
				if(j != n){
					close(piped[j][0]);
				}
				if(j != (n + 1) % N){
					close(piped[j][1]);
				}
			}

			/*apriamo il file*/
			if((fd = open(argv[n+1], O_RDONLY)) < 0){
				printf("errore nella open\n");
				exit(-1);
			}
			j=0;
			while(read(fd, &(linea[j]), 1)){
				if(linea[j] == '\n'){
					nr = read(piped[n][0], tutteLinee, N*sizeof(L));
					if(nr != N*sizeof(L)){
						printf("errore nella lettura\n");
						exit(-1);
					}
					for(k = 0; k <= j; k++){
						tutteLinee[n][k] = linea[k];
					}
					nw = write(piped[(n+1)%N][1], tutteLinee, N*sizeof(L));
					if(nw != N*sizeof(L)){
						printf("errore nella scrittura\n");
						exit(-1);
					}
					if(n == N-1){
						for(i = 0; i < N; i++){
							for(k = 0; k < 250; k++){
								write(fc, &(tutteLinee[i][k]), 1);
								if(tutteLinee[i][k] == '\n')
									break;
							}
						}
					}
					ritorno = j+1;
					j=0;
				}else{
					j++;
				}
			}
			exit(ritorno);
		}
	}

	/*codice padre*/
	/*chiudiamo i lati della pipe che non ci interessano*/
	for (i = 1; i < N; i++){
		close(piped[i][1]);
		close(piped[i][0]);
	}

	nw = write(piped[0][1], tutteLinee, N*sizeof(L));
	if(nw != sizeof(L)*N){
		printf("errore nella write\n");
		exit(6);
	}
	close(piped[0][1]);

	/*il padre aspetta i figli*/
	for (i = 0; i < N; i++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(7);
		}
		if((status & 0xFF) != 0){
			printf("il figlio con pid %d e' terminato in modo anomalo\n", pidFiglio);
		}else{
			ritorno = (int)((status >> 8) &0xFF);
			printf("il figlio con pid %d ha tornato %d\n", pidFiglio, ritorno);
		}
	}
	exit(0);
}
