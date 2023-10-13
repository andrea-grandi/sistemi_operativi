#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv){

	int pid;
	int pidFiglio,status, ritorno;
	char st[80];

	for(;;){

		printf("inserire il comando da eseguire:\n");
		scanf("%s", st);

		if((pid = fork()) < 0){

			perror("fork");
			exit(errno);
		
		}

		if (pid == 0){

			execlp(st, st, (char *)0);
			perror("errore esecuzione comando");
			exit(errno);

		}

		pidFiglio = wait(&status);

		if(pidFiglio < 0){
				
			perror("errore wait");
			exit(errno);
		
		}

		if ((status & 0xFF) != 0){

			printf("figlio con pid %d terminato in modo anomalo\n", pidFiglio);
		}
		else{
			ritorno = (int)((status >> 8) & 0xFF);
			printf("il figlio con pid %d ha ritornato %d\n", pidFiglio, ritorno);
			printf("eseguire altro comando?\n");
			scanf("%s", st);
			if(strcmp(st, "si")){
				exit(0);
			}
		}
	}
}



