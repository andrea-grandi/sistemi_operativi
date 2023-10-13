#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

typedef int pipe_t[2];

typedef struct{
	int pid;
	char seconda;
	char penultima;
} struct_figlio;

int main(int argc, char **argv){
	int pid;
	int pidFiglio, ritorno, status;
	struct_figlio s;
	pipe_t *piped;
	char linea[250];
	int L;
	int fd;
	int nr;
	int i, j;
	int a, b;

	if(argc != 3){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	L = atoi(argv[argc - 1]);

	if((L <= 0) || (L > 255)){
		printf("errore nell'ultimo parametro\n");
		exit(2);
	}

	/*ora allochiamo la memoria per la pipe*/
	piped = (pipe_t *)malloc(L*sizeof(pipe_t));
	if(piped == NULL){
		printf("errore nella malloc\n");
		exit (3);
	}

	for(i = 0; i < L; i++){
		if(pipe(piped[i]) < 0){
			printf("errore in creazione pipe\n");
			exit(4);
		}
	}

	printf("DEBUG: sono il padre con pid %d e sto per generare %d processi figli\n", getpid(), L);

	/*creiamo i figli*/
	for(i = 0; i < L; i++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(5);
		}
		if(pid == 0){
			/*codice figlio*/
			printf("DEBUG: sono il figlio con pid %d e di indice %d associato alla linea %d del file %s\n", getpid(), i, i, argv[1]);

			/*chiudiamo i lati della pipe che non ci interessano*/
			for(j = 0; j < L; j++){
				close(piped[j][0]);
				if(i != j){
					close(piped[j][1]);
				}
			}

			/*apriamo il file associato*/
			if((fd = open(argv[1], O_RDONLY)) < 0){
				printf("errore in apertura file\n");
				exit(-1);
			}

			a=1; //conteggio linee 
			b=0; //valore iniziale per il conteggio interno alla linea

			while(read(fd, &(linea[b]), 1) != 0){
				if(linea[b] == '\n'){
					if(i+1 == a){
						s.pid = getpid();
						s.seconda = linea[1];
						s.penultima = linea[b - 1];
						write(piped[i][1], &s, sizeof(s));
						break;
					}else{
						b = 0;
						a++;
					}
				}else{
					b++;
				}
			}
			ritorno = i+1;
			exit(ritorno);
		}
	}
	/*codice del padre*/
	/*chiudo i lati della pipe che non servono*/
	for(i = 0; i < L; i++){
		close(piped[i][1]);
	}

	/*recupero delle informazioni*/
	for(i = 0; i < L; i++){
		nr = read(piped[i][0], &s, sizeof(s));
		if(nr != 0){
			if(s.seconda == s.penultima){
				printf("il figlio con indice %d e pid %d ha trovato che il secondo %c e il penultimo %c carattere sono UGUALI\n", i, s.pid, s.seconda, s.penultima);
			}else{
				printf("il figlio con indice %d e pid %d ha trovato che il secondo %c e il penulltimo %c carattere sono DIVERSI\n", i, s.pid, s.seconda, s.penultima);
			}
		}
	}

	/*il padre aspetta i figli*/
	for(i = 0; i < L; i++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(8);
		}
		if((status & 0xFF) != 0){
			printf("il figlio con pid %d ha terminato in modo anomalo\n", pidFiglio);
		}else{
			ritorno = (int)((status >> 8) & 0xFF);
			printf("il figlio con pid %d e indice %d ha ritornato %d\n", pidFiglio, i, ritorno);
		}
	}
	exit(0);
}
