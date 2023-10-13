#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef int pipe_t[2];

int main(int argc, char **argv){
	int pid;
	int pidFiglio;
	int ritorno=0;
	int status;
	int fd;
	int N;
	int i, j;
	int nr, nw;
	char Cz;
	long int occ;
	long int pos;
	long int posLetta;
	char ch;
	pipe_t *pipe_12; //pipe per comunicare da figlio 1 a figlio 2
	pipe_t *pipe_21; //pipe per comunicare da figlio 2 a figlio 1

	if(argc < 4){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	/*controllo ultimo parametro*/
	if(strlen(argv[argc - 1]) != 1){
		printf("errore nell'ultimo parametro\n");
		exit(2);
	}
	
	Cz = argv[argc - 1][0];
	N = argc - 2;

	/*allochiamo memoria per la pipe*/
	pipe_12 = (pipe_t *)malloc(N*sizeof(pipe_t));
	pipe_21 = (pipe_t *)malloc(N*sizeof(pipe_t));
	if((pipe_12 == NULL) || (pipe_21 == NULL)){
		printf("errore nella malloc della pipe\n");
		exit(3);
	}

	/*creiamo la pipe*/
	for(i = 0; i < N; i++){
		if(pipe(pipe_12[i]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(4);
		}
		if(pipe(pipe_21[i]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(5);
		}
	}

	/*generiamo i figli*/
	for(i = 0; i < 2*N; i++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(6);
		}
		if(pid == 0){
			/*codice figlio*/
			/*devo distinguere primo e secondo figlio della coppia*/
			if(i <= N-1){
				/*codice dei primi figli*/
				/*chiudo i lati delle pipe*/
				for(j = 0; j < N; j++){
					close(pipe_12[j][0]);
					close(pipe_21[j][1]);
					if(j != i){
						close(pipe_12[j][1]);
						close(pipe_21[j][0]);
					}
				}
				/*apro il file associato*/
				fd = open(argv[i+1], O_RDONLY);
				if(fd < 0){
					printf("errore in apertura file\n");
					exit(-1);
				}
				
				occ = 0;
				while(read(fd, &ch, 1)){
					if(ch == Cz){
						/*trovata occorrenza del carattere*/
						occ++;
						/*ora devo inviare la posizione*/
						pos = lseek(fd, 0L, SEEK_CUR) - 1;
						nw = write(pipe_12[i][1], &pos, sizeof(pos));
						if(nw != sizeof(pos)){
							printf("errore nella write\n");
							exit(-1);
						}
						nr = read(pipe_21[i][0], &posLetta, sizeof(posLetta));
						if(nr != sizeof(posLetta)){
							break;
						}
						lseek(fd, posLetta+1, SEEK_SET);
					}
				}
				exit(occ);

			}else{
				/*coduice dei secondi figli*/
				/*chiudo i lati della pipe*/
				for(j = 0; j < N; j++){
					close(pipe_21[j][0]);
					close(pipe_12[j][1]);
					if(j != 2*N-i-1){
						close(pipe_12[j][0]);
						close(pipe_21[j][1]);
					}
				}
				/*apriamo il file associato*/
				fd = open(argv[2*N-i], O_RDONLY);
				if(fd < 0){
					printf("errore in apertura file\n");
					exit(-1);
				}

				occ = 0;
				nr = read(pipe_21[2*N-i-1][0], &posLetta, sizeof(posLetta));
				if(nr != sizeof(posLetta)){
					printf("errore nella read\n");
					exit(-1);
				}
				lseek(fd, posLetta+1, SEEK_SET);
				while(read(fd, &ch, 1)){
					if(ch == Cz){
						/*trovata occorrenza*/
						occ++;
						pos = lseek(fd, 0L, SEEK_CUR) - 1;
						nw = write(pipe_21[2*N-i-1][1], &pos, sizeof(pos));
						if(nw != sizeof(pos)){
							printf("errore nella write\n");
							exit(-1);
						}
						nr = read(pipe_12[2*N-i-1][0], &posLetta, sizeof(posLetta));
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
	/*codice padre*/
	/*chiudo i lati della pipe*/
	for(i = 0; i < N; i++){
		close(pipe_12[i][1]);
		close(pipe_21[i][1]);
		close(pipe_12[i][0]);
		close(pipe_21[i][0]);
	}

	/*il padre aspetta i figli*/
	for(i = 0; i < 2*N; i++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(7);
		}
		if((status & 0xFF) != 0){
			printf("figlio terminato in modo anomalo\n");
		}else{
			ritorno = (int)((status >> 8) & 0xFF);
			printf("Figlio con pid %d ha ritornato %d\n", pidFiglio, ritorno);
		}
	}
	exit(0);
}


