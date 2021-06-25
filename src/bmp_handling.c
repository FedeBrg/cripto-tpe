#include <bmp_handling.h>



ImageBMP * read_bmp(char * filename){

	FILE * fp = fopen(filename, "rb");

	if(fp == NULL){
		return NULL;
	}

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


	unsigned int s = bmp->header.bfOffBits - sizeof(HeaderBMP)+1;

	bmp->extraInfo = malloc(s);

	fread(bmp->extraInfo, s , 1, fp);


	uint32_t size = bmp->header.biHeight * bmp->header.biWidth;

	bmp->pixels = malloc(sizeof(*bmp->pixels) * size);

	if(bmp->pixels == NULL){
		printf("Memoria insuficiente\n");
		exit(-1);
	}

    fseek(fp, bmp->header.bfOffBits, SEEK_SET);

    fread(bmp->pixels, size , 1, fp);

    fclose(fp);

    return bmp;

}

char * create_path(const char * directory, const char * name){

	char * filename = malloc(strlen(directory) + strlen(name) + 2);

	strcpy(filename,directory);
	strcat(filename,"/");
	strcat(filename,name);

	return filename;
}

int count_files(char * directory){
	DIR *d;
	struct dirent *dir;
	d = opendir(directory);
	int i = 0;
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			if (dir->d_type == DT_REG) {
            	i++;
        	}
    	}
    	closedir(d);
    } else {
    	return -1;
    }

    return i;
}

ImageBMP * * read_bmps(char * directory, int n){
	DIR *d;
	struct dirent *dir;
	d = opendir(directory);
	ImageBMP * * bmps = malloc(sizeof(ImageBMP *) * n);
	int i = 0;
	if (d) {
		while ((dir = readdir(d)) != NULL && i<n) {
			if (dir->d_type == DT_REG) {

				char * filename = create_path(directory,dir->d_name);
				bmps[i] = read_bmp(filename);
				free(filename);
				i++;
			}
    	}
    	closedir(d);
    }

    return bmps;
}


void write_bmp(ImageBMP * bmp, char * filename){
	
	FILE * fp = fopen(filename, "wb");

	fwrite(&(bmp->header.bfType), sizeof(bmp->header.bfType), 1, fp);
	fwrite(&(bmp->header.bfSize), sizeof(bmp->header.bfSize), 1, fp);
	fwrite(&(bmp->header.bfReserved1), sizeof(bmp->header.bfReserved1), 1, fp);
	fwrite(&(bmp->header.bfReserved2), sizeof(bmp->header.bfReserved2), 1, fp);
	fwrite(&(bmp->header.bfOffBits), sizeof(bmp->header.bfOffBits), 1, fp);

	fwrite(&(bmp->header.biSize), sizeof(bmp->header.biSize), 1, fp);
	fwrite(&(bmp->header.biWidth), sizeof(bmp->header.biWidth), 1, fp);
	fwrite(&(bmp->header.biHeight), sizeof(bmp->header.biHeight), 1, fp);
	fwrite(&(bmp->header.biPlanes), sizeof(bmp->header.biPlanes), 1, fp);
	fwrite(&(bmp->header.biBitCount), sizeof(bmp->header.biBitCount), 1, fp);
	fwrite(&(bmp->header.biCompression), sizeof(bmp->header.biCompression), 1, fp);
	fwrite(&(bmp->header.biSizeImage), sizeof(bmp->header.biSizeImage), 1, fp);
	fwrite(&(bmp->header.biXPelsPerMeter), sizeof(bmp->header.biXPelsPerMeter), 1, fp);
	fwrite(&(bmp->header.biYPelsPerMeter), sizeof(bmp->header.biYPelsPerMeter), 1, fp);
	fwrite(&(bmp->header.biClrUsed), sizeof(bmp->header.biClrUsed), 1, fp);
	fwrite(&(bmp->header.biClrImportant), sizeof(bmp->header.biClrImportant), 1, fp);

	unsigned int s = bmp->header.bfOffBits - sizeof(HeaderBMP) +1;
	fwrite(bmp->extraInfo, s , 1, fp);

	fseek(fp, bmp->header.bfOffBits, SEEK_SET);
	uint32_t size = bmp->header.biHeight * bmp->header.biWidth;
    fwrite(bmp->pixels, size , 1, fp);

    fclose(fp);

}

uint8_t * * split_portadora(ImageBMP * portadora){
	
	uint8_t * pixels = portadora->pixels;

	uint32_t width = portadora->header.biWidth;
	uint32_t height = portadora->header.biHeight;

	int n = (width*height)/4;

	uint8_t * * splitted = malloc(n * sizeof(uint8_t *));
	int k = 0;

	for (int i = height-1; i > 0; i-=2){
		for (int j = 0; j < width-1; j+=2){
			uint8_t * sub_array = malloc(4 * sizeof(uint8_t));
			sub_array[0] = pixels[i*width + j];					// X
			sub_array[1] = pixels[i*width + j + 1];				// W
			sub_array[2] = pixels[i*width + j - width];			// V
			sub_array[3] = pixels[i*width + j + 1 - width];		// U

			splitted[k] = sub_array;
			k++;

		}
	}

	return splitted;
}


uint8_t * merge_portadora(uint8_t * * portadora, uint32_t width, uint32_t height ){
	
	uint8_t * pixels = malloc(height * width);

	int pi = 0;

	for (int i = height-1; i > 0; i-=2){
		for (int j = 0; j < width-1; j+=2){
			uint8_t * sub_array = portadora[pi];

			pixels[(i*width + j)] = sub_array[0];			// X
			pixels[(i*width + j)+1] = sub_array[1];			// W
			pixels[(i*width + j)-width] = sub_array[2];		// V
			pixels[(i*width + j)+1-width] = sub_array[3];	// U
			
			pi++;
		}

	}

	return pixels;
}

void free_image(ImageBMP * image){

    free(image->extraInfo);
    free(image->pixels);
    free(image);
}

void split_portadora_free(uint8_t * * split, uint32_t width, uint32_t height){
    for (int i = 0; i < width*height/4; ++i){
        free(split[i]);
    }

    free(split);
}