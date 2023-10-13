CC=gcc
CFLAGS=-Wall -std=c11

all: copia

copia.o: copia.c
	$(CC) $(CFLAGS) -c copia.c 

main: copia.o
	$(CC) $(CFLAGS) -o main copia.o

clean: 
	rm -rf *.o
