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
void guardian1(__sighandler_t sig, char* msg);
void guardian(int n, char* msg);

int main(int argc, char *argv[])
{
	guardian1(signal(SIGTERM, end), "SIGTERM");
	guardian1(signal(SIGQUIT, mansigquit), "SIGQUIT");
	while(1)
	{
		pause();
		/* Due to internal kernel errors in my kernel
		 * (is up to date), and discussed before with
		 * the professor, we handle the read with the
		 * SIGQUIT propagate */
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
	/*char buff[256];*/

	guardian(read(0, (void *) &seed, sizeof(int)), "Error de lectura");
	srand(seed);
	num = rand() % 10;
	guardian(write(1, (void *) &num, sizeof(int)), 
			"Error en escritura en el pipe");

	/* sprintf(buff, "\t%s[%d] Seed: %d Number: %d %s\n", 
		COLOR, getpid(), seed, num, RESET);
	write(2, buff, strlen(buff)); 
	* Debugging commented*/

	guardian1(signal(SIGQUIT, mansigquit), "SIGQUIT in mansigquit");
}

void guardian(int n, char* msg) {
	if(n<0){
		perror(msg);
		exit(-1);
	}
}

void guardian1(__sighandler_t sig, char* msg){
	if(sig==SIG_ERR){
		perror(msg);
		exit(-1);
	}
}
