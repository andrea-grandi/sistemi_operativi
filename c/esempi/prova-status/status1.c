#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){

	int pid, pidFiglio, status, exit_s;

	if((pid = fork()) < 0){
		printf("errore in fork\n");
		exit(1);
	}

	if(pid == 0){
		printf("esecuzione del figlio\n");
		sleep(4);
		exit(5);
	}

	printf("generato figlio con PID = %d\n", pid);

	if((pidFiglio = wait(&status)) < 0){
		printf("errore in wait\n");
		exit(2);
	}

	if(pid == pidFiglio){
		printf("terminato figlio con PID = %d\n", pidFiglio);
	}
	else{
		printf("il pid della wait non corrisponde al pid della fork\n");
		exit(3);
	}

	if((status & 0xFF) != 0){
		printf("figlio terminato in modo involontario\n");
	}
	else{
		exit_s = status >> 8;
		exit_s &= 0xFF;
		printf("per il figlio %d lo stato di EXIT e' %d\n", pid, exit_s);
	}

	exit(0);

}
