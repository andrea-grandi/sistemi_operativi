#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

	int pid;

	printf("UNO\n");

	if((pid = fork()) < 0){

			printf("Errore in fork\n");
			exit (1);
			
	}

	if(pid == 0){
		printf("DUE\n");
	}
	else{
		printf("Ho creato figlio con PID = %d\n", pid);
		exit(0);
	}
}


