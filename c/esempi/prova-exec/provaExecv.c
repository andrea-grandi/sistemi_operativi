#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){

	char *av[3];

	av[0] = "ls";
	av[1] = "-l";
	av[2] = (char *)0;

	printf("esecuzione di ls: prima versione\n");
        execv("/bin/ls", av);

	printf("errore in execv\n");
	exit(1);
}



