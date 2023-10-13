#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
	int pid;

	pid = fork();

	if(pid == 0){
		printf("esecuzione di ls\n");
		execl("/bin/ls", "ls", "-l", (char *)0);
		printf("errore in execl\n");
		exit(1);
	}

	if (pid > 0){
		wait ((int *)0);
		printf("terminato ls\n");
		exit(0);
	}

	if (pid < 0){
		printf("errore in fork\n");
		exit(1);
	}
}
