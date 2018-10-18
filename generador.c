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
	int *seed;
	int num;
	signal(SIGTERM, end);
	while(1)
	{
		seed = malloc(sizeof(int));
		read(0, (void *) seed, sizeof(int));
		srand(*seed);
		num = rand() % 10;
		sprintf(seed, "%d\n", num);
		write(1, (void *) seed, strlen(seed) );
		free(seed);	
	}
	return -1;
}

void end(int sig) {
	close(0);
	close(1);
	exit(0);
}
