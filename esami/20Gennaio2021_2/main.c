#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/stat.h>

typedef int pipe_t[2];

int main(int argc, char **argv){
	
	int pid;
	//int pidFiglio;
	//int ritorno = 0;
	//int status;
	int Q;
	int j, q;
	int fd;
	pipe_t *piped;
	int nr, nw;
	int k = 0;
	char ch;
	long int pos;
	char ok;

	if(argc < 3){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	Q = argc - 1;

	/*alloco memoria per la pipe*/
	piped = (pipe_t *)malloc(Q*sizeof(pipe_t));
	if(piped == NULL){
		printf("errore nella malloc\n");
		exit(2);
	}

	/*creo la pipe*/
	for(q = 0; q < Q; q++){
		if(pipe(piped[q]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(3);
		}
	}

	/*creo i processi figli*/
	for(q = 0; q < Q; q++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(4);
		}
		if(pid == 0){
			/*codice del figlio*/
			/*chiudiamo i lati della pipe*/
			for(j = 0; j < Q; j++){
				if(j != q){
					close(piped[j][0]);
				}
				if(j != (q+1)%Q){
					close(piped[j][1]);
				}
			}
			/*apriamo i file*/
			fd = open(argv[q+1], O_RDONLY);
			if(fd < 0){
				printf("errore in apertura file\n");
				exit(5);
			}

			pos = 0L;

			while(read(fd, &ch, 1)){
				if(pos == (q+k*Q)){
					nr = read(piped[q][0], &ok, sizeof(ok));
					if(nr != sizeof(ok)){
						printf("errore nella read\n");
						exit(-1);
					}
					printf("Figlio con indice %d e pid %d ha letto il carattere %c\n", q, getpid(), ch);

					nw = write(piped[(q+1)%Q][1], &ok, sizeof(ok));
					if(nw != sizeof(ok)){
						printf("errore nella write\n");
						exit(-1);
					}
					k++;
				}
				pos++;
			}
			exit(q);
		}
	}
	/*codice del padre*/
	/*chiudiamo le pipe che non usiamo*/
	for(q = 1; q < Q; q++){
		close(piped[q][0]);
		close(piped[q][1]);
	}

	nw = write(piped[0][1], &ok, sizeof(ok));
	if(nw != sizeof(ok)){
		printf("errore nella write\n");
		exit(5);
	}
	close(piped[0][1]);

	for(q = 0; q < Q; q++){
		/*solito wait dei figli*/
	}
	exit(0);
}
