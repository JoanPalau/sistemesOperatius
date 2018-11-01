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
void guardian(int n, char* msg);
void guardian1(__sighandler_t sig, char* msg);

#define COLOR "\x1b[33m"
#define RESET "\x1b[0m"
/* Declare global variables */
int pids[NUM_GENERADORS];
int fds[NUM_GENERADORS*2][2];

int main(int argc, char *argv[])
{
	if (argc != 2) {
		const char* cadena = "Nombre incorrecte d'arguments\n";
		guardian(write(0, cadena, strlen(cadena)), "Error en escriure a" 
						"stdout per falta d'arguments");
		exit(-1);
	}
	else{	
		init(argv); /* Initializes all variables */
		

		while(1) {
			pause(); /* Waits or a signal to kill the process, or 
				  * handle another lotto. Also, pause sets errno
				  * to EINTR (interrupted system call), which 
				  * can be problematic in an unexpected perror 
				  * function call         		      */

		}
	}
	return -1; 
	/* Being here is an error, so i return -1*/
}

void lotto(int sig) 
{
	char buff[255] = "\n(S'ha pitjat Ctrl+4)\n";
	write(1, buff, strlen(buff));
	generarSeeds();
	llegirNum();
	guardian1(signal(SIGQUIT, lotto), "Reinstalling sigquit lotto()");
	/*sprintf(buff, "\t%s[%d] Subscrivint%s\n", COLOR,getpid(), RESET);
	guardian(write(0, buff, strlen(buff)), "Error en escriure" 
					       " subscrivint debug");
	* Debugging commented */
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
	guardian1(signal(SIGQUIT, lotto), "error sigquit initSignals");
	guardian1(signal(SIGINT, end), "error sigquit initSignals");
}
void end(int sig) 
{
	finalitzarPipes();
	closeChilds();
	exit(0);
}


void initsrand(char *argv[]) 
{
	/*int *num;
	num = (int *) malloc(sizeof(int));*/
	int num;
	sscanf(argv[1], "%d", &num);
	srand(num);
	/*free(num);*/
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
				perror("Error en executar fill");
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
	/*Clono stdout a 40 per a fer writes del nombre enviat per debugging
	dup2(1,40);*/
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
	/* char buff[256]; */
	for(i = 0; i < NUM_GENERADORS; i++){
		int num = rand();
		guardian(write(fds[2*i+1][1], (const void *) &num, sizeof(int)),
				"Error en escriure en pipe generarSeeds()");
		/* sprintf(buff, "\t%s[%d] Seed Number: %d %s\n", 
			COLOR, i, num, RESET);
		guardian(write(1, buff, strlen(buff)), 
			"Error en escriure debug generarSeeds()");
		* Debugged commented*/
	}
}

void llegirNum()
{
	int i, numero, resultat = 0;
	/* char buff[255]; */
	char *msg = (char*) malloc(sizeof(char)*(NUM_GENERADORS+1));
	for(i = 0; i < NUM_GENERADORS; i++){
		/*sprintf(buff, "\t%s[%d] Llegint fill%s\n", COLOR, i, RESET);
		guardian(write(1, buff, strlen(buff)), 
			"Error en escriure llegirNum() debug");
		* Debugging commented */
		guardian(read(fds[2*i][0], (void *) &numero, sizeof(int)), 
			"Error lectura en llegirNum()");
		resultat = resultat*10 + numero;
	}
	sprintf(msg, "Taula>Número premiat: %d\n", resultat);
	guardian(write(1, msg, strlen(msg)), 
		"Error en escriure nombre de loteria llegirNum()"); 
}

void closeChilds() 
{
	/* char buff[256];
	sprintf(buff, "\t%s[%d] Closing childs %s\n", COLOR, getpid(), RESET);
	guardian(write(1, buff, strlen(buff)), 
		"Error en escriure closeChilds() debugg");
	* Debugging commented */
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


