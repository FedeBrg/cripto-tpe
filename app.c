#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "lagrange/lagrange.h"
#include "galois/galois.h"
#include "manejo_bmp/bmp_handling.h"
#include "encrypt/encrypt.h"
#include "decrypt/decrypt.h"

int main(){

	int k = 6;
	int n = 6;
	char * out_dir = "output";
	char * secret_name = "manejo_bmp/test_bmps/Eva.bmp";
	//char * secret_name = "secret.bmp";
	char * portadoras_dir = "manejo_bmp/test";

	ImageBMP * bmp = read_bmp(secret_name);



    char name[30];


    llenar_tablas();




	ImageBMP * * carriers = encrypt(n,k,portadoras_dir,bmp);




	struct stat st = {0};
   if (stat(out_dir, &st) == -1) {
       mkdir(out_dir, 0700);
   }

	for (int i = 0; i < n; i++){
		char name[30];
		sprintf(name, "./%s/portadora%d.bmp", out_dir,i);
		write_bmp(carriers[i], name);
	}


     decrypt(6,"manejo_bmp/secret");

}