#include "galois.h"


// https://en.wikipedia.org/wiki/Finite_field_arithmetic

// Haciendo el truco de desplazar podes usar todo con uint8 y te ahorras espacio


uint8_t gen_desplazado = 0b01100011;



uint8_t suma_galois(uint8_t x, uint8_t y){
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