#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	char *argin[4];

	argin[0] ="myecho";
	argin[1] = "hello";
	argin[2] = "world";
	argin[3] = (char *)0;

	printf("esecuzione di myecho\n");
	execvp(argin[0], argin);

	printf("errore in execvp\n");
	exit(1);
}
