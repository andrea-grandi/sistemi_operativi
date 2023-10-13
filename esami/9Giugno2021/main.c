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
	int pidFiglio, ritorno, status;
	int N;
	int fcreato;
	int n, j;
	char linea[200];
	int fd;
	pipe_t *piped;
	pipe_t p;
	int pidPrimo;


	if(argc < 3){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	N = argc - 1;

	/*creiamo il file fcreato*/
	fcreato = open("/tmp/AndreaGrandi",  0644);
	if(fcreato < 0){
		printf("errore nella creazione del file\n");
		exit(2);
	}

	/*creaiamo il figlio speciale*/
	if(pipe(p) < 0){
		printf("errore nella creazione del figlio speciale\n");
		exit(3);
	}

	if((pidPrimo = fork()) < 0){
		printf("errore nella prima fork\n");
		exit(4);
	}
	if(pidPrimo == 0){
		/*codice primo figlio*/
		/*chiudiamo i lati della pipe che non servono*/
		close(0);
		printf("sono il primo figlio (speciale) che fa il wc sulle linee del file F1\n");
		if(open(argv[1], O_RDONLY)){
			printf("errore in apertura file\n");
			exit(-1);
		}
		close(1);
		dup(p[1]);
		close(p[0]);
		close(p[1]);
		execlp("wc", "wc", "-l", (char *)0);
		perror("errore nella esecuzione di wc -l\n");
		exit(-1);
	}

	/*codice padre*/
	close(p[1]);
	while(read(p[0], &(linea[j]), 1)){
		j++;
	}

	if(j != 0){
		linea[j-1]='\0';
		X=atoi(linea);
		printf("padre ricevuto valore %d\n", X);
	}else{
		printf("errore nel calcolo del numero di linee\n");
		exit(5);
	}
	close(p[0]);

	/*allochiamo la pipe*/
	piped = (pipe_t *)malloc(N*sizeof(pipe_t));
	if(piped == NULL){
		printf("errore nella malloc\n");
		exit(6);
	}

	for(n = 0; n < N; n++){
		if(pipe(piped[n]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(7);
		}
	}

	for(n = 0; n < N; n++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(8);
		}
		if(pid == 0){
			/*codice figli*/
			/*chiudiamo i lati della pipe*/
			for(j = 0; j < N; j++){
				close(piped[j][0]);
				if(j != n){
					close(piped[j][1]);
				}
			}
			/*apriamo il file associato*/
			fd = open(argv[n+1], O_RDONLY);
			if(fd < 0){
				printf("errore in apertura file\n");
				exit(-1);
			}
			j = 0;
			while(read(fd, &(linea[j]), 1)){
				if(linea[j] == '\n'){
					j++;
					write(piped[n][1], &j, sizeof(j));
					write(piped[n][1], &linea, j);
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
	/*chiudiamo i lati della pipe*/
	for(n = 0; n < N; n++){
		close(piped[n][1]);
	}

	for(n = 1; n <= X; n++){
		for(j = 0; j < N; j++){
			read(piped[j][0], &valore, sizeof(valore));
			read(piped[j][0], linea, valore);
			write(fcreato, linea, valore);
		}
	}

	/*il padre aspetta i figli*/





	exit(0);
}
