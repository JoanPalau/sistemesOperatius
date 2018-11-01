#compiler
CC=gcc
CFLAGS=-pedantic -Wall

TARGETS=loteria generador codif

all: $(TARGETS)
	
loteria: loteria.c
	$(CC) $(CFLAGS) -o $@ $^
	
generador: generador.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o $(TARGETS)
