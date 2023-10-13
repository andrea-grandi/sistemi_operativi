#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	printf("esecuzione di ls: seconda versione\n");
	execl("/bin/ls", "ls", "-l", (char *)0);

	printf("errore in execl\n");
	exit(1);
}
