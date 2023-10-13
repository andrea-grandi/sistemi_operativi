#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>

typedef int pipe_t[2];
typedef char L[250];

int main(int argc, char **argv){
	int pid;
	int pidFiglio, ritorno, status;
	int N;
	int i, j;
	int fd;
	pipe_t *piped;
	L linea;
	L *l_tot;
	int fcreato;
	int n, k;
	int nr, nw;

	if(argc < 3){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	N = argc - 1;

	l_tot = (L *)malloc(N*sizeof(L));
	if(l_tot == NULL){
		printf("errore nella malloc\n");
		exit(6);
	}

	/*allochiamo memoria per la pipe*/
	piped = (pipe_t *)malloc(N*sizeof(pipe_t));
	if(piped == NULL){
		printf("errore nella malloc\n");
		exit(2);
	}

	/*creazione pipe*/
	for(i = 0; i < N; i++){
		if(pipe(piped[i]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(3);
		}
	}

	/*il processo padre crea il file fcreato*/
	fcreato = open("Grandi", 0644);
	if(fcreato < 0){
		printf("errore nella creazione file\n");
		exit(4);
	}

	/*generazione figli*/
	for(i = 0; i < N; i++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(5);
		}
		if(pid == 0){
			/*codice figli*/
			/*chiudo i lati della pipe*/
			for(j = 0; j < N; j++){
				if(j != i){
					close(piped[j][0]);
				}
				if(j != (i+1)%N){
					close(piped[j][1]);
				}
			}
			
			/*apriamo il file corrispondente*/
			fd = open(argv[i+1], O_RDONLY);
			if(fd < 0){
				printf("errore in apertura file\n");
				exit(-1);
			}

			j = 0;
			while(read(fd, &(linea[j]), 1)){
				if(linea[j] == '\n'){
					/*devo inserire la linea nell'array passato dal figlio precedente*/
					nr = read(piped[i][0], l_tot, N*sizeof(L));
					if(nr != N*sizeof(L)){
						printf("errore in lettura\n");
						exit(-1);
					}
					for(k = 0; k <= j; k++){
						l_tot[i][k] = linea[k];
					}
					/*inviamo l'array al figlio successivo*/
					nw = write(piped[(i+1)%N][1], l_tot, N*sizeof(L));
					if(nw != N*sizeof(L)){
						printf("errore nella write\n");
						exit(-1);
					}

					/*ultimo figlio*/
					if(i == N-1){
						for(n = 0; n < N; n++){
							for(k = 0; k < 250; k++){
								write(fcreato, &(l_tot[n][k]), 1);
								if(l_tot[n][k] == '\n'){
									break;
								}
							}
						}
					}
					ritorno = j+1;
					j = 0;
				}else{
					j++;
				}
			}
			exit(ritorno);
		}
	}
	/*codice padre*/
	/*prossimo exit 7*/

	/*chiudiamo i lati della pipe*/
	for(i = 0; i < N; i++){
		close(piped[i][0]);
		close(piped[i][1]);
	}

	/*il padre manda al primo figlio un array di linee*/
	nw = write(piped[0][1], l_tot, N*sizeof(L));
	if(nw != N*sizeof(L)){
		printf("errore nella write\n");
		exit(7);
	}

	close(piped[0][1]);

	/*il padre aspetta i figli*/
	for(i = 0; i < N; i++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(8);
		}
		if((status & 0xFF) != 0){
			printf("il figlio con pid %d ha terminato in modo anomalo\n", pidFiglio);
		}else{
			ritorno = (int)((ritorno >> 8) & 0xFF);
			printf("il figlio con pid %d ha ritornato %d\n", pidFiglio, ritorno);
		}
	}
	exit(0);
}
