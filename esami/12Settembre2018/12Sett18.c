#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctype.h>

typedef int pipe_t[2];

int main(int argc, char **argv){
	int pid, pidFiglio, pidNipote, ritorno, status;
	int N;
	int i, j;
	pipe_t *pipe_fp;
	pipe_t *pipe_np;
	long int trasformazioni;
	int fd;
	char ch;


	if(argc < 3){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	N = argc - 1;

	/*allochiamo la memoria per le pipe*/
	pipe_fp = (pipe_t *)malloc(N*sizeof(pipe_t));
	pipe_np = (pipe_t *)malloc(N*sizeof(pipe_t));

	if((pipe_fp == NULL) || (pipe_np == NULL)){
		printf("errore nella malloc\n");
		exit(2);
	}

	/*creazione delle pipe figlio-padre*/
	for(i = 0; i < N; i++){
		if(pipe(pipe_fp[i]) < 0){
			printf("errore nella creazione della pipe figlio-padre\n");
			exit(3);
		}
		if (pipe(pipe_np[i]) < 0){
			printf("errore nella creazione della pipe nipote-padre\n");
			exit(4);
		}
	}

	printf("DEBUG: sono il padre con pid %d e sto per generare %d processi figli\n", getpid(), N);

	/*generazione figli*/
	for (i = 0; i < N; i++){
		if ((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(5);
		}
		if (pid == 0){
			/*codice del figlio*/
			printf("sono il figlio con pid %d  di indice %d e sto per creare il nipote che leggera' dal mio stesso file %s\n", getpid(), i, argv[i+1]);
		       /*chiudo le pipe che non servono*/
			for(j = 0; j < N; j++){
				close(pipe_fp[j][0]);
				if(j != i){
					close(pipe_fp[j][1]);
				}
			}

			if((pid = fork()) < 0){
				printf("errore nella fork\n");
				exit(-1);
			}
			if(pid == 0){
				/*codice del nipote*/
				/*il nipote deve leggere dal file argv[i+1] e deve trasformare tutte le minuscole in maiuscole*/

				printf("DEBUG: sono il processo nipote con pid %d di indice %d\n", getpid(), j);

				/*chiudo le pipe che non servono*/
				close(pipe_fp[i][1]);
				for(j = 0; j < N; j++){
					close(pipe_np[j][0]);
					if(i != j){
						close(pipe_np[j][1]);
					}
				}

				/*ora apriamo il file*/
				if((fd = open(argv[i+1], O_RDWR)) < 0){
					printf("errore in apertura file\n");
					exit(-1);
				}
				while(read(fd, &ch, 1)){
					if(islower(ch)){
						lseek(fd, -1L, SEEK_CUR);
						ch = ch - 32;
						write(fd, &ch, 1);
						trasformazioni++;
					}
				}

				write(pipe_np[i][1], &trasformazioni, sizeof(trasformazioni));
				ritorno = trasformazioni/256;
				exit(ritorno);
			}else{
				/*codice del figlio*/
				/*il figlio invece deve trasformare tutti i caratteri numerici in spazi*/
				/*chiudiamo le pipe*/
				for(j = 0; j < N; j++){
					close(pipe_np[j][0]);
					close(pipe_np[j][1]);
				}

				/*apriamo il file*/
				if((fd = open(argv[i+1], O_RDWR)) < 0){
					printf("errore in apertura file\n");
					exit(-1);
				}
				while(read(fd, &ch, 1)){
					if(isdigit(ch)){
						lseek(fd, -1L, SEEK_CUR);
						ch = ' ';
						write(fd, &ch, 1);
						trasformazioni++;
					}
				}
				write(pipe_fp[i][1], &trasformazioni, sizeof(trasformazioni));

				if((pidNipote = wait(&status)) < 0){
					printf("errore nella wiat\n");
					exit(-1);
				}
				if((status & 0xFF) != 0){
					printf("il processo nipote con pid %d e' terminato in modo anomalo\n", pidNipote);
				}else{
					ritorno = (int)((status >> 8) & 0xFF);
					printf("il processo nipote con pid %d ha ritornato %d\n", pidNipote, ritorno);
				}
				ritorno = trasformazioni/256;
				exit(ritorno);
			}
		}
	}
	
	/*sono il padre*/
	/*chiudo i lati della pipe che non mi servono*/
	for (i = 0; i < N; i++){
		close(pipe_fp[i][1]);
		close(pipe_np[i][1]);
	}

	/*recupero delle informazioni*/
	for(i = 0; i < N; i++){
		read(pipe_fp[i][0], &trasformazioni, sizeof(trasformazioni));
		printf("il figlio di indice %d ha operato %ld trasformazioni di caratteri numerici in carattere spazio sul file %s\n", i, trasformazioni, argv[i+1]);
		read(pipe_np[i][0], &trasformazioni, sizeof(trasformazioni));
		printf("il nipote di indice %d ha operato %ld trasformazioni di caratteri minuscoli in MAIUSCOLI sullo stesso file %s\n", i, trasformazioni, argv[i+1]);
	}

	/*il padre aspetta i figli*/
	for (i = 0; i < N; i++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(5);
		}
		if((status & 0xFF) != 0){
			printf("il figlio con pid %d e' terminato in modo anomalo\n", pidFiglio);
		}else{
			ritorno = (int)((status >> 8) & 0xFF);
			if(ritorno == 255){
				printf("il figlio con pid %d ha ritornato %d --> problemi\n", pidFiglio, ritorno);
			}
			printf("il figlio con pid %d ha ritornato %d\n", pidFiglio, ritorno);
		}
	}
	exit(0);
}
