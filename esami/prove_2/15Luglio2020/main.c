#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

typedef int pipe_t[2];

typedef struct{
	int pid;
	char c_secondo;
	char c_penultimo;
}s;

int main(int argc, char **argv){
	int pid; 
	int pidFiglio; 
	int ritorno=0;
	int status;
	int L;
	int q, j;
	char linea[250];
	pipe_t *piped;
	s cur;

	if(argc != 3){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	L = atoi(argv[argc-1]);
	if((L <= 0) || (L > 255)){
		printf("errore numero non positivo\n");
		exit(2);
	}

	printf("DEBUG: numero linee file %d\n", L);

	/*allochiamo memoria per la pipe*/
	piped = (pipe_t *)malloc(L*sizeof(pipe_t));
	if(piped == NULL){
		printf("errore nella malloc\n");
		exit(3);
	}

	/*creiamo la pipe*/
	for(q = 0; q < L; q++){
		if(pipe(piped[q]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(4);
		}
	}

	/*creazione processi figli*/
	for(q = 0; q < L; q++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(5);
		}
		if(pid == 0){
			/*codice figlio*/
			/*chiudiamo i lati della pipe*/
			for(j = 0; j < L; j++){
				close(piped[j][0]);
				if(q != j){
					close(piped[j][1]);
				}
			}
			/*apriamo il file associato, sempre lo stesso file*/
			fd = open(argv[1], O_RDONLY);
			if(fd < 0){
				printf("errore in apertura file\n");
				exit(-1);
			}

			j=0;
			i=1;
			while(read(fd, &(linea[j]), 1)){
				if(linea[j] == '\n'){
					/*siamo arrivati all fine della linea*/
					if(q == i-1){
						/*siamo nella linea associata*/
						cur.pid = getpid();
						cur.c_secondo = linea[1];
						cur.c_penultimo = linea[j-1];
						write(piped[q][1], &cur, sizeof(s));
						break;
					}else{
						j=0;
						i++;
					}
				}else{
					j++;
				}
			}
			ritorno = q+1;
		       exit(ritorno);	
		}
	}
	/*codice padre*/
	/*chiudiamo i lati della pipe che non ci interessa*/
	for(q = 0; q < L; q++){
		close(piped[q][1]);
	}

	for(q = 0; q < L; q++){
		nr = read(piped[q][0], &cur, sizeof(s));
		if(nr != sizeof(s)){
			printf("errore nella read\n");
		}
		if(cur.c_secondo == cur.c_penultimo){
			printf("figlio di indice %d e pid %d ha trovato che il secondo carattere (%c) e il penultimo (%c) della linea %d-esima del file %s sono uguali\n", q, getpid(), cur.c_secondo, cur.c_penultimo, q+1, argv[1]);
		}
	}

	/*il padre aspetta i figli*/


	exit(0);
}
	

