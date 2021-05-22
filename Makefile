all: dec enc gal lag bmp main

main: app.c galois/galois.o lagrange/lagrange.o manejo_bmp/bmp_handling.o
	gcc app.c manejo_bmp/bmp_handling.o galois/galois.o lagrange/lagrange.o decrypt/decrypt.o encrypt/encrypt.o -o app -lm

gal: galois/galois.c 
	gcc -c -o galois/galois.o galois/galois.c 

lag: lagrange/lagrange.c
	gcc -c -o lagrange/lagrange.o lagrange/lagrange.c

bmp: manejo_bmp/bmp_handling.c
	gcc -c -o manejo_bmp/bmp_handling.o manejo_bmp/bmp_handling.c

dec: decrypt/decrypt.c
	gcc -c -o decrypt/decrypt.o decrypt/decrypt.c

enc: encrypt/encrypt.c
	gcc -c -o encrypt/encrypt.o encrypt/encrypt.c

clean:
	rm app galois/galois.o lagrange/lagrange.o manejo_bmp/bmp_handling.o decrypt/decrypt.o encrypt/encrypt.o