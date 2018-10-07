/*----------------------------------------------------
PRA1 : [TODO]
Codi font : loteria.c

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

//Constants
#define NUM_GENERADORS (int)5

//prototypes
void generarFills(int pids[NUM_GENERADORS]);
void tancarFitxersPare(int fds[NUM_GENERADORS*2][2]);
void tancarFitxers(int fds[NUM_GENERADORS*2][2]);
void reubicarPipes(int i, int fds[NUM_GENERADORS*2][2]);
void generarSeeds(int pids[NUM_GENERADORS], int seeds[NUM_GENERADORS]);
void comunicarSeeds(int seeds[NUM_GENERADORS], int fds[NUM_GENERADORS*2][2];);
void llegirNum(int fds[NUM_GENERADORS*2][2]);


int main(int argc, char *argv[])
{
	if (argc != 2) {
	const char* cadena = "Nombre incorrecte d'arguments";
	write(0, cadena, strlen(cadena));
		exit(-1);
	}
	else{
		int pids[NUM_GENERADORS];
		int seeds[NUM_GENERADORS];
		int numero[NUM_GENERADORS];

		//crear pipes
		int fds[NUM_GENERADORS*2][2];		//fds[i][0] lectura		fds[i][1] escriptura
		int i;
		for(i = 0; i < 10; i++){
			if( (pipe(fds[i]) ){
				perror("Error al crear els pipes");
				exit(-1);
			}
		}
		
		generarFills(pids);

		tancarFitxersPare(fds);

		generarSeeds(pids, seeds);

		comunicarSeeds(seeds);

		//esperar joins
		wait();
		wait();
		wait();
		wait();
		wait();

		llegirNums(numero);

		imprimirNum(numero);

		exit(0);
	}
}
	
void generarFills(int pids[NUM_GENERADORS])
{
	int i;
	for(i = 0; i < NUM_GENERADORS; i++){

		pids[i] = fork();
		
		switch(pids[i]){
			case -1:
				//error
				perror("Error creació fill");
				exit(-1);
				break;
			case 0:
				//instruccions fill
				reubicarPipes(i, fds);
				//tancar fitxers no necessaris

				tancarFitxersFills(3, limit);
				//canviar execució programa
				execl("./generador","generador", NULL);
				exit(-1);
				break;
				
			default:

		}

	}
}

void tancarFitxersPare(int fds[NUM_GENERADORS*2][2])
{
	int i = 0;
	for(i; i <= NUM_GENERADORS; i=i+2){
		close(fds[i][0]);
		close(fds[i+1][1]);
	}
}
//Arreglar, els ha de tancar tots menys els que empra el fill
void tancarFitxers(int fds[NUM_GENERADORS*2][2])
{
	for(i; i < NUM_GENERADORS*2; i=i++) {
		close(fds[i][0]);
		close(fds[i][1]);
	}
}

void reubicarPipes(int i, fds[NUM_GENERADORS*2][2])
{
	//reubicar escriptura
	close(0);
	dup(fds[2*i][1]);
	//reubicar lectura
	close(1);
	dup(fds[2*i+1][0]);
}	

void generarSeeds(int pids[NUM_GENERADORS], int seeds[NUM_GENERADORS])
{
	int i;
	for(i = 0; i < NUM_GENERADORS; i++){
		seeds[i] = srand(pids[i]);
	}
}

void comunicarSeeds(int seeds[NUM_GENERADORS], int fds[NUM_GENERADORS*2][2];);
{
	int i;
	for(i = 0; i < NUM_GENERADORS; i++){
		write( fds[i*2+1][1], seeds[i], sizeof(int) );
	}
}

void llegirNum(int fds[NUM_GENERADORS*2][2])
{
	int i, numero, resultat = 0;
	for(i = 0; i < NUM_GENERADORS; i++){
		read(fds[2*i][0], numero, sizeof(int) );
		resultat = resultat*10 + numero;
	}
	write(0, resultat, sizeof(int));
}


