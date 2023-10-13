#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/wait.h>

typedef int pipe_t[2];

int main(int argc, char **argv){
	int pid;
	int pidFiglio;
	int ritorno = 0;
	int status;
	int i, j;
	int N;
	int fd;
	int nr, nw;
	pipe_t pipe_fn;
	pipe_t pipe_nf;
	int FOut;
	char ok;

	if((argc < 3) && ((argc % 2) == 1)){
		printf("errore nel numero di parametri, parametri non pari\n");
		exit(1);
	}

	N = argc - 1;

	/*

	//allochiamo memoria per le pipe
	pipe_fn = (pipe_t *)malloc((N/2)*sizeof(pipe_t));
	pipe_nf = (pipe_t *)malloc((N/2)*sizeof(pipe_t));
	if((pipe_nf == NULL) || (pipe_fn == NULL)){
		printf("errore nella malloc\n");
		exit(2);
	}

	//creiamo le pipe
	for(i = 0; i < N/2; i++){
		if(pipe(pipe_fn[i]) < 0){
			printf("errore nella creazione della pipe figlio-nipote\n");
			exit(3);
		}
		if(pipe(pipe_nf[i]) < 0){
			printf("errore nella creazione pipe nipote-figlio\n");
			exit(4);
		}
	}

	*/

	/*creiamo le pipe*/
	if(pipe(pipe_fn) < 0){
		printf("errore nella creazione della pipe figlio-nipote\n");
		exit(2);
	}
	if(pipe(pipe_nf) < 0){
		printf("errore nella creazione pipe nipote-figlio\n");
		exit(3);
	}

	/*creazione figli*/
	for(i = 0; i < N/2; i++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(5);
		}
		if(pid == 0){
			/*codice figlio*/
			/*creo file FOut*/
			sprintf(FOut, "merge%d", i);
			if((fdout = creat(FOut, 0644)) < 0){
				printf("errore nella creazione file\n");
				exit(-1);
			}

			/*creazione nipote*/
			if((pid = fork()) < 0){
				printf("errore nella fork nipote\n");
				exit(-1);
			}
			if(pid == 0){
				/*codice nipote*/
				/*chiudo i lati della pipe*/
				close(pipe_nf[0]);
				close(pipe_fn[1]);

				/*apriamo il file associato*/
				fd = open(argv[N/2+i+1], O_RDONLY);
				if(fd < 0){
					printf("errore in apertura file\n");
					exit(-1);
				}

				while(read(fd, &ch, 1)){
					read(pipe_fn[0], &ok, 1);
					write(fdout, &ch, 1);
					write(pipe_nf[1], &ok, sizeof(ok));
				}
				ritorno = ch;
				exit(ritorno);
			}else{
				/*codice figlio*/
				/*chiudiamo le pipe*/
				close(pipe_nf[1]);
				close(pipe_fn[0]);

				fd = open(argv[i+1], O_RDONLY);
				if(fd < 0){
					printf("errore in apertura file\n");
					exit(-1);
				}

				while(read(fd, &ch, 1)){
					if(primo == 1){
						primo = 0;
					}else{
						read(pipe_nf[0], &ok, 1);
					}
					write(fdout, &ch, 1);
					write(pipe_fn[1], &ok, sizeof(ok));
				}

				/*il figlio aspetta i nipoti*/
				exit(ritorno);
			}
		}
	}

	/*codice padre*/
	/*il padre aspetta semplicemente i figli*/
	exit(0);
}
