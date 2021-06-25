#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "lagrange/lagrange.h"
#include "galois/galois.h"
#include "manejo_bmp/bmp_handling.h"
#include "encrypt/encrypt.h"
#include "decrypt/decrypt.h"



void test(){



	ImageBMP * s1 = read_bmp("manejo_bmp/test_bmps/Marilyn.bmp");
	ImageBMP * s2 = read_bmp("secret.bmp");

	ImageBMP * portadora1 = read_bmp("manejo_bmp/test_bmps/Alfred.bmp");
    ImageBMP * portadora2 = read_bmp("manejo_bmp/test_bmps/Eva.bmp");
    ImageBMP * portadora3 = read_bmp("manejo_bmp/test_bmps/Facundo.bmp");
    ImageBMP * portadora4 = read_bmp("manejo_bmp/test_bmps/Gustavo.bmp");
    ImageBMP * portadora5 = read_bmp("manejo_bmp/test_bmps/Marilyn.bmp");
    ImageBMP * portadora6 = read_bmp("manejo_bmp/test_bmps/James.bmp");


	ImageBMP * portadora1a = read_bmp("output/portadora0.bmp");
    ImageBMP * portadora2a = read_bmp("output/portadora1.bmp");
    ImageBMP * portadora3a = read_bmp("output/portadora2.bmp");
    ImageBMP * portadora4a = read_bmp("output/portadora3.bmp");
    ImageBMP * portadora5a = read_bmp("output/portadora4.bmp");
    ImageBMP * portadora6a = read_bmp("output/portadora5.bmp");


     for (int i = 0; i < 300*300; i ++){
     	if(s1->pixels[i] != s2->pixels[i]){
     		printf("---------------------------------------\n");
     		printf("%d\n", i);
     		printf("%d %d %d %d %d %d\n",portadora1->pixels[i],portadora2->pixels[i],portadora3->pixels[i],portadora4->pixels[i],portadora5->pixels[i],portadora6->pixels[i] );
     		printf("%d %d %d %d %d %d\n",portadora1a->pixels[i],portadora2a->pixels[i],portadora3a->pixels[i],portadora4a->pixels[i],portadora5a->pixels[i],portadora6a->pixels[i] );
     		printf("%d - %d\n",s1->pixels[i],s2->pixels[i] );
     	}
     }

	
}

void encdec(){

	int k = 6;
	int n = 6;
	char * out_dir = "output";
	char * secret_name = "manejo_bmp/test_bmps/Marilyn.bmp";
	//char * secret_name = "secret.bmp";
	char * portadoras_dir = "manejo_bmp/test";

	ImageBMP * bmp = read_bmp(secret_name);


    //char name[30];


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

	for (int i = 0; i < n; i++){
        free_image(carriers[i]);
    }

    free(carriers);

    free_image(bmp);


    decrypt(6,"manejo_bmp/secret");

    free_tables();


}


int main(){

	encdec();

	//test();

}


