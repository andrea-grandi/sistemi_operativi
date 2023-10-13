#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctype.h>

typedef int pipe_t[2];

typedef struct{
	int pid;
	long int occorrenze;
} s;

int main(int argc, char **argv){
	int pid;
	int pidFiglio, ritorno, status;
	int i, j;
	int N;
	char C;
	pipe_t *piped;
	s *cur;
	int fd;
	char ch;
	int nr;

	if(argc < 4){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	if(strlen(argv[argc-1]) != 1){
		printf("errore nell'ultimo parametro\n");
		exit(2);
	}

	C = argv[argc-1][0];
	if(!islower(C)){
		printf("ultimo parametro non minuscolo\n");
		exit(3);
	}

	N = argc - 2;

	printf("DEBUG: carattere da cercare: %d\n", C);

	printf("DEBUG: sono il processo padre con pid %d e sto per creare %d processi figli\n", getpid(), N);

	/*allochiamo la memoria per la pipe*/
	piped = (pipe_t *)malloc(N*sizeof(pipe_t));
	if(piped == NULL){
		printf("errore nella malloc\n");
		exit(4);
	}

	/*creiamo la pipe*/
	for(i = 0; i < N; i++){
		if((pipe(piped[i])) < 0){
			printf("errore nella creazione pipe\n");
			exit(5);
		}
	}

	/*creiamo i figli*/
	for(i = 0; i < N; i++){
		if((pid = fork()) < 0){
			printf("errore nella creazione figlio\n");
			exit(6);
		}
		if(pid == 0){
			/*codice figlio*/
			/*chiudiamo i lati delle pipe che non usiamo*/
			for(j = 0; j < N; j++){
				if(i != j)
					close(piped[j][1]);
				if((i == 0) || (j != i-1))
					close(piped[j][0]);
			}

			/*allochiamo l'arrai per il figlio*/
			if((cur = (s *)malloc(N*sizeof(s))) < 0){
				printf("errore nella malloc\n");
				exit(-1);
			}
			cur[i].pid = getpid();
			cur[i].occorrenze = 0;

			/*apriamo il file*/
			if((fd = open(argv[i+1], O_RDONLY)) < 0){
				printf("errore in apertura file\n");
				exit(-1);
			}
			while(read(fd, &ch, 1)){
				if(ch == C){
					/*trovata occorrenza*/
					(cur[i].occorrenze)++;
				}
			}
			if(i != 0){
				nr = read(piped[i-1][0], cur, i*sizeof(s));
				if(nr != i*sizeof(s)){
					printf("errore nella read\n");
					exit(-1);
				}
			
				for(j = 0; j < i; j++){
					printf("ho ricevuto da figlio di indice %d trovato %ld occorrenze del caratte %c\n", cur[j].pid, cur[j].occorrenze, C);
				}
			}	
		
			write(piped[i][1], cur, (i+1)*sizeof(s));
			exit(i);
		}
	}

	/*codice padre*/
	/*chiudiamo i lati della pipe*/
	for(i = 0; i < N; i++){
		close(piped[i][1]);
		if(i != N-1){
			close(piped[i][0]);
		}
	}

	/*allochiamo l'array per il padre*/
	if((cur = (s *)malloc(N*sizeof(s))) == NULL){
		printf("errore nella malloc\n");
		exit(7);
	}

	/*il padre legge dalla pipe l'array di struct*/
	nr = read(piped[N-1][0], cur, N*sizeof(s));
	if(nr != N*sizeof(s)){
		printf("errore nella read\n");
		exit(8);
	}

	nr = nr/sizeof(s);
	printf("il padre ha letto %d strutture\n", nr);
	for(i = 0; i < N; i++){
		printf("il figlio di indice %d e pid %d ha  trovato %ld occorrenze del carattere %c nel file %s\n", i, cur[i].pid, cur[i].occorrenze, C, argv[i+1]);
	}

	/*il padre aspetta i figli*/
	for(i = 0; i < N; i++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(9);
		}
		if((status & 0xFF) != 0){
			printf("il figlio con pid %d e' terminato in modo anomalo\n", pidFiglio);
		}else{
			ritorno = (int)((status >> 8) & 0xFF);
			printf("il figlio con pid %d ha ritornato %d\n", pidFiglio, ritorno);
		}
	}
	exit(0);
}
