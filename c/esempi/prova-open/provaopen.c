#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	int fd1, fd2, fd3;

	if (argc != 2) {
       		puts("Errore nel numero dei parametri");
		exit(1);
	}

	if ((fd1 = open(argv[1], O_RDONLY)) < 0){
		puts("Errore in apertura file");
		exit(2);
	}
	else {
		printf("Valore di fd1 = %d\n", fd1);
	}

	if ((fd2 = open(argv[1], O_RDONLY)) < 0){
		puts("Errore in apertura file");
		exit(2);
	}
	else {
		printf("Valore di fd2 = %d\n", fd2);
	}
	
	close (fd1);

	if((fd3 = open(argv[1], O_RDONLY)) < 0) {
		puts("Errore in apertura file");
		exit(2);
	}
	else {
		printf("Valore di fd3 = %d\n", fd3);
	}

	return 0;

}

	
