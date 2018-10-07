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
void generar fills(int* pids);
void tancarFitxersPare(int i, int limit);
void tancarFitxersFills(int i, int limit);
void reubicarPipes(int i);
void generarSeeds(int* pids, int* seeds);
void comunicarSeeds(int* seeds);
void llegirNum(int* numero);
void imprimirNum(int* numero);


int main(int argc, char *argv[])
{
	if (argc != 2){
	const char* cadena = "Nombre incorrecte d'arguments";
	write(0, cadena, strlen(cadena));
		exit(-1);
	}
	else{
		int pids[5];
		int seeds[5];
		int numero[5];

		//crear pipes
		int fds[10][2];		//fds[i][0] lectura		fds[i][1] escriptura
		int i;
		for(i = 0; i < 10; i++){
			if( (pipe(fds[i]) ){
				perror("Error al crear els pipes");
				exit(-1);
			}
		}
		
		generarFills(pids);

		tancarFitxersPare(4, limit);

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
	
void generarFills(int* pids)
{
	int i;
	int limit = 4;
	for(i = 0; i < NUM_GENERADORS; i++){

		*pids = fork();
		
		switch( *pids ){
			case -1:
				//error
				perror("Error creació fill");
				exit(-1);
			case 0:
				//instruccions fill
				reubicarPipes(limit);
				//tancar fitxers no necessaris
				limit = limit - 1;
				tancarFitxersFills(3, limit);
				//canviar execució programa
				execl("./generador","generador", NULL);
				exit(-1);
				
			default:
				//instruccions pare
			
				limit = limit + 4;
		}
		pids++;
	}
}

void tancarFitxersPare(int i, int limit)
{
	for(i; i <= limit; i=i+4){
		close(i);
		close(i+1);
}
//Arreglar, els ha de tancar tots menys els que empra el fill
void tancarFitxersFills(int i, int limit)
{
		for(i; i < limit; i=i+4){
		close(i);
		close(i+3);
}

void reubicarPipes(int i)
{
	//reubicar escriptura
	close(0);
	dup(i+1);
	//reubicar lectura
	close(1);
	dup(i);
}	

void generarSeeds(int* pids, int* seeds)
{
	int i;
	for(i = 0; i < NUM_GENERADORS; i++){
		*seeds = srand(*pids);
		seeds++;
		pids++;
	}
}

void comunicarSeeds(int* seeds)
{
	int i;
	for(i = 6; i <= 22; i = i + 4){
		write( i, seeds, sizeof(int) );
		seeds++;
	}
}

void llegirNums(int* numero)
{
	int i;
	for(i = 3; i <= 19; i = i + 4){
		read(i, *numero, sizeof(int) );
		numero++;
	}
}

void imprimirNum(int* numero)
{
	int i;
	for(i = 0; i < NUM_GENERADORS; i++){
		write(1, *numero, sizeof(int) );
		numero++;
	}
	write(1,"\n", sizeof("\n"));
}

