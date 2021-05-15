

#include <stdio.h>
#include "decrypt.h"
#include "../lagrange/lagrange.h"
#include "../galois/galois.h"
#include "../manejo_bmp/bmp_handling.h"

int main(){
    char x,w,v,u;
    x=0;
    w=0xFF;
    v=0xAA;
    u=4;
    getYFromBlock(w, v, u);
}

void decrypt(int k, char path[]){
    //k es el k y path es donde estan las imagenes
    k=4;
    //magia para levantar las imagenes
    ImageBMP **images; //malloc
    images = read_bmps("../manejo_bmp/test_bmps/",k);
    //leo imagenes

    uint8_t *** blocks= malloc(sizeof( uint8_t*) *k); //malloc
    for (int i = 0; i < k; ++i) {
        blocks[i]=split_portadora(images[i]);
    }
    //obtengo los bloques

    unsigned int length=(images[0]->header.biHeight * images[0]->header.biHeight)/k;
    uint8_t y[length][k];//array con los valores de Y arrancando desde arriba a la dercha
    uint8_t x[length][k];//malloc?
    uint8_t s[length]; //probablemente sea con memeoria
    for (int i = 0; i < (images[0]->header.biHeight * images[0]->header.biHeight)/k; ++i) {
        for (int j = 0; j < k; ++j) {
            y[i][j]=getYFromBlock( blocks[i][j][1], blocks[i][j][2], blocks[i][j][3]);
            x[i][j]=blocks[i][j][0];
        }
    }
    for (int i = 0; i < (images[0]->header.biHeight * images[0]->header.biHeight)/k; ++i) {
        s[i]=(char)lagrange(k,x[i],y[i]); //castear a double?
        for (int j = 0; j < k; ++j) {
            y[i][j]=(y[i][j]*s[0])/x[i][j]; //chequear que sean esos x e y ?
        }
    }
}

//char* getTFromImage(int k){
//    int length=10;
//    unsigned char y[length]; //array con los valores de Y arrancando desde arriba a la dercha
//    unsigned char x[length];
//    unsigned char s[length]; //probablemente sea con memeoria
//    for (int i = 0; i < length; ++i) {
//        //armar los bloques y pasarselos a
//        y[i]= getYFromBlock( 2, 3, 4);//pasarle un bloque
//        x[i]= 1;
//    }
//    for (int i = 0; i < length ; ++i) {
//        s[i]=lagrange(k,x,y); //castear a double? agregar mod g(x)
//        y[i]=(y[i]*s[0])/x[i] ; //agregar mod g(x)?
//        //chequear indices del apunte (estan torcidos? que hace r?
//    }
//
//    return s;
//    //hacer desencriptacion magica
//}

unsigned char getYFromBlock( uint8_t w, uint8_t v, uint8_t u){

    //chequear pariedad
    if((w&1)^(w&2)>>1^(w&4)>>2^(v&1)^(v&2)>>1^(v&4)>>2^(u&1)^(u&2)>>1==(u&4)>>2){
        unsigned char t=0; //ejemplo w=11111111 v=10101010 y u=00000000 => w&7 = 00000111, v&7=00000010 y u&3 = 00000000 => x=11101000

        t=t|((w&7)<<5); //and con 00000111 para obtener los ultimos 3 bits y los corro hacia la derecha
        t=t|((v&7)<<2); //and con 00000111 para obtener los ultimos 3 bits y corro hacia la dreche 2 veces
        t=t|((u&3)); //ultimos dos bits

        printf("parity match\n");
        printf("t is: %x\n",t);
        return t;
    } else{
        printf("no parity match");
    }
}