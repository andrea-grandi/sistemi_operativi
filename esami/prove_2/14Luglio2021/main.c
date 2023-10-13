#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/stat.h>

typedef int pipe_t[2];
typedef char L[250];

int main(int argc, char **argv){
	int pid;
	int pidFiglio;
	int ritorno=0;
	int status;
	int fd;
	int N;
	int n, j;
	L linea;
	L *tutteLinee;
	int nroLinee;
	int nr, nw;
	int fcreato;
	pipe_t *piped;
	int k, x, y;

	if(argc < 4){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	nroLinee = atoi(argv[argc-1]);
	if(nroLinee <= 0){
		printf("errore nell'ultimo parametro\n");
		exit(2);
	}

	N = argc - 2;

	/*creiamo il file fcreato*/
	fcreato = creat("GRANDI", 0644);
	if(fcreato < 0){
		printf("errore nella creazione del file fcreato\n");
		exit(3);
	}

	/*allochiamo l'array di linee*/
	tutteLinee = (L *)malloc(N*sizeof(L));
	if(tutteLinee == NULL){
		printf("errore nella malloc di tutteLinee\n");
		exit(4);
	}

	/*allochiamo memoria per la pipe*/
	piped = (pipe_t *)malloc(N*sizeof(pipe_t));
	if(piped == NULL){
		printf("errore nella malloc di piped\n");
		exit(5);
	}

	/*creiamo la pipe*/
	for(n = 0; n < N; n++){
		if(pipe(piped[n]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(6);
		}
	}

	/*ciclo generazione figli*/
	for(n = 0; n < N; n++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(7);
		}
		if(pid == 0){
			/*codicxe figlio*/
			/*chiudiamo i lati della pipe che non servono*/
			for(j = 0; j < N; j++){
				if(j != n){
					close(piped[j][1]);
				}
				if((n == 0) || (j != (n-1))){
					close(piped[j][0]);
				}
			}

			/*apriamo il file associato*/
			fd = open(argv[n+1], O_RDONLY);
			if(fd < 0){
				printf("errore in apertura file\n");
				exit(-1);
			}

			j=0;
			while(read(fd, &(linea[j]), 1)){
				if(linea[j] == '\n'){
					if(n != 0){
						nr = read(piped[n-1][0], tutteLinee, N*sizeof(L));
						if(nr != N*sizeof(L)){
							printf("errore nella read figlio\n");
							exit(-1);
						}
					}

					/*siamo arrivati infonfo alla linea*/

					for(k = 0; k <= j; k++){
						tutteLinee[n][k] = linea[k];
					}

					/*ora devo inviare la linea (array) in avanti*/
					nw = write(piped[n][1], tutteLinee, N*sizeof(L));
					if(nw != N*sizeof(L)){
						printf("errore nella write figlio\n");
						exit(-1);
					}
					ritorno = j+1;
					j=0;
										
				}else{
					j++;
				}
			}
			//ritorno: lunghezza dell'ultima linea letta
			exit(ritorno);
		}
	}
	/*codice padre*/
	/*chiudiamo i lati della pipe che non servono*/
	for(n = 0; n < N; n++){
		//il padre legge solo dall'ultimo figlio
		close(piped[n][1]);
		if(n != N-1){
			close(piped[n][0]);
		}
	}

	/*leggiamo da pipe l'array*/
	for(n = 1; n <= nroLinee; n++){
		nr = read(piped[n-1][0], tutteLinee, N*sizeof(L));
		if(nr != N*sizeof(L)){
			printf("errore nella read padre\n");
			exit(8);
		}

		for(x = 0; x < N; x++){
			for(y = 0; y< 250; y++){
				write(fcreato, &(tutteLinee[x][y]), 1);
				if(tutteLinee[x][y] == '\n'){
					break;
				}
			}
		}
	}

	/*il padre aspetta i figli*/
	for(n = 0; n < N; n++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(9);
		}
		if((status & 0xFF) != 0){
			printf("figlio terminato in modo anomalo\n");
		}else{
			ritorno = (int)((status >> 8)& 0xFF);
			printf("figlio con pid %d ha ritornato %d\n", pidFiglio, ritorno);
		}
	}

	exit(0);
}


	
