#include "lagrange/lagrange.h"
#include "galois/galois.h"
#include "manejo_bmp/bmp_handling.h"

int main(){


	ImageBMP * bmp = read_bmp("manejo_bmp/test_bmps/Marilyn.bmp");




	//printf("%d\n",bmp->header.biSize );

	for (int i = 0; i < 256*3; ++i){
		//printf("%x ",bmp->colorTable[i] );
	}


	write_bmp(bmp, "test.bmp");


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