#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

typedef int pipe_t[2];

int main(int argc, char **argv){

	int pid, pidFiglio;
	int i, j;
	int ritorno, status;
	char c;
	int fdp;
	int N;
	pipe_t *piped;
	pipe_t p;

	if(argc < 3){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	N = argc - 1;

	/*allochiamo l'array di N pipe*/
	piped = (pipe_t *)malloc(N*sizeof(pipe_t));

	if(piped == NULL){
		printf("errore nella malloc\n");
		exit(2);
	}

	/*creazione delle N pipe*/
	for(i = 0; i < N; i++){
		if(pipe(piped[i]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(3);
		}
	}

	printf("sono il padre con pid %d e sto per generare %d figli\n", getpid(), N);

	/*ciclo generazione figli*/
	for(i = 0; i < N; i++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(4);
		}
		if(pid == 0){
			printf("sono il figlio con pid %d e indice %d e sto per creare il nipote che calcolera' la lunghezza in linee del file %s\n", getpid(), i, argv[i+1]);
			for(j = 0; j < N; j++){
				close(piped[j][0]);
				if(i != j){
					close(piped[j][1]);
				}
			}
			/*creo nipote*/
			if(pipe(p) < 0){
				printf("errore nella creazione pipe\n");
				exit(5);
			}

			if((pid = fork()) < 0){
				printf("errore nella fork (nipote)\n");
				exit(6);
			}

			if(pid == 0){
				/*nipote*/
				printf("sono il processo nipote con pid %d e indice %d e sto per calcolare le linee del file %s\n", getpid(), i, argv[i + 1]);
				close(piped[i][1]);
				close(0);
				if(open(argv[i + 1], O_RDONLY) < 0){
					printf("errore nella open\n");
					exit(-1);
				}
				close(1);
				dup(p[1]);
				close(p[0]);
				close(p[1]);
				close(2);
				open("/dev/null", O_WRONLY);
				execlp("wc", "wc", "-l", (char *)0);
				exit(-1);
			}

			/*figlio*/
			close(p[1]);
			j=0;
			while(read(p[0], &(numero[j]), 1)){
				printf("indice j: %d carattere letto da pipe %c\n", j, numero[j]);
				j++;
			}
			if(j != 0){
				//DA FINIRE!!!!!!
			}

		}
	}

	/*padre*/

	exit(0);
}
