#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

typedef int pipe_t[2];

int main(int argc, char **argv){
	
	int pid;
	int pidFiglio, ritorno, status;
	int fd;
	pipe_t pipe_fp[2];
	pipe_t pipe_pf[2];
	int i, j;
	char linea[250];
	int L;
	int nw, nr;
	char ok;
	int L1, L2;
	int n_linee;
	
	if(argc != 3){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	/*creiamo le pipe*/
	for(i = 0; i < 2; i++){
		if(pipe(pipe_pf[i]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(3);
		}
		if(pipe(pipe_fp[i]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(4);
		}
	}

	/*creiamo i due figli*/
	for (i = 0; i < 2; i++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(5);
		}
		if(pid == 0){
			/*codice figlio*/
			/*chiudiamo i lati della pipe*/
			for(j = 0; j < 2; j++){
				close(pipe_fp[j][0]);
				close(pipe_pf[j][1]);
				if(i != j){
					close(pipe_fp[j][1]);
					close(pipe_pf[j][0]);
				}
			}
			/*apro il file associato*/
			fd = open(argv[i+1], O_RDONLY);
			if(fd < 0){
				printf("errore nella open\n");
				exit(-1);
			}
			L = 0;
			while(read(fd, &(linea[L]), 1)){
				if(linea[L] == '\n'){
					L++;
					nw = write(pipe_fp[i][1], &L, sizeof(L));
					if(nw != sizeof(L)){
						printf("errore nella write\n");
						exit(-1);
					}
					nr = read(pipe_pf[i][0], &ok, sizeof(ok));
					if(nr != sizeof(ok)){
						printf("errore nella read\n");
						exit(-1);
					}
					if(ok == 's'){
						write(pipe_fp[i][1], linea, L);
						ritorno++;
					}
				}else{
					L++;
				}
			}
			exit(ritorno);
		}
	}
	/*codice padre*/
	/*chiudiamo i lati della pipe*/
	for(i = 0; i < 2; i++){
		close(pipe_pf[i][0]);
		close(pipe_fp[i][1]);
	}

	while(read(pipe_fp[0][0], &L1, sizeof(L1))){
		n_linee++;
		nr = read(pipe_fp[i][0], &L2, sizeof(L2));
		if(nr != sizeof(L2)){
			printf("errore nella read\n");
			exit(7);
		}
		if(L1 == L2){
			ok = 's';
		}else{
			ok = 'n';
		}
		for(i = 0; i < 2; i++){
			if((nw = write(pipe_pf[i][1], &ok, sizeof(ok))) != sizeof(ok)){
				printf("errore nella write\n");
				exit(8);
			}
		}
		if(ok == 's'){
			printf("sono il padre e ho ricevuto per la linea %d la seguente info\n", n_linee);
			read(pipe_fp[0][0], linea, L1);
			linea[L1 - 1] = '\0';
			printf("%s\n", linea);
			printf("metre ho ricevuto la seguente info del secondo figlio\n");
			read(pipe_fp[1][0], linea, L1);
			linea[L1-1] = '\0';
			printf("%s\n", linea);
		}else{
			printf("la linea numero %d risulta essere di lunghezza diversa nel primo e nel secondo file\n", n_linee);
		}
	}

	/*il padre aspetta i figli*/
	for(i = 0; i < 2; i ++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(9);
		}
		if((ritorno & 0xFF) != 0){
			printf("il figlio con pid %d ha terminato in modo anomalo\n", pidFiglio);
		}else{
			ritorno = (int)((status >> 8) & 0xFF);
			printf("il figlio con pid %d ha ritornato %d\n", pidFiglio, ritorno);
		}
	}
	exit(0);
}
