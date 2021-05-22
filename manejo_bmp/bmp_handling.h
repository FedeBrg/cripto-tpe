#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <dirent.h> 
#include <string.h>

// https://web.archive.org/web/20080912171714/http://www.fortunecity.com/skyscraper/windows/364/bmpffrmt.html
#pragma pack(push, 1)
typedef struct{
	// BITMAPFILEHEADER
	uint16_t bfType;
	uint32_t bfSize;
	uint16_t bfReserved1;
	uint16_t bfReserved2;
	uint32_t bfOffBits;

	// BITMAPINFOHEADER
	uint32_t biSize;
	uint32_t biWidth;
	uint32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	uint32_t biXPelsPerMeter;
	uint32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;


}HeaderBMP;
#pragma pack(pop)


typedef struct {
    HeaderBMP header;
    uint8_t * extraInfo;
    uint8_t * pixels;
} ImageBMP;

// Le pasas una ImageBMP * que leiste con read_bmp y te devuelve un Array de Arrays con
// [[X0, W0, V0, U0], [X1, W1, V1, U1], ..., [Xn, Wn, Vn, Un]]
// Donde n = (bmp->header.biHeight * bmp->header.biHeight)/4
uint8_t * * split_portadora(ImageBMP * portadora);

// Le pasas una ImageBMP * que leiste con read_bmp y te devuelve un Array de Arrays con
// [S1, S2, ..., Sn], donde n = (bmp->header.biHeight * bmp->header.biHeight)/k
// y cada S = [s0, s1, ..., sk]
uint8_t * * split_secret(ImageBMP * secret, int k);

// Le pasas el path a UNA BMP y te la lee
ImageBMP * read_bmp(char * filename);
ImageBMP * read_bmp_new(char * filename);


// Le pasas el k que te dan de entrada y el path a un DIRECTORIO y te devuelve un ImageBMP * * con todas las bmps
ImageBMP * * read_bmps(char * directory, int k); 


// Le pasas el array de arrays con los cuadraditos de 2x2 y te los mergea para poder ponerlos como pixel en la ImageBMP
uint8_t * merge_portadora(uint8_t * * portadora, uint32_t width, uint32_t height);


// Le pasas el [S1, S2, ..., Sn], donde n = (bmp->header.biHeight * bmp->header.biHeight)/k
// y cada S = [s0, s1, ..., sk]
// te devuelve un array para meter en la ImageBMP
uint8_t * merge_secret(uint8_t * * secret, int k, uint32_t width, uint32_t height);


void write_bmp(ImageBMP * bmp, char * filename);


void write_bmps(ImageBMP * bmp, char * filename, int k);