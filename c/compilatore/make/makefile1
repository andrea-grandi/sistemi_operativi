CC=gcc
CFLAGS=-Wall -std=c11

all: main


main.o: main.c func.h
	$(CC) $(CFLAGS) -c main.c 

func.o: func.c func.h
	$(CC) $(CFLAGS) -c func.c

main: main.o func.o
	$(CC) $(CFLAGS) -o main main.o func.o

clean: 
	rm -rf *.o
