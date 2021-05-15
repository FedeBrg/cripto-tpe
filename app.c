#include "lagrange/lagrange.h"
#include "galois/galois.h"
#include "manejo_bmp/bmp_handling.h"

int main(){


	ImageBMP * * bmps = read_bmps("manejo_bmp/test_bmps",2);

	printf("Image height: %d\n",bmps[0]->header.biHeight );


	uint8_t * * splt = split_secret(bmps[0],4);


	uint8_t * pix = merge_secret(splt,4,bmps[0]->header.biWidth,bmps[0]->header.biHeight);

	for (int i = 0; i < bmps[0]->header.biWidth*bmps[0]->header.biHeight; i++){
		if (bmps[0]->pixels[i] != pix[i])
		{
			printf("ERROR\n");
		}
	}
	printf("A\n");

}