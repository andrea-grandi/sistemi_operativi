#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctype.h>

#define PERM 0644

typedef int pipe_t[2];

int main(int argc, char **argv){
	int *pid;
	int pidFiglio, ritorno, status;
	int i, j;
	pipe_t *piped;
	char ch;
	int fd;
	int B;
	int L;
	int q;
	char *file;
	char *buff;
	int fdw;

	if(argc != 4){
		printf("errore nel numero di parametri\n");
		exit (1);
	}
	
	if((L = atoi(argv[2])) <= 0){
		printf("errore nel parametro L\n");
		exit(2);
	}

	if((B = atoi(argv[3])) <= 0){
		printf("errore nel parametro B\n");
		exit(3);
	}

	/*allochiamo memoria per la pipe*/
	piped = (pipe_t *)malloc(B*sizeof(pipe_t));
	if(piped == NULL){
		printf("errore nella fork\n");
		exit(4);
	}

	/*allochiamo la memoria per i pid*/
	pid = (int *)malloc(B*sizeof(int));
	if(pid == NULL){
		printf("errore nella malloc\n");
		exit(5);
	}

	/*creiamo la pipe*/
	for (i = 0; i < B; i++){
		if(pipe(piped[i]) != 0){
			printf("errore nella creazione della pipe\n");
			exit(6);
		}
	}

	/*allochiamo la memoria per il file*/
	file = (char *)malloc((strlen(argv[1])+1+strlen("Chiara")+1)*sizeof(char));
	if (file == NULL){
		printf("errore nella malloc\n");
		exit(7);
	}

	sprintf(file, "%s.Chiara", argv[1]);

	/*apriamo il file*/
	if((fdw = open(file, O_WRONLY, PERM)) < 0){
		printf("errore in apertura file\n");
		exit(8);
	}

	/*creazione figli*/
	for(q = 0; q < B; q++){
		if((pid[q] = fork()) < 0){
			printf("errore nella fork\n");
			exit(9);
		}
		if(pid[q] == 0){
			/*codice figlio*/
			/*chiudiamo i lati della pipe*/
			for(j = 0; j < B; j++){
				close(piped[j][0]);
				if(j != q){
					close(piped[j][1]);
				}
			}
			/*il figlio deve aprire il file in lettura*/
			if((fd = open(argv[1], O_RDONLY)) < 0){
				printf("errore nella open\n");
				exit(-1);
			}

			buff = (char *)malloc(L/B*sizeof(char));
			if(buff == NULL){
				printf("errore nella malloc\n");
				exit(-1);
			}

			lseek(fd, (long)q*L/B, SEEK_SET);
			read(fd, buff, L/B);
			write(piped[q][1], &buff[0], 1);
			exit(L/B);
		}
	}
	/*codice padre*/
	/*chiudiamo i lati della pipe*/
	for(j = 0; j < B; j++){
		close(piped[j][1]);
	}

	/*leggiamo dalla pipe*/
	for(q = 0; q < B; q++){
		read(piped[q][0], &ch, 1);
		write(fdw, &ch, 1);
	}

	/*attesa figli*/
	for(i = 0; i < B; i++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(10);
		}
		if((status & 0xFF) != 0){
			printf("il figlio con pid %d ha terminato in modo anomalo\n", pidFiglio);
		}else{
			ritorno = (int)((status >> 8) & 0xFF);
			printf("il figlio con pid %d ha ritornato %d\n", pidFiglio, ritorno);
		}
	}
	exit(0);
}
