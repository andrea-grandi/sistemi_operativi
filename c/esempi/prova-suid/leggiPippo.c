#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv){

	int fd;
	char c;

	printf("real-user id = %d\n", getuid());
	printf("effective-user id = %d\n", geteuid());
	printf("SONO %s\n", argv[0]);
	puts("sto per leggere il file pippo");

	if ((fd = open("pippo", O_RDONLY)) < 0){
		puts("errore in apertura\n");
		exit(1);
	}

	while(read(fd, &c, 1)){
		write(1, &c, 1);
	}

	exit(0);
}
