/*----------------------------------------------------
PRA1 : [TODO]
Codi font : generador.c

Joan Palau Oncins
Sergi Simón Balcells
----------------------------------------------------*/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include<time.h>

void end(int sig);

int main(int argc, char *argv[])
{
	int seed;
	int num;
	signal(SIGTERM, end);
	while(1)
	{
		read(0, (void *) &seed, sizeof(int));
		srand(seed);
		num = rand() % 10;
		write(1, (void *) &num, sizeof(int) );
	}
	return -1;
}

void end(int sig) {
	close(0);
	close(1);
	exit(0);
}
