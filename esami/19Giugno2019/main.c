#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

typedef int pipe_t[2];

int *finito;
int N;

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
	int fd;
	char ch;
	pipe_t *pipe_fp;
	pipe_t *pipe_pf;
	int nr, nw;
	int stampe;
	char linea[250];
	char chControllo;
	char chMax;
	int indice;

	if(argc < 3){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	N = argc - 1;

	/*allochiamo memoria per la pipe*/
	pipe_fp = (pipe_t *)malloc(N*sizeof(pipe_t));
	if(pipe_fp == NULL){
		printf("errore nella malloc\n");
		exit(2);
	}

	pipe_pf = (pipe_t *)malloc(N*sizeof(pipe_t));
	if(pipe_pf == NULL){
		printf("errore nella malloc\n");
		exit(6);
	}

	/*creiamo la pipe*/
	for(i = 0; i < N; i++){
		if(pipe(pipe_pf[i]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(3);
		}
		if(pipe(pipe_fp[i]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(7);
		}
	}

	/*creiamo i figli*/
	for(i = 0; i < N; i++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(4);
		}
		if(pid == 0){
			/*codice figlio*/
			/*chiudiamo i lati della pipe*/
			for(j = 0; j < N; j++){
				close(pipe_fp[j][0]);
				close(pipe_pf[j][1]);
				if(j != i){
					close(pipe_fp[j][1]);
					close(pipe_pf[j][0]);
				}
			}
			/*apriamo il file associato*/
			if((fd = open(argv[i+1], O_RDONLY)) < 0){
				printf("errore nella apertura file\n");
				exit(-1);
			}
			/*leggiamo dal file*/
			j=0;
			stampe=0;
			while(read(fd, &linea[j], 1)){
				if(linea[j] == '\n'){
					/*comunico al padre il primo carattere*/
					nw = write(pipe_fp[i][1], &linea[0], 1);
					if(nw != 1){
						printf("errore nella scrittura\n");
						exit(-1);
					}
					nr = read(pipe_pf[i][0], &chControllo, 1);
					if(nr != 1){
						printf("errore nella lettura\n");
						exit(-1);
					}

					if(chControllo == 'S'){
						linea[j+1] = '\0';
						printf("sono il figlio di indice %d e pid %d e ho trovato una linea con un primo carattere %c con codice ASCII maggiore degli altri nel file %s. Ecco la linea: \n%s", i, getpid(), linea[0], argv[i+1], linea);
						stampe++;
					}
					j=0;
				}else{
					j++;
				}
			}
			exit(stampe);
		}
	}
	/*codice padre*/
	/*chiudo i lati della pipe che non interessano*/
	for(i = 0; i < N; i++){
		close(pipe_pf[i][0]);
		close(pipe_fp[i][1]);
	}

	while(!finitof()){
		chMax = -1;
		for(i = 0; i < N; i++){
			finito[i] = (read(pipe_fp[i][0], &ch, 1) != 1);
			if(!finito[i]){
				if(ch > chMax){
					chMax = ch;
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
					printf("errore nella write\n");
				}
			}
		}
	}

	/*il padre aspetta i figli*/
	for (i = 0; i < N; i++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(5);
		}
		if((status & 0xFF) != 0){
			printf("il figlio con pid %d ha terminato in modo anomalo\n", pidFiglio);
		}
		else{
			ritorno = (int)((status >> 8) & 0xFF);
			printf("il figlio con pid %d ha ritornato %d\n", pidFiglio, ritorno);
		}
	}
	exit(0);
}
