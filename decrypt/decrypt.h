#include <stdint-gcc.h>
#include <stdio.h>
//#include "../manejo_bmp/bmp_handling.h"


#ifndef CRIPTO_DECRYPT_H
#define CRIPTO_DECRYPT_H

#endif //CRIPTO_DECRYPT_H 

void decrypt(int k, char* path);
char* getTFromImage(int k);
uint8_t getYFromBlock(uint8_t w, uint8_t v, uint8_t u);
void test_lagrange();
//void compare_image(ImageBMP s,ImageBMP img1);
