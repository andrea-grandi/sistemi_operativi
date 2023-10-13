#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv){
	int i;

	printf("sono myecho\n");
	for(i = 0; i < argc; i++){
		printf("argomento argv[%d] = %s\n", i, argv[i]);
	}
	exit(0);
}
