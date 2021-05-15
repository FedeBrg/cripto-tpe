all: gal lag bmp main

main: app.c galois/galois.o lagrange/lagrange.o manejo_bmp/bmp_handling.o
	gcc app.c manejo_bmp/bmp_handling.o galois/galois.o lagrange/lagrange.o -o app

gal: galois/galois.c 
	gcc -c -o galois/galois.o galois/galois.c 

lag: lagrange/lagrange.c
	gcc -c -o lagrange/lagrange.o lagrange/lagrange.c

bmp: manejo_bmp/bmp_handling.c
	gcc -c -o manejo_bmp/bmp_handling.o manejo_bmp/bmp_handling.c


clean:
	rm app galois/galois.o lagrange/lagrange.o manejo_bmp/bmp_handling.o