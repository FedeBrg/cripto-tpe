//
// Created by JUAN on 9/05/2021.
//

#include <stdint-gcc.h>
#include <stdio.h>


#ifndef CRIPTO_DECRYPT_H
#define CRIPTO_DECRYPT_H

#endif //CRIPTO_DECRYPT_H

void decrypt(int k, char* path);
char* getTFromImage(int k);
uint8_t getYFromBlock(uint8_t w, uint8_t v, uint8_t u);
void test_lagrange();
void test_lagrange_2();
void test_lagrange_3();
void test_lagrange_4();