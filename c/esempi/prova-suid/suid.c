#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
	
	int pid;
	int pidFiglio, status, ritorno;

	pid = fork();

	if (pid < 0){
		printf("errore in fork\n");
		exit(1);
	}

	if (pid == 0){
		printf("real-user id = %d\n", getuid());
		printf("effective-user id = %d\n", geteuid());
		printf("esecuzione di programma (con suid settato)\n");
		printf("che visualizza file\n");
		execl("leggiPippo1", "leggiPippo1", (char *)0);
		printf("errore nella exec\n");
		exit (-1);
	}

	pidFiglio = wait(&status);

	if (pidFiglio < 0){
		printf("errore nella wait\n");
		exit(2);
	}

	if ((status & 0xFF) != 0){
		printf("figlio con pid %d terminato in modo anomalo\n", pidFiglio);
	}
	else{
		ritorno=(int)((status >> 8) & 0xFF);
		printf("il figlio con pid = %d ha ritornato %d\n", pidFiglio, ritorno);
	}

	exit(0);
}
