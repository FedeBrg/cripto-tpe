#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "manejo_bmp/bmp_handling.h"
#include "encrypt/encrypt.h"
#include "decrypt/decrypt.h"

int main(int argc, char ** argv){
	if(argc != 5){
		printf("Numero de parametros incorrecto.\n");
		return -1;
	}

	int k = atoi(argv[3]);
	if (k > 6 || k < 4){
		printf("El valor k=%d esta fuera del rango [4,6].\n",k );
		return -1;
	}

	int * error = 0;

	if(strcmp(argv[1], "d") == 0){
		ImageBMP * secret = read_bmp(argv[2]);
		ImageBMP ** carriers = encrypt(k, argv[4], secret, error);

		if (error == 1){
			printf("No hay suficientes imagenes en el directorio.");
		} else if(error == 2){
			printf("No existe el directorio especificado.");
		}

		struct stat st = {0};
	   	if (stat(out_dir, &st) == -1) {
	   		mkdir(out_dir, 0700);
	   	}

		for (int i = 0; i < n; i++){
			char name[30];
			sprintf(name, "./%s/portadora%d.bmp", out_dir, i);
			write_bmp(carriers[i], name);
		}

		for (int i = 0; i < n; i++){
	        free_image(carriers[i]);
	    }

	    free(carriers);
	    free_image(secret);
	}

	else if(strcmp(argv[1], "r") == 0){
		ImageBMP * secret = decrypt(argv[3], argv[4]);
		write_bmp(secret, argv[2]);
		free_image(secret);
	}
	else{
		printf("Elige d o r y vuelve a intentar.\n");
	}
}