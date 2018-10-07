/*----------------------------------------------------
PRA1 : [TODO]
Codi font : generador.c

Joan Palau Oncins
Sergi Simón Balsells
----------------------------------------------------*/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include<time.h>

int main(int argc, char *argv[])
{
	char *seed;
	seed = (char*) malloc( sizeof(int) );
	int num;
	
	while( read(0, *seed, sizeof(int)) != EOF )
	{
		if(*seed == "SIGTERM"){
			//lliberar memoria i acabar procés
			free(seed);
			exit(-1);
		}
		else{
			num = rand(seed) % 10;
			write(1, num, sizeof(int) );
		}
	}
	
	//lliberar memoria i acabar procés
	free(seed);
	exit(0);
}
