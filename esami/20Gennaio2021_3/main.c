#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/stat.h>

typedef int pipe_t[2];

int main(int argc, char **argv){
	int pid;
	int pidFiglio;
	int ritorno = 0;
	int status;
	int q, j;
	int Q;
	char ch;
	int fd;
	int nr, nw;
	pipe_t *piped;
	int k = 0;
	char ok;

	if(argc < 3){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	Q = argc - 1;

	/*allochiamo memoria per le pipe*/
	piped = (pipe_t *)malloc(Q*sizeof(pipe_t));
	if(piped == NULL){
		printf("errore nella malloc\n");
		exit(2);
	}

	/*creiamo le pipe*/
	for(q = 0; q < Q; q++){
		if(pipe(piped[q]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(3);
		}
	}

	/*creazione figli*/
	for(q = 0; q < Q; q++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(4);
		}
		if(pid == 0){
			/*codice figlio*/
			/*chiudiamo i lati della pipe che non ci interessano*/
			for(j = 0; j < Q; j++){
				if(j != q){
					close(piped[j][0]);
				}
				if(j != (q+1)%Q){
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
			while(read(fd, &ch, 1)){
				if(j == (q+k*Q)){
					/*trovato carattere*/
					nr = read(piped[q-1][0], &ok, sizeof(ok));
					if(nr != sizeof(ok)){
						printf("errore nella read\n");
						exit(-1);
					}

					printf("Figlio con indice %d e pid %d ha letto il caratter %c\n", q, getpid(), ch);

					nw = write(piped[q][1], &ok, sizeof(ok));
					if(nw != sizeof(ok)){
						printf("errore nella write\n");
						exit(-1);
					}
					k++;
				}
				j++;
			}
			exit(q);
		}
	}
	/*codice padre*/
	/*chiudiamo i lati della pipe*/
	for(q = 0; q < Q; q++){
		close(piped[q][0]);
		close(piped[q][1]);
	}

	nw = write(piped[0][1], &ok, sizeof(ok));
	if(nw != sizeof(ok)){
		printf("errore nella write del padre\n");
		exit(7);
	}

	close(piped[0][1]);

	/*il padre aspetta i figli*/
	for(q = 0; q < Q; q++){
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
