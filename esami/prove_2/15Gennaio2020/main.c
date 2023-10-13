#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

typedef int pipe_t[2];

int main(int argc, char **argv){
	int pid;
	int pidFiglio;
	int pidNipote;
	char ritorno;
	int status;
	int fd;
	int nr, nw;
	int N;
	int i, j;
	char FOut[10];
	pipe_t *pipe_fn;
	pipe_t *pipe_nf;
	char ok;
	int fcreato;
	char ch;

	if(argc < 3){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	N = argc - 1;

	if(N % 2 != 0){
		printf("errore nel numero di parametri: non pari\n");
		exit(9);
	}

	/*allochiamo memoria per le pipe*/
	pipe_fn = (pipe_t *)malloc((N/2)*sizeof(pipe_t));
	pipe_nf = (pipe_t *)malloc((N/2)*sizeof(pipe_t));
	if((pipe_fn == NULL) || (pipe_nf == NULL)){
		printf("errore nella malloc\n");
		exit(2);
	}

	/*creiamo le pipe*/
	for(i = 0; i < N/2; i++){
		if(pipe(pipe_fn[i]) < 0){
			printf("errore nella creazione della pipe f-n\n");
			exit(3);
		}
		if(pipe(pipe_nf[i]) < 0){
			printf("errore nella creazione della pipe n-f\n");
			exit(4);
		}
	}

	/*generiamo i figli*/
	for(i = 0; i < N/2; i++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(5);
		}
		if(pid == 0){
			/*codice figlio*/
			/*creiamo il file FOut*/
			sprintf(FOut, "merge%d", i);
			fcreato = creat(FOut, 0644);
			if(fcreato < 0){
				printf("errore nella creazione file\n");
				exit(-1);
			}

			/*ora generiamo i nipoti*/
			if((pid = fork()) < 0){
				printf("errore nella fork nipoti\n");
				exit(-1);
			}
			if(pid == 0){
				/*codice nipote*/
				/*chiudiamo i lati della pipe*/
				for(j = 0; j < N/2; j++){
					close(pipe_nf[j][0]);
					close(pipe_fn[j][1]);
					if(j != i){
						close(pipe_fn[j][0]);
						close(pipe_nf[j][1]);
					}
				}
				/*apriamo il file associato*/
				fd = open(argv[N/2+i+1], O_RDONLY);
				if(fd < 0){
					printf("errore in apertura file nipote\n");
					exit(-1);
				}
				
				while(read(fd, &ch, 1)){
					/*ogni volta che leggo un carattere devo aspettare ok da processo figlio e poi scrivo*/
					nr = read(pipe_fn[i][0], &ok, 1);
					if(nr != 1){
						/*siamo arrivati in fondo al file*/
						break;
					}
					write(fcreato, &ch, 1);
					nw = write(pipe_nf[i][1], &ok, 1);
					if(nw != 1){
						printf("errore nella write nipote\n");
						exit(-1);
					}
					ritorno = ch;
				}
				exit(ritorno);
			}
			/*codice figli*/
			/*chiudo i lati della pipe*/
			for(j = 0; j < N/2; j++){
				close(pipe_fn[j][0]);
				close(pipe_nf[j][1]);
				if(i != j){
					close(pipe_fn[j][1]);
					close(pipe_nf[j][0]);
				}
			}
			/*apriamo il file associato*/
			fd = open(argv[i+1], O_RDONLY);
			if(fd < 0){
				printf("errore in apertura file\n");
				exit(-1);
			}

			while(read(fd, &ch, 1)){
				if(i != 0){
					nr = read(pipe_nf[i][0], &ch, 1);
					if(nr != 1){
						break;
					}
				}
				write(fcreato, &ch, 1);
				nw = write(pipe_fn[i][1], &ok, 1);
				if(nw != 1){
					printf("errore nella write figlio\n");
					exit(-1);
				}
				ritorno = ch;
			}
			/*aspettiamo i nipoti*/
			if((pidNipote = wait(&status)) < 0){
				printf("errore nella wait\n");
				exit(-1);
			}
			if((status & 0xFF) != 0){
				printf("nipote terminato in modo anomalo\n");
			}else{
				ritorno = ((status >> 8) & 0xFF);
				printf("nipote con pid %d ha ritornato %c\n", pidNipote, ritorno);
			}
			exit(ritorno);
		}
	}
	/*codice padre*/

	/*chiudo le pipe*/
	for(i = 0; i < N/2; i++){
		close(pipe_fn[i][0]);
		close(pipe_fn[i][1]);
		close(pipe_nf[i][0]);
		close(pipe_nf[i][1]);
	}

	/*il padre aspetta i figli*/
	for(i = 0; i < N/2; i++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(6);
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
