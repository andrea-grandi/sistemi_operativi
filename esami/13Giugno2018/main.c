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
	int lunghezza;
} s;

typedef struct{
	int pid;
	int indice;
	int lunghezza;
} s_padre;

void b_sort(s_padre v[], int dim){
	int i;
	int ordinato = 0;
	s_padre a;

	while(dim > 1 && ordinato != 1){
		ordinato = 1;
		for(i = 0; i < dim-1; i++){
			if(v[i].lunghezza > v[i+1].lunghezza){
				a = v[i];
				v[i] = v[i+1];
				v[i+1] = a;
				ordinato = 0;
			}
		}
		dim--;
	}
}

int main(int argc, char **argv){
	int pid;
	int pidFiglio, ritorno, status;
	int i, j;
	int N;
	int fd;
	pipe_t *pipes;
	char ch;
	int L;
	int nr;
	s *cur;
	s_padre *cur_padre;

	if(argc < 4){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	/*controlliamo ultimo parametro*/

	L = atoi(argv[argc - 1]);

	if(L < 0){
		printf("errore nell'ultimo parametro\n");
		exit(2);
	}

	N = argc - 2;

	/*allochiamo la memoria per le pipe*/
	pipes = (pipe_t *)malloc(N*sizeof(pipe_t));

	if((pipes == NULL) || (pipes == NULL)){
		printf("errore nelle malloc\n");
		exit(3);
	}

	/*creiamo le pipe*/
	for(i = 0; i < N; i++){
		if((pipe(pipes[i])) < 0){
			printf("errore nella creazione pipe padre-figlio\n");
			exit(4);
		}
		if((pipe(pipes[i])) < 0){
			printf("errore nella creazione della pipe figlio-padre\n");
			exit(5);
		}
	}

	printf("DEBUG: sono il padre con pid %d e sto per creare %d processi figli\n", getpid(), N);

	/*creazione figli*/
	for (i = 0; i < N; i++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(6);
		}
		if(pid == 0){
			/*codice figlio*/
			/*andiamo a chiudere i lati della pipe che non servono*/
			for(j = 0; j < N; j++){
				if((i == 0) || (j != i-1)){
					close(pipes[j][0]);
				}
				if(j != i){
					close(pipes[j][1]);
				}
			}

			printf("DEBUG: sono il figlio con pid %d e sono  associato al file %s\n", getpid(), argv[i + 1]);

			if((cur = (s *)malloc((i+1)*sizeof(s))) == NULL){
				printf("errore nella malloc\n");
				exit(N);
			}

			cur[i].pid = getpid();
			cur[i].lunghezza = 0;

			/*andiamo ad aprire il file associato*/
			if((fd = open(argv[i+1], O_RDONLY)) < 0){
				printf("errore nell'apertura file\n");
				exit(N+1);
			}
			while(read(fd, &ch, 1)){
				cur[i].lunghezza++;
				if(ch == '\n'){
					if(i != 0){
						nr = read(pipes[i-1][0], cur, i*sizeof(s));
						if(nr != i*sizeof(s)){
							printf("errore nella lettura\n");
							exit(N+2);
						}
						for(j = 0; j < i; j++){
							printf("ho ricevuto dal figlio con pid %d la lunghezza %d\n", cur[j].pid, cur[j].lunghezza);
						}
					}
					write(pipes[i][1], cur, (i+1)*sizeof(s));
					cur[i].lunghezza = 0;
				}
			}
			exit(i);
		}
	}

	/*codice padre*/
	/*chiudiamo i lati della pipe*/
	for(i = 0; i < N; i++){
		close(pipes[i][1]);
		if(i != N-1){
			close(pipes[i][0]);
		}
	}

	cur = (s *)malloc(N*sizeof(s));
	cur_padre = (s_padre *)malloc(N*sizeof(s_padre));
	if((cur == NULL) || (cur_padre == NULL)){
		printf("errore nella malloc\n");
		exit(7);
	}

	for(j = 0; j <= L; j++){
		nr = read(pipes[N-1][0], cur, N*sizeof(s));
		if(nr != N*sizeof(s)){
			printf("errore nella lettura\n");
			exit(8);
		}
		for(i = 0; i < N; i++){
			cur_padre[i].pid = cur[i].pid;
			cur_padre[i].indice = i;
			cur_padre[i].lunghezza = cur[i].lunghezza;
		}

		b_sort(cur_padre, N);
		for(i = 0; i < N; i++){
			printf("il figlio con pid %d per il file %s ha calcolato per la linea %d la lunghezza %d\n", cur_padre[i].pid, argv[cur_padre[i].indice + 1], j, cur_padre[i].lunghezza);
		}
	}

	/*il padre aspetta i figli*/
	for (i = 0; i < N; i++){
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
