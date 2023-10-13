#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>

typedef int pipe_t[2];
typedef char L[250];

int main(int argc, char **argv){
	int pid;
	int pidFiglio, ritorno, status;
	int fd;
	int N;
	int nr, nw;
	int i, j;
	pipe_t *piped;
	int nroLinee;
	L linea;
	L *array;
	int fcreato;
	int k, n;

	if(argc < 4){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	nroLinee = atoi(argv[argc - 1]);
	if(nroLinee <= 0){
		printf("errore nell'ultimo parametro\n");
		exit(2);
	}

	N = argc - 2;

	/*creiamo il file fcreato*/
	fcreato = creat("grandi", 0644);
	if(fcreato < 0){
		printf("errore nella creazione file\n");
		exit(3);
	}

	/*allochiamo memoria per l'array di linee*/
	array = (L *)malloc(N*sizeof(L));
	if(array == NULL){
		printf("errore nella malloc\n");
		exit(4);
	}

	/*allochiamo memoria per la pipe*/
	piped = (pipe_t *)malloc(N*sizeof(pipe_t));
	if(piped == NULL){
		printf("errore nella malloc\n");
		exit(5);
	}

	/*creiamo la pipe*/
	for(i = 0; i < N; i++){
		if(pipe(piped[i]) < 0){
			printf("errore nella creazione pipe\n");
			exit(6);
		}
	}

	/*creiamo i figli*/
	for(i = 0; i < N; i++){
		if((pid = fork()) < 0){
			printf("errore nella creazione figlio\n");
			exit(7);
		}
		if(pid == 0){
			/*codice figlio*/
			/*chiudo i lati della pipe*/
			for(j = 0 ; j < N; j++){
				if(j != i){
					close(piped[j][1]);
				}
				if(j != (i+1)%N){
					close(piped[j][0]);
				}
			}

			/*apriamo il file associato*/
			fd = open(argv[i+1], O_RDONLY);
			if(fd < 0){
				printf("errore in apertura file\n");
				exit(-1);
			}
			/*ora il figlio deve leggere la linea dal file*/
			j = 0;
			while(read(fd, &(linea[j]), 1)){
				if(linea[j] == '\n'){
					if(i != 0){
						/*aspettiamo l'array dal figlio precedente*/
						nr = read(piped[i-1][0], array, N*sizeof(L));
						if(nr != N*sizeof(L)){
							printf("errore nella lettura dell'array\n");
							exit(-1);
						}
					}

					for(k = 0; k <= j; k++){
						array[i][k] = linea[k];
					}

					nw = write(piped[i][1], array, N*sizeof(L));
					if(nw != N*sizeof(L)){
						printf("errore nella scrittura su array\n");
						exit(-1);
					}
					ritorno = j+1;
					j =0;
				}else{
					j++;
				}
			}
			exit(ritorno);
		}
	}
	/*codice padre*/
	/*chiudiamo i lati della pipe*/
	for(i = 0; i < N; i++){
		close(piped[i][1]);
		if(i != N-1){
			close(piped[i][0]);
		}
	}

	/*il padre deve leggere tutti gli array di linee inviati dall'ultimo figlio*/
	for(j = 0; j <= nroLinee; j++){
		nr = read(piped[i-1][0], array, N*sizeof(L));
		if(nr != N*sizeof(L)){
			printf("errore nella lettura da array\n");
			exit(8);
		}

		for(n = 0; n < N; n++){
			for(k = 0; k < 250; k++){
				write(fcreato, &(array[n][k]), 1);
				if(array[n][k] == '\n'){
					break;
				}
			}
		}
	}

	/*il padre aspetta i figli*/
	for(i = 0; i < N; i ++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(9);
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
