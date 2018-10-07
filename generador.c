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
	int seed;
	int num;
	
	while( read(0, seed, sizeof(int)) != EOF )
	{
			num = rand(seed) % 10;
			write(1, num, sizeof(int) );
	}
	close(0);
	close(1);
	exit(0);
}
