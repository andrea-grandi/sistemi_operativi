#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){

	int pid, pidFiglio, status, exit_s;
	int valore;

	if ((pid = fork()) < 0){
		printf("errore in fork\n");
		exit(1);
	}

	if (pid == 0){
		printf("esecuzione del figlio\n");
		printf("dammi un valore intero per provare la exit:\n");
		scanf("%d", &valore);
		if ((valore > 255) || (valore < 0)){
			printf("ATTENZIONE IL VALORE VERRA TRONCATO\n");
		}
		else{
			printf("il valore fornito non verra troncato\n");	
		}
		exit(valore);
	}

	printf("generato figlio con PID = %d\n", pid);

	if ((pidFiglio = wait(&status)) < 0){
		printf("errore nella wait\n");
		exit(2);
	}

	if (pid == pidFiglio){
		printf("terminato figlio con PID = %d\n", pidFiglio);
	}
	else{
		printf("pid della wait non corrisponde al pid della fork\n");
		exit(3);
	}

	if ((status & 0xFF) != 0){
		printf("figlio terminato in modo involontario\n");
	}
	else{
		exit_s = status >> 8;
		exit_s &= 0xFF;
		printf("per il figlio %d lo stato di EXIT e %d\n", pid, exit_s);
	}

	exit(0);
}

