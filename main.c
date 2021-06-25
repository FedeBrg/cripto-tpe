#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <bmp_handling.h>
#include <encrypt.h>
#include <decrypt.h>
#include <galois.h>

int main(int argc, char ** argv){

	char * out_dir = "portadoras_generadas";
	if(argc != 5){
		printf("Numero de parametros incorrecto.\n");
		return -1;
	}

	int k = atoi(argv[3]);
	if (k > 6 || k < 4){
		printf("El valor k=%d esta fuera del rango [4,6].\n",k );
		return -1;
	}

	int n = count_files(argv[4]);

	if(n == -1){
		printf("El directorio no existe.\n");
		return -1;
	} else if (n < k){
		printf("No hay suficientes imagenes en el directorio.(n = %d, k = %d)\n",n,k );
		return -1;
	}

	llenar_tablas();


	if(strcmp(argv[1], "d") == 0){
		ImageBMP * secret = read_bmp(argv[2]);
		//ImageBMP * secret = NULL;
		if(secret == NULL){
			printf("No existe la imagen que se quiere ocultar.\n");
			return -1;
		}
		ImageBMP ** carriers = encrypt(k, n, argv[4], secret);


		struct stat st = {0};
	   	if (stat(out_dir, &st) == -1) {
	   		mkdir(out_dir, 0700);
	   	} 

		for (int i = 0; i < n; i++){
			char name[30];
			sprintf(name, "./%s/portadora%d.bmp", out_dir, i);
			write_bmp(carriers[i], name);
		}

		printf("Distribucion terminada. Portadoras en: portadoras_generadas\n");

		for (int i = 0; i < n; i++){
	        free_image(carriers[i]);
	    }

	    free(carriers);
	 	free_image(secret);
	}

	else if(strcmp(argv[1], "r") == 0){
		ImageBMP * secret = decrypt(k, argv[4]);
		write_bmp(secret, argv[2]);
		free_image(secret);
		printf("Recuperacion terminada.\n");
	}
	else{
		printf("La opcion %s es invalida.\n",argv[1]);
		return -1;
	}

	free_tables();
}