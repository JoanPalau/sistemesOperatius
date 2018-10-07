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
#include<string.h>

//Constants
#define NUM_GENERADORS (int)5

//prototypes
void generarFills(int pids[NUM_GENERADORS], int fds[NUM_GENERADORS*2][2]);
void tancarFitxersPare(int fds[NUM_GENERADORS*2][2]);
void tancarFitxers(int fds[NUM_GENERADORS*2][2]);
void reubicarPipes(int i, int fds[NUM_GENERADORS*2][2]);
void generarSeeds(int seeds[NUM_GENERADORS]);
void comunicarSeeds(int seeds[NUM_GENERADORS], int fds[NUM_GENERADORS*2][2];);
void llegirNum(int fds[NUM_GENERADORS*2][2]);
void closeChilds();
void finalitzarPipes (int fds[NUM_GENERADORS*2][2]);
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

		//crear pipes
		int fds[NUM_GENERADORS*2][2];		//fds[i][0] lectura		fds[i][1] escriptura
		int i;
		int *num;
		num = (int *) malloc(sizeof(int));
		sscanf(argv[1], "%d", num);
		srand(*num);
		free(num);
		for(i = 0; i < 10; i++){
			if( pipe(fds[i]) ){
				perror("Error al crear els pipes");
				exit(-1);
			}
		}

		/* Posar bucle per infinit amb tractament de senyals*/
		
		generarFills(pids, fds);

		tancarFitxersPare(fds);

		generarSeeds(seeds);

		comunicarSeeds(seeds);

		llegirNum(fds);

		/* Finalització del bucle infinit */

		finalitzarPipes(fds);

		closeChilds();

		exit(0);
	}
}
	
void generarFills(int pids[NUM_GENERADORS], int fds[NUM_GENERADORS*2][2])
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

				tancarFitxers(fds);
				//canviar execució programa
				execl("./generador","generador", NULL);
				exit(-1);
				break;
		}
	}
}

void tancarFitxersPare(int fds[NUM_GENERADORS*2][2])
{
	int i = 0;
	for(i; i < NUM_GENERADORS; i++){
		close(fds[2*i][1]);
		close(fds[2*i+1][0]);
	}
}

void finalitzarPipes (int fds[NUM_GENERADORS*2][2])
{
	int i = 0;
	for(i; i < NUM_GENERADORS; i++){
		close(fds[2*i][0]);
		close(fds[2*i+1][1]);
	}
}

//Arreglar, els ha de tancar tots menys els que empra el fill
void tancarFitxers(int fds[NUM_GENERADORS*2][2])
{
	int i;
	for(i; i < NUM_GENERADORS*2; i=i++) {
		close(fds[i][0]);
		close(fds[i][1]);
	}
}

void reubicarPipes(int i, int fds[NUM_GENERADORS*2][2])
{
	//reubicar escriptura
	close(0);
	dup(fds[2*i][1]);
	//reubicar lectura
	close(1);
	dup(fds[2*i+1][0]);
}	

void generarSeeds(int seeds[NUM_GENERADORS])
{
	int i;
	for(i = 0; i < NUM_GENERADORS; i++){
		seeds[i] = rand();
	}
}

void comunicarSeeds(int seeds[NUM_GENERADORS], int fds[NUM_GENERADORS*2][2])
{
	int i;
	for(i = 0; i < NUM_GENERADORS; i++){
		write( fds[i*2+1][1], (const void *) &seeds[i], sizeof(int) );
	}
}

void llegirNum(int fds[NUM_GENERADORS*2][2])
{
	int i, numero, resultat = 0;
	for(i = 0; i < NUM_GENERADORS; i++){
		read(fds[2*i][0], (void *) &numero, sizeof(int) );
		resultat = resultat*10 + numero;
	}
	write(0, (const void *) &resultat, sizeof(int));
}

void closeChilds() 
{
	int i;
	for (i=0; i<NUM_GENERADORS; i++) {
		wait((int *) NULL);
	}
}


