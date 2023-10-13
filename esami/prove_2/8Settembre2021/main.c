#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

typedef int pipe_t[2];

int main(int argc, char **argv){
	int pid;
	int pidFiglio;
	int ritorno=0;
	int status;
	int fd;
	int N;
	int fcreato;
	int nr, nw;
	pipe_t *piped;
	char linea[250];
	int n, j;
	int nro;
	int nroLinea;

	if(argc < 2){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	N = argc - 1;

	/*dobbiamo creare il file fcreato*/
	fcreato = creat("grandi.log", 0644);
	if(fcreato < 0){
		printf("errore nella creazione del file\n");
		exit(2);
	}

	/*allochiamo memoria per la pipe*/
	piped = (pipe_t *)malloc(2*N*sizeof(pipe_t));
	if(piped == NULL){
		printf("errore nella malloc di piped\n");
		exit(3);
	}

	/*creiamo la pipe*/
	for(n = 0; n < 2*N; n++){
		if(pipe(piped[n]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(4);
		}
	}

	/*creazione figli*/
	for(n = 0; n < 2*N; n++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(5);
		}
		if(pid == 0){
			/*codice figlio*/
			/*chiudiamo i lati della pipe*/
			for(j = 0; j < 2*N; j++){
				close(piped[j][0]);
				if(j != n){
					close(piped[j][1]);
				}
			}
			nroLinea=0;
			nro=-1;
			if(n % 2 == 0){
				/*processo pari*/
				/*apriamo il file associato*/
				fd = open(argv[n/2+1], O_RDONLY);
				if(fd < 0){
					printf("errore in apertura file\n");
					exit(-1);
				}
				j=0;
				while(read(fd, &(linea[j]), 1)){
					if(linea[j] == '\n'){
						nroLinea++;
						if((nroLinea % 2) == 0){
							nw = write(piped[n][1], &j, sizeof(j));
							if(nw != sizeof(j)){
								printf("errore nella write al padre\n");
								exit(-1);
							}
							if(j > nro){
								nro=j;
							}
						}
						j=0;
					}else{
						j++;
					}
				}				

			}else{
				/*processo dispari*/
				/*apriamo il file associato*/
				fd = open(argv[(n+1)/2], O_RDONLY);
				if(fd < 0){
					printf("errore in apertura file\n");
					exit(-1);
				}
				j=0;
				while(read(fd, &(linea[j]), 1)){
					if(linea[j] == '\n'){
						/*abbiamo letto una linea*/
						nroLinea++;
						if(nroLinea % 2 != 0){
							nw = write(piped[n][1], &j, sizeof(j));
							if(nw != sizeof(j)){
								printf("errore nella write al padre\n");
								exit(-1);
							}
							if(j > nro){
								nro=j;
							}
						}
					}
					j=0;
				}else{
					j++;
				}
			}
			exit(nro);
		}
	}
	/*codice padre*/
	/*chiudiamo i lati delle pipe*/
	for(n = 0; n < 2*N; n++){
		close(piped[n][1]);
	}

	/*il padre legge dalla pipe*/
	for(n = 0; n < N; n++){
		/*deve leggere PER OGNI FILE*/
		nroLinea=1;
	}


	/*il padre aspetta i figli
	for(n = 0; n < 2*N; n++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(6);
		}
		if((status & 0xFF) != 0){
			printf("figlio terminato in modo anomalo\n");
		}else{
			ritorno = (int)((status >> 8) & 0xFF);
			sprintf(linea, "il figlio con pid %d ha ritornato %d\n", pidFiglio, ritorno);
			write(fcreato, linea, sizeof(linea));
		}
	}
	*/
	exit(0);
}
