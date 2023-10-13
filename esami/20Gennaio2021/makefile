CC=gcc
CFLAG=-Wall
SRC = $(wildcard *.c)
TAR = $(SRC:.c=)

all: $(TAR)

%: %.c
	$(CC) $(CFLAG) $< -o $@
