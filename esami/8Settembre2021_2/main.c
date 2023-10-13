#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef int pipe_t[2];

int main(int argc, char **argv){
	int pid;
	int pidFiglio, ritorno, status;
	int N;
	int fd; 
	int nr, nw;
	int i, j, k, n;
	int fcreato;
	char linea[255];
	int nro;
	int nroLinea;

	if(argc < 2){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	N = argc - 1;
	
	/*creo il file fcreato*/
	fcreato = creat("grandi.log", 0644);
        if(fcreato < 0){
		printf("errore nella creazione file\n");
		exit(2);
	}

	/*alloco memoria per la pipe*/
	piped = (pipe_t *)malloc((2*N)*sizeof(pipe_t));
	if(piped == NULL){
		printf("errore nella malloc\n");
		exit(3);
	}

	/*creo la pipe*/
	for(i = 0; i < 2*N; i++){
		if(pipe(piped[i]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(4);
		}
	}

	/*creo i figli*/
	for(n = 0; n < 2*N; n++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(5);
		}
		if(pid == 0){
			/*codice figlio*/

			for(j = 0; j < 2*N; j++){
				close(piped[j][0]);
				if(j != n){
					close(piped[j][1]);
				}
			}

			if(n%2 == 0){
				/*processo pari*/

				/*apriamo il file*/
				fd = open(argv[n/2+1], O_RDONLY);
				if(fd < 0){
					printf("errore nell'apertura file\n");
					exit(-1);
				}
				j = 0;
				while(read(fd, &(linea[j]), 1)){
					/*leggo le linee del file*/
					if(linea[j] == '\n'){
						j++;
						nroLinea++;
						write(piped[n][1], &j, sizeof(j));
						if(j > nro){
							nro = j;
						}
						j=0;
					}else{
						j++;
					}
				}
			}else{
				/*processo dispari*/
				
				/*apro il file associato*/
				fd = open(argv[(n+1)/2], O_RDONLY);
				if(fd < 0){
					printf("errore nell'apertura file\n");
					exit(-1);
				}

				j = 0;
				while(read(fd, &(linea[j]), 1)){
					if(linea[j] == '\n'){
						j++;
						nroLinea++;
						write(piped[n][1], &j, sizeof(j));
						if(j > nro){
							nro = j;
						}
						j=0;
					}else{
						j++;
					}
				}
			}
			exit(nro);
		}
	}
	/*codice padre*/

		


	exit(0);
}
