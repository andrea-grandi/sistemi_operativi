#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

typedef int pipe_t[2];
typedef char A[250];

int main(int argc, char **argv){
	int pid;
	int pidFiglio;
	int ritorno=0;
	int status;
	int fd;
	int N;
	int i, j;
	int fcreato;
	A linea;
	A *array;
	pipe_t *piped;
	int x, y;
	int nw, nr;

	if(argc < 3){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	N = argc - 1;

	/*allochiamo l'array di linee*/
	array = (A *)malloc(N*sizeof(A));
	if(array == NULL){
		printf("errore nella malloc dell'array\n");
		exit(6);
	}

	/*creiamo il file fcreato*/
	fcreato = creat("grandi", 0644);
	if(fcreato < 0){
		printf("errore nella creazione del file fcreato\n");
		exit(2);
	}

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

	/*generazione figli*/
	for(i = 0; i < N; i++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(5);
		}
		if(pid == 0){
			/*codice figlio*/
			/*chiudiamo i lati della pipe*/
			for(j = 0; j < N; j++){
				if(j != i){
					close(piped[j][0]);
				}
				if(j != (i-1)%N){
					close(piped[j][1]);
				}
			}

			/*apriamo il file associato*/
			fd = open(argv[i+1], O_RDONLY);
			if(fd < 0){
				printf("errore nell'apertura del file\n");
				exit(-1);
			}

			j=0;
			while(read(fd, &(linea[j]), 1)){
				if(linea[j] == '\n'){
					/*abbiamo letto una linea*/
					nr = read(piped[i][0], array, N*sizeof(A));
					if(nr != N*sizeof(A)){
						printf("errore nella read\n");
						exit(-1);
					}
					
					for(x = 0; x <= j; x++){
						array[i][x] = linea[x];
					}
					
					printf("linea letta: %s\n", linea);

					nw = write(piped[(i+1)%N][1], array, N*sizeof(A));
					if(nw != N*sizeof(A)){
						printf("errore nella write\n");
						exit(-1);
					}

					/*se il processo e' l'ultimo scrivo array su fcreato*/
					if(i == N-1){
						for(x = 0; x < N; x++){
							for(y = 0; y < 250; y++){
								write(fcreato, &(array[x][y]), 1);
								if(array[x][y] == '\n'){
									break;
								}
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
	/*chiudiamo i lati ddella pipe*/
	for(i = 0; i < N; i++){
		/*il padre deve solo attivare il ciclo*/
		close(piped[i][0]);
		if(i != 0){
			close(piped[i][1]);
		}
	}

	nw = write(piped[0][1], array, N*sizeof(A));
	if(nw != N*sizeof(A)){
		printf("errore nella write del padre\n");
		exit(8);
	}

	close(piped[0][1]);

	/*il padre aspetta i figli*/
	for(i = 0; i < N; i++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(9);
		}
		if((status & 0xFF) != 0){
			printf("figlio terminato in modo anomalo\n");
		}else{
			ritorno = (int)((status >> 8) & 0xFF);
			printf("figlio con pid %d ha ritornato %d\n", pidFiglio, ritorno);
		}
	}
	exit(0);
}
