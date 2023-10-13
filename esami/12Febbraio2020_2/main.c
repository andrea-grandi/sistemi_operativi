#include <stdlib.h>
#include <stdio.h> 
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/stat.h>

typedef int pipe_t[2];

typedef struct{
	long int c1;
	long int c2;
}s;

int main(int argc, char **argv){

	int pid;
	int pidFiglio;
	int ritorno = 0;
	int status;
	int i, j;
	int N;
	int fd;
	char ch;
	pipe_t *piped;
	s conteggio;
	long int cont1 = 0;
	long int cont2 = 0;

	if((argc < 3) || ((argc - 1) % 2) != 0){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	N = argc - 1;

	/*allochaimo memoria per la pipe*/
	piped = (pipe_t *)malloc(N*sizeof(pipe_t));
	if(piped == NULL){
		printf("errore nella malloc\n");
		exit(2);
	}

	/*creiamo la pipe*/
	for(i = 0; i < N; i++){
		if(pipe(piped[i]) < 0){
		       printf("errore nella creazione della pipe\n");
		       exit(3);
		}
	}

	/*creazione figli*/
	for(i = 0; i < N; i++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(4);
		}
		if(pid == 0){
			/*codice figlio*/
			/*distinguiamo i processi pari da quelli dispari*/
			if(i % 2 == 0){
				/*siamo in un processo pari*/
				/*chiudiamo i lati della pipe*/
				for(j = 0; j < N; j++){
					close(piped[j][0]);
					if(i != j){
						close(piped[j][1]);
					}
				}

				/*apriamo il file associato*/
				fd = open(argv[i+1], O_RDONLY);
				if(fd < 0){
					printf("errore nella apertura file\n");
					exit(-1);
				}
				
				j=0;
				while(read(fd, &ch, 1)){ 
					if(j % 2 == 0){
						//sono in un carattere pari
						if(ch % 2 == 0){
							cont1++;
						}else{
							cont2++;
						}
					}
					j++;
				}
			}else{
				/*siamo in un processo dispari*/
				for(j = 0; j < N; j++){
					close(piped[j][0]);
					if(j != i){
						close(piped[j][1]);
					}
				}

				fd = open(argv[i+1], O_RDONLY);
				if(fd < 0){
					printf("errore in apertura file\n");
					exit(-1);
				}
				j=0;
				while(read(fd, &ch, 1)){
					if(j % 2 != 0){
						if(ch % 2 != 0){
							cont2++;
						}else{
							cont1++;
						}
					}
					j++;
				}

			}
			conteggio.c1 = cont1;
			conteggio.c2 = cont2;
			write(piped[i][1], &conteggio, sizeof(s));
			if(cont1 > cont2){
				ritorno = 0;
			}else{
				ritorno = 1;
			}
			exit(ritorno);
		}
	}
	/*codice padre*/
	/*chiudiamo i lati della pipe che non ci interessano*/
	for(i = 0; i < N; i++){
		close(piped[i][1]);
	}

	/*il padre legge prima la pipe pari e poi quella dispari*/
	for(i = 0; i < N; i=i+2){
		read(piped[i][0], &conteggio, sizeof(s));
		printf("Figlio con indice %d ha calcolato: c1=%ld e c2=%ld\n", i, conteggio.c1, conteggio.c2);
	}

	for(i = 1; i < N; i=i+2){
		read(piped[i][0], &conteggio, sizeof(s));
		printf("Figlio con indice %d ha calcolato: c1=%ld e c2=%ld\n", i, conteggio.c1, conteggio.c2);
	}	

	/*il padre aspetta i figli*/

	for(i = 0; i < N; i++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(5);
		}
		if((ritorno & 0xFF) != 0){
			printf("il figlio con pid %d ha terminato in modo anomalo\n", pidFiglio);
		}else{
			ritorno = (int)((status >> 8) & 0xFF);
			printf("il figlio con pid %d ha ritornato %d\n", pidFiglio, ritorno);
		}
	}

	exit(0);
}
