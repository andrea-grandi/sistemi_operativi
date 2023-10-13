#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>

typedef int pipe_t[2];

int mia_random(int n){
	int casuale;
	casuale = rand() % n;
	return casuale;
}

int main(int argc, char **argv){

	int pid; 
	int pidFiglio, ritorno, status;
	int i, j;
	int fdout, fd;
	int N;
	pipe_t *pipedFP;
	pipe_t *pipedPF;
	int H;
	int r;
	char linea[255]; 
	int valore, giusto;
	int numero;

	/*controllo numero parametri*/
	if(argc < 6){
		printf("errore nel numero di parametri\n");
		exit (1);
	}

	srand(time(NULL));
	N = argc - 2;
	H = atoi(argv[argc - 1]);

	/*controlliamo ultimo parametro*/
	if((H <= 0 ) || (H >= 255)){
		printf("errore valore ultimo parametro\n");
		exit(2);
	}

	/*il processo padre crea il file /tmp/creato*/
	if((fdout = open("/tmp/creato", O_CREAT|O_WRONLY|O_TRUNC, 0644)) < 0){
		printf("errore nella creazione file\n");
		exit(3);
	}

	pipedFP = (pipe_t *)malloc(N*sizeof(pipe_t));
	pipedPF = (pipe_t *)malloc(N*sizeof(pipe_t));

	if((pipedFP == NULL) || (pipedPF == NULL)){
		printf("errore nella malloc\n");
		exit(4);
	}

	/*ora creiamo N pipe*/
	for (i = 0; i < N; i++){
		if(pipe(pipedFP[i]) < 0){
			printf("errore nel creare la pipe di indice %d figlio-padre\n", i);
			exit(5);
		}
		if(pipe(pipedPF[i]) < 0){
			printf("errore nel creare la pipe di indice %d padre-figlio\n", i);
			exit(6);
		}
	}

	/*ora creiamo i figli*/
	for(i = 0; i < N; i++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(7);
		}
		if(pid == 0){
			/*figlio*/
			printf("sono il processo figlio con pid %d e indice %d e sono associato al file %s\n", getpid(), i, argv[i + 1]);
			for(j = 0; j < N; j++){
				close(pipedFP[j][1]);
				close(pipedPF[j][0]);
				if(i != j){
					close(pipedFP[j][1]);
					close(pipedPF[j][0]);
				}
			}
			if((fd = open(argv[i+1], O_RDONLY)) < 0){
				printf("errore in apertura file\n");
				exit(-1);
			}
			j=0;
			while(read(fd, &(linea[j]), 1)){
				if(linea[j] == '\n'){
					j++;
					write(pipedFP[i][1], &j, sizeof(j));
					read(pipedPF[i][0], &r, sizeof(r));
					if(r < j){
						printf("processo indice %d sta per scrivere carattere %c nel file\n", i, linea[r]);
						write(fdout, &(linea[r]), 1);
						ritorno++;
					}else{
						printf("processo di indice %d non scrive nulla nel file\n", i);
					}
					
					j=0;
				}else{
					j++;
				}
			}
			exit(ritorno);
		}
	}

	/*padre*/
	for(i = 0; i < N; i++){
		close(pipedPF[i][0]);
		close(pipedFP[i][1]);
	}

	/*recupero informazioni dai figli*/
	for(j = 0; j <= H; j++){
		r = mia_random(N);
		printf("il numero generato in modo random per la linea %d compreso tra 0 e %d per selezionare il figlio e': %d\n", j, N-1, r);
		for(i = 0; i < N; i++){
			read(pipedFP[i][0], &valore, sizeof(valore));
			if(i == r){
				giusto=valore;
			}
		}
		r = mia_random(giusto);
		printf("il numero generato in modo random compreso tra 0 e %d per selezionare l'indice della linea %d-esima e': %d\n", giusto-1, j, r);
		for(i = 0; i < N; i++){
			write(pipedPF[i][1], &r, sizeof(r));
		}
	}
	
	/*padre aspetta i figli*/
	for(i = 0; i < N; i++){
		if((pid = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(8);
		}
		if((status & 0xFF) != 0){
			printf("il figlio con pid %d terminato in modo anomalo\n", pidFiglio);
		}else{
			ritorno = (int)((status >> 8) & 0xFF);
			printf("il figlio con pid %d ha ritornato %d\n", pidFiglio, ritorno);
		}
	}
	exit(0);
}


