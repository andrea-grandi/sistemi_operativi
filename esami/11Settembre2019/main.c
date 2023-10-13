#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#define PERM 0644

typedef int pipe_t[2];

int main(int argc, char **argv){
	
	int pid;
	int pidFiglio, ritorno, status;
	int i, j;
	int N;
	pipe_t *piped;
	int fd;
	char ch, ch1;
	int c_letti;
	char *Fcreato;
	int fdw;
	int nr, nw;

	if (argc < 3){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	N = argc - 1;

	/*allochiamo la memoria per la pipe*/
	piped = (pipe_t *)malloc(N/2*sizeof(pipe_t));
	if(piped == NULL){
		printf("errore nella malloc\n");
		exit(2);
	}

	/*creiamo la pipe*/
	for (i = 0; i < N/2; i++){
		if (pipe(piped[i]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(3);
		}
	}

	/*creazione figli*/
	for(i = 0; i < N; i++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(4);
		}
		if (pid == 0){
			/*codice figlio*/
			fd = open(argv[i+1], O_RDONLY);
			if(fd < 0){
				printf("errore in apertura file\n");
				exit(-1);
			}
			c_letti = 0;
			if(i % 2 == 0){
				/*processo pari*/
				/*chiudiamo i lati della pipe che non servono*/
				for(j = 0; j < N; j++){
					close(piped[j][0]);
					if(j != i /2){
						close(piped[j][1]);
					}
				}

				/*leggiamo i caratteri dal file*/
				while(read(fd, &ch, 1)){
					c_letti++;
					nw = write(piped[i/2][1], &ch, sizeof(ch));
					if(nw != sizeof(ch)){
						printf("errore nella write\n");
						exit(-1);
					}
				}
			}
			else{
				/*processo dispari*/
				/*chiudiamo i lati della pipe*/
				for(j = 0; j < N; j++){
					close(piped[j][0]);
					if(j != i/2){
						close(piped[j][1]);
					}
				}
				/*creiamo il file*/
				Fcreato = (char *)malloc(strlen(argv[i+1]) + 10);
				if(Fcreato == NULL){
					printf("errore nella malloc\n");
					exit(-1);
				}

				strcpy(Fcreato, argv[i+1]);
				strcat(Fcreato, ".MAGGIORE");
				
				fdw = creat(Fcreato, PERM);
				if(fdw < 0){
					printf("errore nella creazione file\n");
					exit(-1);
				}

				/*leggo dal file*/
				while(read(fd, &ch, 1)){
					c_letti++;
					/*leggo il carattere del processo pari*/
					nr = read(piped[i/2][0], &ch1, sizeof(ch1));
					if(nr != sizeof(ch1)){
						printf("errore nella lettura\n");
						exit(-1);
					}
					if(ch > ch1){
						write(fdw, &ch, 1);
					}else{
						ch = ch1;
					}
				}

			}
			exit(c_letti);
		}
	}
	/*codice padre*/
	/*chiudiamo i lati della pipe che non servono*/
	for(i = 0; i < N; i++){
		close(piped[i][0]);
		close(piped[i][1]);
	}

	/*il padre deve leggere dalla pipe il numero di caratteri letti*/
	/*wit terminazione figli*/
	for(i = 0; i < N; i++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(5);
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
