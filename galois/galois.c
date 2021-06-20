#include "galois.h"
#include <stdlib.h>



// https://en.wikipedia.org/wiki/Finite_field_arithmetic

// Haciendo el truco de desplazar podes usar todo con uint8 y te ahorras espacio


uint8_t gen_desplazado = 0b01100011;

uint8_t tabla_inv[256];

uint8_t * tabla_mult[256];



uint8_t suma_galois(uint8_t x, uint8_t y){
    //printf("%d,%d\n",x,y );
    return x ^ y;
}

uint8_t mult_galois(uint8_t x, uint8_t y){

    return tabla_mult[x][y];

}

uint8_t mult_galois_tabla(uint8_t x, uint8_t y){

    uint8_t p = 0;
    for (; y ; y >>= 1) {
        if(y & 0b00000001){
            p ^= x;
        }
        if (x & 0b10000000){
            x <<= 1;
            x ^= gen_desplazado;
        }
        else{
            x <<= 1;
        }
    }


    return p;

}

void llenar_tablas(){
    for (int i = 1; i < 256; i++){
        uint8_t inv = 1;

        while(mult_galois_tabla(i,inv)!=1){
            inv++;
        }
        tabla_inv[i] = inv;    
    }

    for (int i = 0; i < 256; i++){
        tabla_mult[i] = malloc(256 * sizeof(uint8_t));
        for (int j = 0; j < 256; j++){
            tabla_mult[i][j] = mult_galois_tabla(i,j);
        }
    }

}

uint8_t inv_galois(uint8_t x){
    return tabla_inv[x];

}


uint8_t inv_galois_old(uint8_t x){
    uint8_t inv = 1;

    while(mult_galois(x,inv)!=1){
        inv++;
    }
    return inv;

}

uint8_t div_galois(uint8_t x, uint8_t y){

    uint8_t inv = inv_galois(y);

    return mult_galois(x,inv);

}

uint8_t pot_galois(uint8_t x,uint8_t y){

    uint8_t pot = 1;
    for (int i = 0; i < y; i++){
        pot = mult_galois(pot,x);
    }
    return pot;

}

