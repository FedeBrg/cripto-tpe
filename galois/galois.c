#include "galois.h"


// https://en.wikipedia.org/wiki/Finite_field_arithmetic

// Haciendo el truco de desplazar podes usar todo con uint8 y te ahorras espacio


uint8_t gen_desplazado = 0b01100011;



uint8_t suma_galois(uint8_t x, uint8_t y){
    //printf("%d,%d\n",x,y );
    return x ^ y;
}

uint8_t mult_galois(uint8_t x, uint8_t y){

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


uint8_t inv_galois(uint8_t x){
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

