#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "lagrange/lagrange.h"
#include "galois/galois.h"
#include "manejo_bmp/bmp_handling.h"
#include "encrypt/encrypt.h"
#include "decrypt/decrypt.h"

int main(){

	int k = 4;
	int n = 6;
	char * out_dir = "output";
	char * secret_name = "manejo_bmp/test_bmps/Eva.bmp";
	char * portadoras_dir = "manejo_bmp/test";

//	ImageBMP * bmp = read_bmp(secret_name);

//	ImageBMP * * carriers = encrypt(n,k,portadoras_dir,bmp);

	decrypt(k,portadoras_dir);

    char name[30];
    sprintf(name, "./secret.bmp", out_dir);
//    write_bmp(secret, name);

//	struct stat st = {0};
//    if (stat(out_dir, &st) == -1) {
//        mkdir(out_dir, 0700);
//    }

//	for (int i = 0; i < n; i++){
//		char name[30];
//		sprintf(name, "./%s/portadora%d.bmp", out_dir,i);
//		write_bmp(carriers[i], name);
//	}




	

	// uint8_t * * splt = split_secret(bmps[0],4);


	// uint8_t * pix = merge_secret(splt,4,bmps[0]->header.biWidth,bmps[0]->header.biHeight);

	// for (int i = 0; i < bmps[0]->header.biWidth*bmps[0]->header.biHeight; i++){
	// 	if (bmps[0]->pixels[i] != pix[i])
	// 	{
	// 		printf("ERROR\n");
	// 	}
	// }
	// printf("A\n");

}