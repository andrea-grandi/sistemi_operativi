#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

typedef int pipe_t[2];
int N;
int *finito;

int finitof(){
	int i;
	for(i = 0; i < N; i++){
		if(!finito[i]){
			return 0;
		}
	}
	return 1;
}


int main(int argc, char **argv){
	int pid;
	int pidFiglio, ritorno, status;
	int i, j;
	pipe_t *pipe_fp;
	pipe_t *pipe_pf;
	long int posizione;
        long int occorrenze;
	char ch;
	char chControllo;
	int nr, nw;
	int posizioneMax;
	char Cz;
	int fd;
	int indice;

	if(argc < 4){
		printf("errore nel numero di parametri\n");
		exit (1);
	}

	if(strlen(argv[1]) != 1){
		printf("errore nel primo parametro\n");
		exit (2);
	}
	
	Cz = argv[1][0];
	N = argc - 2;

	finito = (int *)malloc(N*sizeof(int));
	if(finito == NULL){
		printf("errore nella malloc\n");
		exit(9);
	}

	memset(finito, 0, sizeof(int)*N);

	pipe_pf = malloc(N*sizeof(pipe_t));
	pipe_fp = malloc(N*sizeof(pipe_t));
	if ((pipe_pf == NULL) || (pipe_fp == NULL)){
		printf("errore nella malloc\n");
		exit(3);
	}

	/*creazione pipe*/
	for (i = 0; i < N; i++){
		if(pipe(pipe_pf[i]) != 0){
			printf("errore nella creazione della pipe padre-figlio\n");
			exit (4);
		}
		if(pipe(pipe_fp[i]) != 0){
			printf("errore nella creazione della pipe figlio-padre\n");
			exit(5);
		}
	}

		printf("sono il padre con pid %d e sto per generare %d figli\n", getpid(), N);

		/*creazione dei figli*/
		for (i = 0; i < N; i++){
			if((pid = fork()) < 0){
				printf("errore nella fork()\n");
				exit(6);
			}
			if(pid == 0){
				/*codice del figlio*/
				printf("sono il figlio con pid %d di indice %d associato al file %s\n", getpid(), i, argv[i + 2]);

				/*chiudiamo le pipe che non ci interessano*/
				for (j = 0; j < N; j++){
					close(pipe_fp[j][0]);
					close(pipe_pf[j][1]);
					if(i != j){
						close(pipe_fp[j][1]);
						close(pipe_pf[j][0]);
					}
				}

				/*apriamo il file*/
				if((fd = open(argv[i + 2], O_RDONLY)) < 0){
					printf("errore in apertura file\n");
					exit(-1);
				}

				posizione = 0;
				occorrenze = 0;

				while(read(fd, &ch, 1)){
					if(ch == Cz){
						/*occorrenza carattere trovata*/
						occorrenze++;
						nw = write(pipe_fp[i][1], &posizione, sizeof(posizione));
						if(nw != sizeof(posizione)){
							printf("impossibile scrivere nella pipe\n");
							exit(-1);
						}
						nr = read(pipe_pf[i][0], &posizione, sizeof(posizione));
						if(nr != sizeof(posizione)){
							printf("impossibile leggere da pipe\n");
							exit(-1);
						}
						if(chControllo == 'S'){
							printf("sono il figlio con pid %d e indice %d e ho trovato una occorrenza del carattere %c alla posizione %ld del file %s\n", getpid(), i, Cz, posizione, argv[1 + 2]);
						}
					}
					posizione++;
				}
				exit(occorrenze);
			}
		}

		/*padre*/
		/*chiudiamo le pipe che non interessano*/
		for (i = 0; i < N; i++){
			close(pipe_pf[i][0]);
			close(pipe_fp[i][1]);
		}
		while(!finitof()){
			posizioneMax = -1;
			for(i = 0; i < N; i++){
				finito[i] = (read(pipe_fp[i][0], &posizione, sizeof(posizione)) != sizeof(posizione));
				if(!finito[i]){
					if(posizione > posizioneMax){
						posizioneMax = posizione;
						indice = i;
					}
				}
			}
			for(i = 0; i < N; i++){
				if(i == indice){
					chControllo = 'S';
				}else{
					chControllo = 'N';
				}
				if(!finito[i]){
					nw = write(pipe_pf[i][1], &chControllo, 1);
					if(nw != 1){
						printf("il padre non è riuscito ad inviare nulla al figlio\n");
					}
				}
			}
		}

		for(i = 0; i < N; i++){
			if((pidFiglio = wait(&status)) < 0){
				printf("errore nella wait\n");
				exit(7);
			}
			if((status && 0xFF) != 0){
				printf("il figlio con pid %d è terminato in modo anomalo\n", pidFiglio);
			}else{
				ritorno = (int)((status >> 8) & 0xFF);
				printf("il figlio con pid %d ha ritornato %d\n", pidFiglio, ritorno);
			}
		}
		exit(0);
	
}
