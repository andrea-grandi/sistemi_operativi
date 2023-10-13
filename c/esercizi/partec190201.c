#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char **argv){

	int pid;
	int pidFiglio;
	int i;
	int N;
	int p[2][2];
	int ritorno;
	int status;
	char c, c1, c0;
	int fdr;
	int nr0, nr1;
	int tot;

	/* i parametri passati devono essere almeno 2*/
	if(argc < 3){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	N = argc - 1;
	
	if(pipe(p[0]) < 0){
		printf("errore in creazione della prima pipe\n");
		exit(2);
	}
	
	if(pipe(p[1]) < 0){
		printf("errore in creazione della seconda pipe\n");
		exit(3);
	}

	printf("il processo padre con pid %d sta per generare %d processi figli\n", getpid(), N);

	for(i = 0; i < N; i++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(4);
		}

		if(pid == 0){
			close(p[0][0]);
			close(p[1][0]);
			close(p[i % 2][1]);

			if((fdr = open(argv[i + 1], O_RDONLY)) < 0){
				printf("errore nell'apertura file\n");
				exit(5);
			}

			while(read(fdr, &c, 1) > 0){
				if((((i % 2) == 0) && isalpha(c)) || (((i % 2) == 1) && isdigit(c))){
					write(p[(i+1) % 2][1], &c, 1);
				}
			}
			exit(0);
		}
	}

	close(p[0][1]);
	close(p[1][1]);

	nr0 = read(p[0][0], &c0, 1);
	nr1 = read(p[1][0], &c1, 1);

	while((nr0 != 0) || (nr1 != 0)){
		tot = tot + nr0 + nr1;
		write(1, &c1, nr1);
		write(1, &c0, nr0);
		nr0 = read(p[0][0], &c0, 1);
		nr1 = read(p[1][0], &c1, 1);
	}

	printf("numero di caratteri scritti %d\n", tot);

	for(i=0;i<N;i++)
  	{
    		if ((pidFiglio = wait(&status)) < 0)
    		{
      			printf("Errore wait\n");
      			exit(5);
    		}
    		if ((status & 0xFF) != 0)
                	printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
    		else
    		{
      			ritorno=(int)((status >> 8) & 0xFF);
      			printf("Il figlio con pid=%d ha ritornato %d (se 255 problemi!)\n", pidFiglio, ritorno);
    		}
  	}  
  	exit(0);
}

			

