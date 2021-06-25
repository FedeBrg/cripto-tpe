libflags = -c -Wall -Werror -o
appflags = -Wall -Werror

all: dec enc gal lag bmp main

main: main.c src/galois.o src/lagrange.o src/bmp_handling.o src/decrypt.o src/encrypt.o
	gcc  $(appflags) main.c src/bmp_handling.o src/galois.o src/lagrange.o src/decrypt.o src/encrypt.o -o ss -lm -I includes/

gal: src/galois.c 
	gcc $(libflags) src/galois.o src/galois.c -I includes/

lag: src/lagrange.c
	gcc $(libflags) src/lagrange.o src/lagrange.c -I includes/

bmp: src/bmp_handling.c
	gcc $(libflags) src/bmp_handling.o src/bmp_handling.c -I includes/

dec: src/decrypt.c
	gcc $(libflags) src/decrypt.o src/decrypt.c -I includes/

enc: src/encrypt.c
	gcc $(libflags) src/encrypt.o src/encrypt.c -I includes/

clean:
	rm ss src/*.o