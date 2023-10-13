#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char **argv){
	int pid, pidFiglio;
	int ritorno, status;
	int i;
	int fdr;
	int N;
	int p[2];
	char c;


	if(argc < 4){
		printf("errore nel numero dei parametri\n");
		exit(1);
	}

	struct{
		char c;
		long int n;
	} msg;

	N = argc - 2;

	//controllo singoli caratteri
	for (i = 0; i < N; i++){
		if(strlen(argv[i+2]) != 1){
			printf("errore nella stringa %s: non singolo carattere\n", argv[i+2]);
			exit(2);
		}
	}

	//creo la pipe
	if(pipe(p) < 0){
		printf("errore nella creazione della pipe\n");
		exit(3);
	}

	printf("sono il processo padre con pid %d e sto per creare %d processi figli\n", getpid(), N);

	for(i = 0; i < N; i++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(4);
		}
		if(pid == 0){
			printf("sono il processo figlio con pid %d con indice %d: sono associato al carattere %c\n", getpid(), i, argv[i+2][0]);

			close(p[0]);

			if((fdr = open(argv[1], O_RDONLY)) < 0){
				printf("errore nella open\n");
				exit (-1);
			}

			msg.c = argv[i+2][0];
			msg.n = 0L;

			while(read(fdr, &c, 1) > 0){
				if(c == argv[i + 2][0]){
					msg.n++;
				}
			}

			write(p[1], &msg, sizeof(msg));
			exit(0);
		}
	}

	close(p[1]);
	while(read(p[0], &msg, sizeof(msg)) > 0){
		printf("%ld occorrenze del carattere %c nel file %s\n", msg.n, msg.c, argv[1]);
	}

	//classico controllo wait
	for(i = 0; i < N; i++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(5);
		}
		if((status & 0xFF) != 0){
			printf("il figlio con pid %d e' terminato in modo anomalo\n", pidFiglio);
		}else{
			ritorno = (int)((status >> 8) & 0xFF);
			printf("il figlio con pid %d ha ritornato %d\n", pidFiglio, ritorno);
		}
	}
	exit(0);
}
