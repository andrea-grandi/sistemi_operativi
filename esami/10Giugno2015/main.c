#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/stat.h>

typedef int pipe_t[2];

typedef struct{
	int id;
	int occ;
}s;

int main(int argc, char **argv){

	int pid;
	int pidFiglio;
	int ritorno = 0;
	int status;
	int i, j;
	int N;
	int H;
	int fd;
	int nr, nw;
	char ch;
	char Cx;
	char linea[255];
	pipe_t *piped;
	s cur;
	s pip;
	int cont;

	if(argc < 4){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	/*controllo ultimo parametro*/
	H = atoi(argv[argc - 1]);
	if(H <= 0){
		printf("errore nell'ultimo parametro\n");
		exit(2);
	}

	N = argc - 2;

	printf("inserire un carattere Cx che verra' cercato dei figli in ogni linea: \n");
	read(0, &Cx, 1);

	printf("il carattere letto e' : %c\n", Cx);

	/*allochiamo memoria per la pipe*/
	piped = (pipe_t *)malloc(N*sizeof(pipe_t));
	if(piped == NULL){
		printf("errore nella malloc\n");
		exit(3);
	}

	/*creiamo la pipe*/
	for(i = 0; i < N; i++){
		if(pipe(piped[i]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(4);
		}
	}

	/*creiamo i figli*/
	for(i = 0; i < N; i++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(5);
		}
		if(pid == 0){
			/*codice figlio*/
			/*chiudiamo i lati della pipe che non ci interessano*/
			for(j = 0; j < N; j++){
				if(j != i){
					close(piped[j][1]);
				}
				if((i == 0) || (j != i-1)){
					close(piped[j][0]);
				}
			}

			/*apriamo il file associato*/
			fd = open(argv[i+1], O_RDONLY);
			if(fd < 0){
				printf("errore in apertura file\n");
				exit(-1);
			}
			
			j=0;
			while(read(fd, &ch, 1)){
				if(ch == Cx){
					/*trovata occorrenza*/
					cur.occ++;
				}else{
					if(ch == '\n'){
						/*ho letto una linea*/
						if(i != 0){
							/*non siamo nel primo figlio quindi inviamo in avanti la struct*/
							nr = read(piped[i][0], &pip, sizeof(s));
							if(nr != sizeof(s)){
								printf("errore nella read\n");
								exit(-1);
							}
							if(cur.occ >= pip.occ){
								cur.id = pip.id;
								cur.occ = pip.occ;
							}
						}
						nw = write(piped[i][1], &cur, sizeof(s));
						if(nw != sizeof(s)){
							printf("errore nella write\n");
							exit(-1);
						}

						cur.occ = 0;
						cur.id = 0;
					}
				}
			}
			exit(0);
		}
	}
	/*codice padre*/


	exit(0);
}
