#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <dirent.h> 
#include <string.h>
#include "bmp_handling.h"







ImageBMP * read_bmp(char * filename){

	FILE * fp = fopen(filename, "rb");

	ImageBMP * bmp = malloc(sizeof(*bmp));

	fread(&(bmp->header.bfType), sizeof(bmp->header.bfType), 1, fp);
	fread(&(bmp->header.bfSize), sizeof(bmp->header.bfSize), 1, fp);
	fread(&(bmp->header.bfReserved1), sizeof(bmp->header.bfReserved1), 1, fp);
	fread(&(bmp->header.bfReserved2), sizeof(bmp->header.bfReserved2), 1, fp);
	fread(&(bmp->header.bfOffBits), sizeof(bmp->header.bfOffBits), 1, fp);

	fread(&(bmp->header.biSize), sizeof(bmp->header.biSize), 1, fp);
	fread(&(bmp->header.biWidth), sizeof(bmp->header.biWidth), 1, fp);
	fread(&(bmp->header.biHeight), sizeof(bmp->header.biHeight), 1, fp);
	fread(&(bmp->header.biPlanes), sizeof(bmp->header.biPlanes), 1, fp);
	fread(&(bmp->header.biBitCount), sizeof(bmp->header.biBitCount), 1, fp);
	fread(&(bmp->header.biCompression), sizeof(bmp->header.biCompression), 1, fp);
	fread(&(bmp->header.biSizeImage), sizeof(bmp->header.biSizeImage), 1, fp);
	fread(&(bmp->header.biXPelsPerMeter), sizeof(bmp->header.biXPelsPerMeter), 1, fp);
	fread(&(bmp->header.biYPelsPerMeter), sizeof(bmp->header.biYPelsPerMeter), 1, fp);
	fread(&(bmp->header.biClrUsed), sizeof(bmp->header.biClrUsed), 1, fp);
	fread(&(bmp->header.biClrImportant), sizeof(bmp->header.biClrImportant), 1, fp);



	uint32_t size = bmp->header.biHeight * bmp->header.biWidth;

	bmp->pixels = malloc(sizeof(*bmp->pixels) * size);


    int n = fseek(fp, bmp->header.bfOffBits, SEEK_SET);


    fread(bmp->pixels, size , 1, fp);

    return bmp;

}

char * create_path(const char * directory, const char * name){

	char * filename = malloc(strlen(directory) + strlen(name) + 2);

	strcpy(filename,directory);
	strcat(filename,"/");
	strcat(filename,name);

	return filename;


}

ImageBMP * * read_bmps(char * directory, int k){

	DIR *d;
	struct dirent *dir;
	d = opendir(directory);
	ImageBMP * * bmps = malloc(sizeof(ImageBMP *) * k);
	int i = 0;
	if (d) {
		while ((dir = readdir(d)) != NULL && i<k) {
			if(strcmp(".",dir->d_name)){

				char * filename = create_path(directory,dir->d_name);
				printf("%s\n",filename );
				bmps[i] = read_bmp(filename);

				i++;
			}
    	}
    	closedir(d);
    }

    if(i<k){
    	exit(1);
    }
    return bmps;
}




uint8_t * * split_secret(ImageBMP * secret, int k){

	uint8_t * pixels = secret -> pixels;

	uint32_t size = secret->header.biHeight * secret->header.biWidth;

	int n = size/k;

	uint8_t * * splitted = malloc(n * sizeof(uint8_t *));

	int pi = 0;

	for (int i = 0; i < n; i++){

		uint8_t * sub_array = malloc(k * sizeof(uint8_t));

		for (int j = 0; j < k; j++){
			sub_array[j] = pixels[pi];
			pi++;
		}

		splitted[i] = sub_array;
	}

	return splitted;

}

uint8_t * * split_portadora(ImageBMP * portadora){

	uint8_t * pixels = portadora->pixels;

	uint32_t width = portadora->header.biWidth;
	uint32_t height = portadora->header.biHeight;

	int n = width*height/4;


	uint8_t * * splitted = malloc(n * sizeof(uint8_t *));

	int pi = 0;


	for (int i = 0; i < height-1; i+=2){
		for (int j = 0; j < width-1; j+=2){
			uint8_t * sub_array = malloc(4 * sizeof(uint8_t));
			sub_array[0] = pixels[(i*width + j)];
			sub_array[1] = pixels[(i*width + j)+1];
			sub_array[2] = pixels[(i*width + j)+width];
			sub_array[3] = pixels[(i*width + j)+1+width];

			splitted[pi] = sub_array;
			printf("%d\n",pi );

			pi++;


		}

	}

	return splitted;
}




int test_split_secret(ImageBMP * secret, int k){

	uint8_t * * splitted = split_secret(secret, k);

	int size = secret->header.biWidth * secret->header.biHeight;

	int n = size/k;


	for (int i = 0; i < n; i++){

		for (int j = 0; j < k; j++){
			printf("%x,",splitted[i][j] );
		}
		printf("\n");

	}
}


int test_split_portadora(ImageBMP * portadora){

	uint8_t * * splitted = split_portadora(portadora);

	uint32_t width = portadora->header.biWidth;
	uint32_t height = portadora->header.biHeight;

	int n = width*height/4;

	int pi = 0;

	for (int i = 0; i < n; i++){
		for (int j = 0; j < 4; j++){
			printf("%x\t", splitted[i][j]);
		}
		printf("\n");
	}

}



int main(){


	//ImageBMP * bmp = read_bmp_temp("test_bmps/Alfred.bmp");

	//ImageBMP * * bmps = read_bmps("test_bmps",4);


	for (int i = 0; i < 4; ++i){
		//printf("%d\n",bmps[i]->header.biHeight);
	}


	int k = 4;
	//test_split_secret(bmp,k);

	//test_split_portadora(bmp);


}



////////////////////////////////////////////////////////////////////////////////////////
ImageBMP * read_bmp_new(const char * filename){

	FILE * fp = fopen(filename, "rb");

	ImageBMP * bmp = malloc(sizeof(*bmp));


	fread(&(bmp->header), sizeof(bmp->header), 1, fp);

	//printf("%x\n",bmp->header.biSizeImage);

	//printf("%d\n",bmp->header.bfSize);


    bmp->pixels = malloc(sizeof(*bmp->pixels) * bmp->header.biSizeImage);

    printf("%x\n\n", bmp->header.bfOffBits);

    int n = fseek(fp, 1062, SEEK_SET);
    //printf("%d\n",n );


    fread(bmp->pixels, bmp->header.biSizeImage, 1, fp);

    return bmp;
}