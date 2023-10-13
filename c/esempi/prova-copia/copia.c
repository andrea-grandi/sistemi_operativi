#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define PERM 0644 /* in UNIX */

int copyfile(char *f1, char *f2){
	int infile, outfile, nread;
	char buffer [BUFSIZ]; /* usato per i caratteri */

	if((infile = open(f1, O_RDONLY)) < 0) {
		return 2; 
		/* ERRORE se non si riesce ad aprire in lettura il primo file */
	}

	if((outfile = creat(f2, PERM)) < 0) {
		close(infile);
		return 3;
		/* ERRORE se non si riesce a creare il secondo file */
	}

	while((nread = read(infile, buffer, BUFSIZ)) > 0) {
		if(write(outfile, buffer, nread) < nread) {
			close(infile);
			close(outfile);
			return 4;
			/* ERRORE se non si riesce a scrivere */
		}
	}
	close(infile);
	close(outfile);
	return 0;
}

int main(int argc, char **argv){
	
	int status;

	if(argc != 3){
		printf("Errore: numero dei parametri sbagliato\n");
		printf("Ci vogliono 2 argomenti: nome-file-sorgente e nome-file-destinazione\n");
		exit(1);
	}

	status = copyfile(argv[1], argv[2]);

	if(status != 0){
		printf("Ci sono stati degli errori durante la copia\n");
		return status;
	}
}
