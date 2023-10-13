#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

typedef int pipe_t[2];

int main(int argc, char **argv){

	int pid;
	int pidFiglio, ritorno, status;
	int N;
	int i, j;
	int fd;
	pipe_t *pipe_fp;
	pipe_t *pipe_np;
	long int t;
	char ch;
	int nr, nw;
	int pidNipote;

	if(argc < 3){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	N = argc - 1;

	/*allochiamo memoria per le pipe*/
		
	pipe_fp = (pipe_t *)malloc(N*sizeof(pipe_t));
	pipe_np = (pipe_t *)malloc(N*sizeof(pipe_t));
	if((pipe_fp == NULL) || (pipe_np == NULL)){
		printf("errore nella malloc delle pipe\n");
		exit(2);
	}

	/*creiamo le pipe*/
	for (i = 0; i < N; i++){
		if(pipe(pipe_fp[i]) < 0){
			printf("errore nella creazione pipe figlio-nipote\n");
			exit(3);
		}
		if(pipe(pipe_np[i]) < 0){
			printf("errore nella creazione pipe nipote-figlio\n");
			exit(4);
		}
	}

	/*creazione figli*/
	for(i = 0; i < N; i++){
		if((pid = fork()) < 0){
			printf("errore nella fork figlio\n");
			exit(5);
		}
		if(pid == 0){
			/*codice del figlio*/
			/*chiudiamo i lati delle pipe*/
			for(j = 0; j < N; j++){
				close(pipe_fp[j][0]);
				if(i != j){
					close(pipe_fp[j][1]);
				}
			}
			if((pid = fork()) < 0){
				printf("errore nella fork nipote\n");
				exit(-1);
			}
			if(pid == 0){
				/*codice nipote*/
				/*chiudo i lati della pipe*/
				for(j = 0; j < N; j++){
					close(pipe_np[j][0]);
					if(j != i){
						close(pipe_np[j][1]);
					}
				}
				/*apro il file corrispondente*/
				fd = open(argv[i+1], O_RDWR);
				if(fd < 0){
					printf("errore in apertura file nipote\n");
					exit(-1);
				}
				t=0;
				while(read(fd, &ch, 1)){
					/*leggo un carattere alla volta*/
					if(islower(ch)){
						/*trovata occorrenza carattere*/
						t++;
						/*ora devo trasformare il carattere trovato in maiuscolo*/
						lseek(fd, 0L, SEEK_CUR);
						ch = ch - 32;
						write(fd, &ch, 1);
					}
				}
				nw = write(pipe_np[i][1], &t, sizeof(t));
				if(nw != sizeof(t)){
					printf("errore nella write nipote padre\n");
					exit(-1);
				}
				ritorno = t/256;
				exit(ritorno);
			}
			/*codice figlio*/
			for(j = 0; j < N; j++){
				close(pipe_np[j][0]);
				close(pipe_np[j][1]);
			}
				
			/*apro il file corrispondente*/
			fd = open(argv[i+1], O_RDWR);
			if(fd < 0){
				printf("errore in apertura file figlio\n");
				exit(-1);
			}
		
			while(read(fd, &ch, 1)){
				if(isdigit(ch)){
					t++;
					lseek(fd, 0L, SEEK_CUR);
					ch = ' ';
					write(fd, &ch, 1);
				}
			}
			
			nw = write(pipe_fp[i][1], &t, sizeof(t));
			if(nw != sizeof(t)){
				printf("errore nella write figlio padre\n");
				exit(-1);
			}

			/*il figlio aspetta il nipote*/
			for(j = 0; j < N; j++){
				if((pidNipote = wait(&status)) < 0){
				       printf("errore nella wait\n");
			               exit(-1);
				}
		 		if((status & 0xFF) != 0){
					printf("il nipote con pid %d ha terminato in modo anomalo\n", pidNipote);
				}else{
					ritorno = (int)((status >> 8) & 0xFF);
					printf("il nipote con pid %d ha ritornato %d\n", pidNipote, ritorno);
				}
			}
			ritorno = t/256;
			exit(ritorno);
		}
	}

	/*codice del padre*/
	/*chiudiamo le pipe che non servono*/
	for(i = 0; i < N; i++){
		close(pipe_fp[i][1]);
		close(pipe_np[i][1]);
	}

	/*recuperiamo le informazioni da nipoti e figli*/
	for(i = 0; i < N; i++){
		nr = read(pipe_np[i][0], &t, sizeof(t));
		if(nr != sizeof(t)){
			printf("errore nella read\n");
			exit(6);
		}
		printf("il nipote di indice %d ha fatto %ld trasformazioni sul file %s\n", i, t, argv[i+1]);

		nr = read(pipe_fp[i][0], &t, sizeof(t));
		if(nr != sizeof(t)){
			printf("errore nella read\n");
			exit(7);
		}
		printf("il figlio di indice %d ha fatto %ld trasformazioni sul file %s\n", i, t, argv[i+1]);
	}

	/*il padre aspetta i figli*/
	for(i = 0; i < N; i++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(8);
		}
		if((ritorno & 0xFF) != 0){
			printf("il figlio con pid %d ha terminato in modo anomalo\n", pidFiglio);
		}else{
			ritorno = (int)((status >> 8) & 0xFF);
			printf("il figlio con pid %d ha ritornato %d\n", pidFiglio, ritorno);
		}
	}
	exit(0);
}
