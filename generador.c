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

#include<string.h> /* Testos */

#define COLOR "\x1b[32m"
#define RESET "\x1b[0m"

void end(int sig);
void mansigquit(int sig);

int main(int argc, char *argv[])
{
	signal(SIGTERM, end);
	signal(SIGQUIT, mansigquit);
	while(1)
	{
		pause();
	}
	return -1;
}

void end(int sig) {
	close(0);
	close(1);
	exit(0);
}

void mansigquit(int sig){
	int seed;
	int num;
	char buff[256];

		if(read(0, (void *) &seed, sizeof(int))>=0) {
			srand(seed);
			num = rand() % 10;
			if(write(1, (void *) &num, sizeof(int))<0) {
				perror("Error en escritura en el pipe");
			};
			/* sprintf(buff, "\t%s[%d] Seed: %d Number: %d %s\n", 
				COLOR, getpid(), seed, num, RESET);
			write(2, buff, strlen(buff)); 
			* Debugging commented*/
		} else {
			perror("Error de lectura");
		}
	signal(SIGQUIT, mansigquit);
}
