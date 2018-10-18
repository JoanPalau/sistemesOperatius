loteria: loteria.c
	$gcc -pedantic -ansi  -Wall -o loteria loteria.c
generador: generador.c
	$gcc -pedantic -ansi  -Wall -o generador generador.c
all: loteria.c generador.c
	$gcc -pedantic -ansi  -Wall -o generador generador.c
	$gcc -pedantic -ansi  -Wall -o loteria loteria.c


