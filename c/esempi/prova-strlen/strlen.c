#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char **argv){
	int fd;
	char c;

	if (argc != 3) {
		puts("Errore nen numero dei parametri");
		exit (1);
	}

	if ((fd = open(argv[1], O_RDWR)) < 0){
		puts("Errore in apertura del file");
		exit (2);
		}

	if (strlen(argv[2]) != 1){
		puts("Errore non carattere");
			exit (3);
	}

	while(read(fd, &c, 1)){
	       if (c == argv[2][0]){
			lseek(fd, -1L, 1);
	 		write(fd, " ", 1);
		}
	}

	return 0;
}

			

