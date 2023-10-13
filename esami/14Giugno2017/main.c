#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>

typedef int pipe_t[2];

int main(int argc, char **argv){
	int pid;
	int pidFiglio, ritorno, status;
	int N;
	int i, j;
	pipe_t *pipe_fp;
	pipe_t *pipe_pf;
	int fd;
	char ch, c;
	char Cx;
	int finito;
	long int posizione;
	char scarto;
	int size;
	int nr;

	if(argc < 3){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	/*CONTROLLIAMO L'ULTIMO PARAMETRO E SALVIAMOLO*/
	size = strlen(argv[argc - 1]);
	if (size != 1){
		printf("errore nell'ultimo parametro\n");
		exit (2);
	}
		
	Cx = argv[argc - 1][0];

	N = argc - 2;

	printf("DEBUG: sono il padre con pid %d e sto per generare %d figli\n", getpid(), N);
	
	/*allochiamo memoria per le pipe*/

	pipe_fp = (pipe_t *)malloc(N*sizeof(pipe_t));
	pipe_pf = (pipe_t *)malloc(N*sizeof(pipe_t));

	if((pipe_pf == NULL) || (pipe_fp == NULL)){
		printf("errore nelle malloc\n");
		exit(3);
	}

	/*creiamo le pipe*/
	for (i = 0; i < N; i++){
		if(pipe(pipe_fp[i]) < 0){
			printf("errore nella creazione delle pipe\n");
			exit(4);
		}
		if(pipe(pipe_pf[i]) < 0){
			printf("errore nella creazione delle pipe\n");
			exit (5);
		}
	}

	/*creiamo i figli*/
	for (i = 0; i < N; i++){
		if ((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(6);
		}
		if (pid == 0){
			
			/*codice del figlio*/
			/*chiudiamo i lati della pipe che non servono*/
			for(j = 0; j < N; j++){
				close(pipe_fp[j][0]);
				close(pipe_pf[j][1]);
				if(i != j){
					close(pipe_fp[j][1]);
					close(pipe_pf[j][0]);
				}
			}

			/*apriamo il file corrispondente al figli*/
			if((fd = open(argv[i + 1], O_RDWR)) < 0){
				printf("errore in apertura file\n");
				exit(-1);
			}
			while(read(fd, &ch, 1)){
				if(ch == Cx){
					posizione = lseek(fd, 0L, SEEK_CUR);
					write(pipe_fp[i][1], &posizione, sizeof(posizione));
					read(pipe_pf[i][0], &c, 1);
					if(c != '\n'){
						lseek(fd, -1L, SEEK_CUR);
						write(fd, &c, 1);
						ritorno++;
					}
				}
			}
			exit(ritorno);
		}
	}

	/*codice del padre*/
	/*chiudiamo i lati delle pipe che non ci interessano*/
	for(i = 0; i < N; i++){
		close(pipe_fp[i][1]);
		close(pipe_pf[i][0]);
	}

	/*recupero delle informazioni*/
	finito = 0;
	while(!finito){
		finito = 1;
		for (i = 0; i < N; i++){
			nr = read(pipe_fp[i][0], &posizione, sizeof(posizione));
			if (nr != 0){
				finito = 0;
				printf("il figlio di indice %d ha letto dal file %s nella posizione %ld il carattere %c. Vuoi sostituire? (type any key)\n", i, argv[i+1], posizione, Cx);
				read(0, &c, 1);
				if(c != '\n'){
					read(0, &scarto, 1);
				}
				write(pipe_pf[i][0], &c, 1);
			}
		}
	}

	/*il padre aspetta i figli*/
	for(i = 0; i < N; i++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(7);
		}
		if((status & 0xFF) != 0){
			printf("il figlio con pid %d e' terminato in modo anomalo\n", pidFiglio);
		}else{
			ritorno = (int)((status >> 8) & 0xFF);
			printf("il figlio con pid %d e indice %d ha ritornato %d\n", pidFiglio, i, ritorno);
		}
	}
	exit(0);
}
