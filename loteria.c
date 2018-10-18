/*----------------------------------------------------
PRA1 : [TODO]
Codi font : loteria.c

Joan Palau Oncins
Sergi Simón Balcells
----------------------------------------------------*/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<signal.h>
#include<sys/wait.h>
#include<time.h>
#include<string.h>

/* Constants */
#define NUM_GENERADORS 5

/* prototypes */
void generarFills();
void tancarFitxersPare();
void tancarFitxers();
void reubicarPipes(int i);
void generarSeeds();
void llegirNum();
void closeChilds();
void finalitzarPipes ();
void init(char *argv[]);
void end(int sig);
void initsrand(char *argv[]);
void initPipes();
void lotto(int sig);
void initSignals();
void killChild();
void waitChild();

/* Declare global variables */
int pids[NUM_GENERADORS];
int fds[NUM_GENERADORS*2][2];

int main(int argc, char *argv[])
{
	if (argc != 2) {
		const char* cadena = "Nombre incorrecte d'arguments\n";
		write(0, cadena, strlen(cadena));
		exit(-1);
	}
	else{	
		init(argv); /* Initializes all variables */
		printf("Initialitzated all variables. Waiting for signal\n");


		while(1) {
			printf("\n");
			pause(); /* Waits or a signal to kill the process, or handle another lotto */
		}
	}
	return -1; /* Si arriba aqui el main vol dir que hi ha un error, ergo -1*/
}

void lotto(int sig) 
{
	generarSeeds();
	llegirNum();
}

void init(char *argv[]) 
{
	initsrand(argv);
	initPipes();		
	generarFills();
	tancarFitxersPare();
	initSignals();	
}

void initSignals() {
	signal(SIGQUIT, lotto);
	signal(SIGINT, end);
}
void end(int sig) 
{
	finalitzarPipes();
	closeChilds();
	exit(0);
}


void initsrand(char *argv[]) 
{
	int *num;
	num = (int *) malloc(sizeof(int));
	sscanf(argv[1], "%d", num);
	srand(*num);
	free(num);
}

void initPipes() 
{
	/* crear pipes */
	/* fds[i][0] lectura		fds[i][1] escriptura */
	int i;
	for(i = 0; i < NUM_GENERADORS*2; i++){
		if( pipe(fds[i]) ){
			perror("Error al crear els pipes\n");
			exit(-1);
		}
	}
}
	
void generarFills()
{
	int i;
	for(i = 0; i < NUM_GENERADORS; i++){

		pids[i] = fork();
		
		switch(pids[i]){
			case -1:
				/* error */
				perror("Error creació fill\n");
				exit(-1);
				break;
			case 0:
				/* instruccions fill */
				reubicarPipes(i);
				/* tancar fitxers no necessaris */

				tancarFitxers();
				/* canviar execució programa */
				execl("./generador","generador", NULL);
				exit(-1);
				break;
		}
	}
}

void tancarFitxersPare()
{
	int i;
	for(i = 0; i < NUM_GENERADORS; i++){
		close(fds[2*i][1]);
		close(fds[2*i+1][0]);
	}
}

void finalitzarPipes ()
{
	int i;
	for(i = 0; i < NUM_GENERADORS; i++){
		close(fds[2*i][0]);
		close(fds[2*i+1][1]);
	}
}

/* Arreglar, els ha de tancar tots menys els que empra el fill */
void tancarFitxers()
{
	int i;
	for(i = 0; i < NUM_GENERADORS*2; i++) {
		close(fds[i][0]);
		close(fds[i][1]);
	}
}

void reubicarPipes(int i)
{
	/* reubicar escriptura */
	close(1);
	dup(fds[2*i][1]);
	/* reubicar lectura */
	close(0);
	dup(fds[2*i+1][0]);
}	

void generarSeeds()
{
	int i;
	for(i = 0; i < NUM_GENERADORS; i++){
		int num = rand();
		write( fds[i*2+1][1], (const void *) &num, sizeof(int) );

	}
}

void llegirNum()
{
	int i, numero, resultat = 0;
	char *msg = (char*) malloc(sizeof(char)*NUM_GENERADORS);
		for(i = 0; i < NUM_GENERADORS; i++){
		read(fds[2*i][0], (void *) &numero, sizeof(int) );
		resultat = resultat*10 + numero;
	}
	sprintf(msg, "%d", resultat);
	write(0, msg, sizeof(int));
}

void closeChilds() 
{
	killChild();
	waitChild();
}

void waitChild() {
	int i;
	for (i = 0; i<NUM_GENERADORS; i++) {
		wait((int *) NULL);
	}
}

void killChild() {
	int i;
	for (i = 0; i<NUM_GENERADORS; i++) {
		kill(pids[i], SIGTERM);
	}

}
