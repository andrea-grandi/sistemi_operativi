#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int count;

int main(){

	int piped[2];
	char c = 'x';

	if (pipe(piped) < 0){
		printf("errore\n");
		exit(1);
	}

	for(count=0;;){
		write(piped[1], &c, 1);

		if((++count % 1024) == 0){
			printf("%d caratteri nella pipe\n", count);
		}
	}

	exit(0);
}
