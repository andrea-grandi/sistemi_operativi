#include <stdio.h>
#include <stdlib.h>
#include <fcnt1.h>

int main(int argc, char **argv){
	int i=0, fd;

	if (argc != 2) {
		puts("Errore nel numero di parametri");
		exit(1);
	}

	while (1){
		if ((fd = open(argv[1], O_RDONLY)) < 0){
			puts("Errore in apertura file");
			printf("Valore di i = %d\n", i);
			exit(2);
		}
		else {
			i++;
		}
	}

		return 0;
}
