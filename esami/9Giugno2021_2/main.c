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
	int pid_s;
	int pidFiglio;
	int ritorno = 0;
	int status;
	int N;
	int X;
	int n, j;
	char linea[200];
	pipe_t *piped;
	pipe_t p;
	int fd;
	int fcreato;
	int valore;
	int nr, nw;

	if(argc < 3){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	N = argc - 1;

	/*creiamo il file*/
	fcreato = creat("/tmp/andreagrandi", 0644);
	if(fcreato < 0){
		printf("errore nella creazione file\n");
		exit(2);
	}

	if(pipe(p) < 0){
		printf("errore creazione pipe primo figlio\n");
		exit(4);
	}

	/*creiamo un figlio speciale*/
	if((pid_s = fork()) < 0){
		printf("errore nella fork primo figlio\n");
		exit(3);
	}
	if(pid_s == 0){
		/*codice figlio speciale*/
		close(0);
		fd = open(argv[n+1], O_RDONLY);
		if(fd < 0){
			printf("errore in apertura file\n");
			exit(-1);
		}
		close(1);
		dup(p[1]);
		close(p[0]);
		close(p[1]);
		execlp("wc", "wc", "-l", (char *)0);
		perror("errore nella exec\n");
		exit(-1);
	}
	close(p[1]);

	j=0;
	while(read(p[0], &(linea[j]), 1)){
		j++;
	}
	if(j != 0){
		linea[j-1] = '\0';
		X = atoi(linea);
		printf("padre ha ricevuto valore %d\n", X);
	}else{
		printf("errore nel calcolo numero di linee da parte del figlio\n");
		exit(5);
	}

	close(p[0]);

	/*alochiamo la pipe per i figli normali*/
	piped = (pipe_t *)malloc(N*sizeof(pipe_t));
	if(piped == NULL){
		printf("errore nella malloc\n");
		exit(6);
	}

	/*creiamo la pipe*/
	for(n = 0; n < N; n++){
		if(pipe(piped[n]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(8);
		}
	}

	/*genero i figli*/
	for(n = 0; n < N; n++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(7);
		}
		if(pid == 0){
			/*codice figli*/
			/*chiudo i lati della pipe*/
			for(j = 0; j < N; j++){
				close(piped[j][0]);
				if(j != n){
					close(piped[j][1]);
				}
			}

			/*apro il file associato*/
			fd = open(argv[n+1], O_RDONLY);
			if(fd < 0){
				printf("errore nella open\n");
				exit(-1);
			}

			j=0;
			while(read(fd, &(linea[j]), 1)){
				if(linea[j] == '\n'){
					j++;
					nw = write(piped[n][1], &j, sizeof(j));
					if(nw != sizeof(j)){
						printf("errore nella write\n");
						exit(-1);
					}
					nw = write(piped[n][0], &(linea[j]), j);
					if(nw != j){
						printf("errore nella write\n");
						exit(-1);
					}
					ritorno = j;
					j = 0;
				}else{
					j++;
				}
			}
			exit(ritorno);
		}
	}
	/*codice padre*/
	/*chiudiamo i lati della pipe che non servono*/
	for(n = 0; n < N; n++){
		close(piped[n][1]);
	}

	for(j = 1; j <= X; j++){
		for(n = 0; n < N; n++){
			read(piped[n][0], &valore, sizeof(valore));
			read(piped[n][0], linea, sizeof(linea));
			write(fcreato, linea, valore);
		}
	}

	/*solita wait*/




	exit(0);
}
