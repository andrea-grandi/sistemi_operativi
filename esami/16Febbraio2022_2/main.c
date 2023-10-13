#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>

typedef int pipe_t[2];

typedef struct{
	int pid;
	long int occ;
} s;

int main(int argc, char **argv){
	
	int pid;
	int pidFiglio;
	int ritorno = 0;
	int status;
	int N;
	int i, j;
	char ch;
	s *cur;
	int fd;
	int nr, nw;
	char C;
	pipe_t *piped;

	if(argc < 4){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	C = argv[argc - 1][0];
	if(strlen(argv[argc - 1]) != 1 || !islower(C)){
		printf("errore nell'ultimo parametro\n");
		exit(2);
	}

	N = argc - 2;

	/*allochiamo memoria per la pipe*/
	piped = (pipe_t *)malloc(N*sizeof(pipe_t));
	if(piped == NULL){
		printf("errore nella malloc\n");
		exit(3);
	}

	/*creiamo la pipe*/
	for(i = 0; i < N; i++){
		if(pipe(piped[i]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(4);
		}
	}

	/*creiamo i figli*/
	for(i = 0; i < N; i++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(5);
		}
		if(pid == 0){
			/*codice del figlio*/
			/*chiudo i lati della pipe che non servono*/
			for(j = 0; j < N; j++){
				if(j != i){
					close(piped[j][1]);
				}
				if((j != i-1) || (i == 0)){
					close(piped[j][0]);
				}
			}

			/*allochiamo l'array di strutture*/
			cur = (s *)malloc((i+1)*sizeof(s));
			if(cur == NULL){
				printf("errore nella malloc\n");
				exit(-1);
			}
			cur[i].pid = getpid();
			cur[i].occ = 0;

			/*apriamo il file associato*/
			fd = open(argv[i+1], O_RDONLY);
			if(fd < 0){
				printf("errore in apertura file\n");
				exit(-1);
			}
			
			while(read(fd, &ch, 1)){
				if(ch == C){
					cur[i].occ++;
				}
			}

			if(i != 0){
				nr = read(piped[i-1][0], cur, i*sizeof(s));
				if(nr != i*sizeof(s)){
					printf("errore nella read\n");
					exit(-1);
				}
			}

			printf("figlio di indice %d ha trovato %ld occorrenze\n", i, cur[i].occ);

			nw = write(piped[i][1], cur, (i+1)*sizeof(s));
			if(nw != (i+1)*sizeof(s)){
				printf("errore nella write\n");
				exit(-1);
			}

			exit(i);
		}		
	}
	/*codice del padre*/
	/*chiudo i lati della pipe*/
	for(i = 0; i < N; i++){
		close(piped[i][1]);
		if(i != N-1)
			close(piped[i][0]);
	}

	/*allochiamo l'array di N strutture*/
	cur = (s *)malloc(N*sizeof(s));
	if(cur == NULL){
		printf("errore nella malloc\n");
		exit(6);
	}

	/*leggiamo dalla pipe*/
	nr = read(piped[N-1][0], cur, N*sizeof(s));
	if(nr != N*sizeof(s)){
		printf("errore nella read\n");
		exit(7);
	}

	for(i = 0; i < N; i++){
		printf("il figlio di indice %d e pid %d ha trovato %ld occorrenze del carattere %c nel file %s\n", i, cur[i].pid, cur[i].occ, C, argv[i+1]);
	}

	/*il padre aspetta i figli*/
	for(i = 0; i < N; i++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(8);
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
