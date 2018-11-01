#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#define      SEGONSFILL   (int)5 
char *color_blue   = "\033[01;34m";
char *color_red    = "\033[01;31m";
char *color_end    = "\033[00m";
 
int    main()
{
	int     estat,i, exitcode;
	/* unsigned int llavor; *
	 *char    *s; *
	 * unused variables commented  */
	pid_t   pid,fpid;
	char   cadena[100];
	 
	sprintf(cadena, "\n%s[%d] ********** Iniciant PROCES des de %d **********%s\n", color_blue, getpid(), getppid(), color_end);
	if((write(1, cadena, strlen(cadena)))==-1)
	perror("ERROR: write 1");
	 
	pid = fork();
	 
	if(pid==-1)
	{
		/*BLOC 3*/
		sprintf(cadena, "%s[%d]  Error  en  l'execució  del  fork.%s\n",
			color_blue, getpid(), color_end);
		write(2, cadena /*s*/, strlen(cadena /*s*/));
		/* Even thought it's sprintf in "cadena", in question it was
		 * with the "s". It was changed to */
		exit(0);
	}
	else if(pid==0)
	{
		/* * * * * * * * * * BLOC 2 * * * * * * * * * */
		sprintf(cadena, "\t%s[%d] ***** Iniciant proces fill de %d ... ***** %s\n", color_red, getpid(), getppid(), color_end);
		write(1, cadena, strlen(cadena));
		/*llavor = (unsigned int) time(NULL);
		sprintf(cadena,  "\t%s[%d]  Estableix  llavor  [%d]%s\n",  color_red,  getpid(), llavor, color_end);
		write(1, cadena, strlen(cadena));
		srand(llavor); //Establiment de la llavor, diferent en cada execució per assegurar seqüències diferents.*/
		 
		for(i=0;i<SEGONSFILL;i++)
		{
			sleep(1);
			sprintf(cadena, 
				"\t%s[%d]  El  proces  fill  acabara  en  %d  segons%s\n", 
				color_red, getpid(), SEGONSFILL-i, color_end);
			write(1, cadena, strlen(cadena));
		}
		 
		exitcode = rand()%10;     // Genera un nombre aleatori entre 0 i 9. sprintf(cadena, "\t%s[%d] El proces fill ha finalitzat retornant amb codi de finalitzacio [%d]!%s\n", color_red, getpid(), exitcode, color_end);
		write(1, cadena, strlen(cadena));
		exit(exitcode);
	}
	else
	{
		/* * * * * * *  * *BLOC 1 * * * * * * * * * * * * * */
		sprintf(cadena,  "%s[%d]  PROCES  PARE  suspen  execucio  en  espera  de  la finalitzacio del fill %d ... %s\n", color_blue, getpid(), pid, color_end); write(1, cadena, strlen(cadena));
 
		if((fpid=wait(&estat)) == -1)
		{
			sprintf(cadena, "%s[%d] PROCES PARE no te fills ... %s\n", color_blue,
				getpid(), color_end);
			write(1, cadena, strlen(cadena));
			exit(0);
		}
		 
		sprintf(cadena, "%s[%d] PROCES PARE ha rebut acabament proces fill [%d] amb codi [%d] i acaba. %s\n\n", color_blue, getpid(), fpid, WEXITSTATUS(estat), color_end);
		write(1, cadena, strlen(cadena));
		 
		exit(0);


	}
	return 0;
}






