#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	printf("UNO\n");

	fork();

	printf("DUE\n");
	
	fork();

	printf("TRE\n");
	
	exit(0);
}

