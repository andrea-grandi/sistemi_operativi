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
	int ritorno = 0;
	int status;
	int fd;
	int q, j;
	int Q;
	pipe_t *piped;
	char linea[255];
	char ok;
	int cont;
	int nr, nw;

	/*controllo sul numero di parametri*/
	if(argc < 3){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	Q = argc - 1;

	/*allochiamo memoria per la pipe*/
	piped = (pipe_t *)malloc(Q*sizeof(pipe_t));
	if(piped == NULL){
		printf("errore nella malloc\n");
		exit(2);
	}

	/*creiamo la pipe*/
	for(q = 0; q < Q; q++){
		if(pipe(piped[q]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(3);
		}
	}

	/*creiamo i figli*/
	for(q = 0; q < Q; q++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(4);
		}
		if(pid == 0){
			/*codice figlio*/
			/*chiudiamo i lati della pipe che non servono*/
			for(j = 0; j < Q; j++){
				if(j != q){
					close(piped[j][0]);
				}
				if((q == 0) || (j != q-1)){
					close(piped[j][1]);
				}
			}

			/*apriamo il file associato*/
			fd = open(argv[q+1], O_RDONLY);
			if(fd < 0){
				printf("errore in apertura file\n");
				exit(-1);
			}

			j=0;
			cont=0;
			while(read(fd, &(linea[j]), 1)){
				if(linea[j] == '\n'){
					/*letta una linea*/
					/*aspetto ok da figlio precedente e poi stampo*/
					nr = read(piped[q-1][0], &ok, sizeof(ok));
					if(nr != sizeof(ok)){
						printf("errore nella lettura\n");
						exit(-1);
					}

					linea[j] = '\0';

					printf("Figlio con indice %d e pid %d ha letto %d caratteri numerici nella linea %s\n", q, getpid(), cont, linea);

					/*ora devo mandare avanti ok*/
					nw = write(piped[q][1], &ok, sizeof(ok));
					if(nw != sizeof(ok)){
						printf("errore nella scrittura\n");
						exit(-1);
					}
					
					j=0;
					ritorno = cont;
					cont = 0;
				}
				if(isdigit(linea[j])){
					/*trovato carattere numerico*/
					cont++;
					j++;
				}
			}
			exit(ritorno);
		}
	}

	/*codice padre*/

	/*chiudiamo i lati della pipe che non ci interessano*/
	for(q = 0; q < Q; q++){
		close(piped[q][0]);
		close(piped[q][1]);
	}

	/*il padre manda ok al primo figlio*/
	nw = write(piped[0][1], &ok, sizeof(ok));
	if(nw != sizeof(ok)){
		printf("errore in scrittura del padre\n");
		exit(6);
	}

	close(piped[0][1]);

	/*il padre aspetta i figli*/
	for(q = 0; q < Q; q++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(7);
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
