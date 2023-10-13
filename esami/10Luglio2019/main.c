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
	int fd;
	char Cz;
	pipe_t *pipe_f1;
	pipe_t *pipe_f2;
	long int pos;
	int occ;
	int nw, nr;
	char ch;
	long int posLetta;

	if(argc < 4){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	Cz = argv[argc - 1][0];
	printf("il carattere da cercare e' : %c\n", Cz);

	if(strlen(argv[argc - 1]) != 1){
		printf("errore nell'ultimo parametro\n");
		exit(2);
	}

	N = argc - 2;

	/*allochiamo memoria per le pipe*/
	pipe_f1 = (pipe_t *)malloc(N*sizeof(pipe_t));
	pipe_f2 = (pipe_t *)malloc(N*sizeof(pipe_t));
	if(pipe_f1 == NULL || pipe_f2 == NULL){
		printf("errore nella malloc di piped\n");
		exit(4);
	}

	/*creiamo la pipe*/
	for(i = 0; i < N; i++){
		if(pipe(pipe_f1[i]) < 0){
			printf("errore nella creazione pipe\n");
			exit(5);
		}
		if(pipe(pipe_f2[i]) < 0){
			printf("errore nella creazione pipe\n");
			exit(6);
		}
	}

	/*creiamo i figli*/
	for(i = 0; i < 2*N; i++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(7);
		}
		if(pid == 0){
			/*codice figlio*/
			/*ora distinguiamo il primo e il secondo membro della coppia*/
			if(i < N){
				/*siamo nei primi N figli*/
				/*chiudiamo i lati della pipe*/
				for(j = 0; j < N; j++){
					close(pipe_f1[j][0]);
					close(pipe_f2[j][1]);
					if(i != j){
						close(pipe_f1[j][1]);
						close(pipe_f2[j][0]);
					}
				}

				/*apriamo il file associato*/
				fd = open(argv[i+1], O_RDONLY);
				if(fd < 0){
					printf("errore nell'apertura file\n");
					exit(-1);
				}
				
				occ=0;
				while(read(fd, &ch, 1)){
					if(ch == Cz){
						occ++;					
						pos = lseek(fd, 0L, SEEK_CUR) - 1;
						nw = write(pipe_f1[i][1], &pos, sizeof(pos));
						if(nw != sizeof(pos)){
							printf("errore nella write\n");
							exit(-1);
						}

						nr = read(pipe_f2[i][0], &posLetta, sizeof(posLetta));
						if(nr != sizeof(posLetta)){
							break;
						}

						lseek(fd, posLetta+1, SEEK_SET);
					}			
				}
				exit(occ);

			}else{
				/*siamo nella seconda coppia di figli*/
				for(j = 0; j < N; j++){
					close(pipe_f1[j][1]);
					close(pipe_f2[j][0]);
					if(j != 2*N-i-1){
						close(pipe_f1[j][0]);
						close(pipe_f2[j][1]);
					}
				}

				fd = open(argv[2*N-1], O_RDONLY);
				if(fd < 0){
					printf("errore in apertura file\n");
					exit(-1);
				}
				
				occ=0;
				nr = read(pipe_f1[2*N-i-1][0], &posLetta, sizeof(posLetta));
				if(nr != sizeof(posLetta)){
					printf("errore nella read\n");
					exit(-1);
				}

				lseek(fd, posLetta+1, SEEK_SET);
 
				while(read(fd, &ch, 1)){
					if(ch == Cz){
						occ++;
						pos = lseek(fd, 0L, SEEK_CUR) - 1;
						nw = write(pipe_f2[2*N-i-1][1], &pos, sizeof(pos));
						if(nw != sizeof(pos)){
							printf("errore nella write\n");
							exit(-1);
						}

						nr = read(pipe_f1[2*N-i-1][0], &posLetta, sizeof(posLetta));
						if(nr != sizeof(posLetta)){
							break;
						}

						lseek(fd, posLetta+1, SEEK_SET);
					}
				}
				exit(occ);
			}
		}
	}

	/*codice del padre*/

	for(i = 0; i < N; i++){
		close(pipe_f1[i][0]);
		close(pipe_f1[i][1]);
		close(pipe_f2[i][0]);
		close(pipe_f2[i][1]);
	}

	for(i = 0; i < 2*N; i++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(7);
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
