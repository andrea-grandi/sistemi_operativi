#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>
#define PERM 0644

int mia_random(int n){
	int casuale;
	casuale = rand() % n;
	return casuale;
}

typedef int pipe_t[2];

int main(int argc, char **argv){
	int pid;
	int pidFiglio, status;
	int ritorno = 0;
	int N;
	int fd;
	int i, j;
	int H;
	int Fcreato;
	pipe_t *pipe_fp;
	pipe_t *pipe_pf;
	char linea[250];
	int valore;
	int r;
	int giusto;
	
	if(argc < 6){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	srand(time(NULL));
	
	N = argc - 2;
	
	H = atoi(argv[argc - 1]);
	if(H <= 0 || H >= 255){
		printf("errore nell'ultimo parametro\n");
		exit(2);
	}

	if((Fcreato = open("/tmp/creato", O_CREAT, PERM)) < 0){
		printf("errore nella creazione file\n");
		exit(3);
	}

	/*allochiamo memoria per le pipe*/
	if((pipe_pf = (pipe_t *)malloc(N*sizeof(pipe_t))) == NULL || (pipe_fp = (pipe_t*)malloc(N*sizeof(pipe_t))) == NULL){
		printf("errore nella malloc\n");
		exit(4);
	}

	/*creiamo le pipe*/
	for(i = 0; i < N; i++){
		if(pipe(pipe_fp[i]) < 0){
			printf("errore nella creazione pipe\n");
			exit(5);
		}
		if(pipe(pipe_pf[i]) < 0){
			printf("errore nella creazione pipe\n");
			exit(6);
		}
	}

	/*creazione figli*/
	for(i = 0; i < N; i++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(7);
		}
		if(pid == 0){
			/*codice figlio*/
			/*chiudiamo i lati della pipe*/
			for(j = 0; j < N; j++){
				close(pipe_pf[j][1]);
				close(pipe_fp[j][0]);
				if(i != j){
					close(pipe_pf[j][0]);
					close(pipe_fp[j][1]);
				}
			}

			/*apro il file associato*/
			fd = open(argv[i+1], O_RDONLY);
			if(fd < 0){
				printf("errore in apertura file\n");
				exit(-1);
			}

			/*ora il figlio legge dal file una linea alla volta*/
			j=0;
			while(read(fd, &(linea[j]), 1)){
				if(linea[j] == '\n'){
					j++;
					write(pipe_fp[i][1], &j, sizeof(j));
					read(pipe_pf[i][0], &r, sizeof(r));

					if(r < j){
						//ammissibile
						printf("sono il figlio con pid %d e indice %d e ho ricevuto dal padre l'indice %d\n", getpid(), i, linea[r]);
						write(Fcreato, &(linea[r]), 1);
						ritorno++;
					}else{
						printf("l'indice inviato dal padre non e' ammissibile\n");
					}
					
					j=0;
				}else{
					j++;
				}
			}
			exit(ritorno);
		}
	}

	/*codice padre*/
	/*chiudiamo i lati delle pipe*/
	for(i = 0; i < N; i++){
		close(pipe_pf[i][0]);
		close(pipe_fp[i][1]);
	}

	for(j = 1; j <= H; j++){
		
		r = mia_random(N);
		for(i = 0; i < N; i++){
			read(pipe_fp[i][0], &valore, sizeof(valore));
			if(r == i){
				giusto = valore;
			}
		}

		r = mia_random(giusto);

		for(i = 0; i < N; i++){
			write(pipe_pf[i][1], &r, sizeof(r));
		}
	}

	/*aspettiamo i figli*/
	for(i = 0; i < N; i++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(8);
		}
		if((status & 0xFF) != 0){
			printf("il figlio con pid %d ha termianto in modo anomalo\n", pidFiglio);
		}else{
			ritorno = (int)((status >> 8) & 0xFF);
			printf("il figlio con pid %d ha ritornato %d\n", pidFiglio, ritorno);
		}
	}
	exit(0);
}
