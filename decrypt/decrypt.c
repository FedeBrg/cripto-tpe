
#include "decrypt.h"
#include "../manejo_bmp/bmp_handling.h"
#include "../lagrange/lagrange.h"
#include "../galois/galois.h"


//     int main(){
//         char x,w,v,u;
//         x=0;
//         w=0xFF;
//         v=0xAA;
//         u=4;
//         getYFromBlock(w, v, u);
//     }

void decrypt(int k, char path[]){
    //k es el k y path es donde estan las imagenes
    k=6;
    //magia para levantar las imagenes
    ImageBMP **images; //malloc
//    images = read_bmps("../manejo_bmp/test_bmps/",k);
    //leo imagenes



    ImageBMP * portadora1 = read_bmp_new("../manejo_bmp/secret/Alfredshare.bmp");
    printf("\n%d,%d\n",portadora1->header.biHeight,portadora1->header.biWidth);
    printf("\n%d,%d\n",portadora1->header.biHeight,portadora1->header.biWidth);
    ImageBMP * portadora2 = read_bmp_new("../manejo_bmp/secret/Evashare.bmp");
    ImageBMP * portadora3 = read_bmp_new("../manejo_bmp/secret/Facundoshare.bmp");
    ImageBMP * portadora4 = read_bmp_new("../manejo_bmp/secret/Gustavoshare.bmp");
    ImageBMP * portadora5 = read_bmp_new("../manejo_bmp/secret/Marilynshare.bmp");
    ImageBMP * portadora6 = read_bmp_new("../manejo_bmp/secret/Jamesshare.bmp");

    // hago el array de punteros con la imagenes, aca despues hay que hacer el read_bmps
    ImageBMP * carriers[6];
    carriers[0] = portadora1;
    carriers[1] = portadora2;
    carriers[2] = portadora3;
    carriers[3] = portadora4;
    carriers[4] = portadora5;
    carriers[5] = portadora6;

    printf("1");


    images=carriers;

    uint8_t *** blocks= malloc(sizeof( uint8_t*) *k); //malloc
    for (int i = 0; i < k; ++i) {
        printf("2");

        blocks[i]=split_portadora(images[i]);
    }
    //obtengo los bloques

    unsigned int length=(images[0]->header.biHeight * images[0]->header.biHeight)/k;
//    uint8_t y[length][k];//array con los valores de Y arrancando desde arriba a la dercha
//    uint8_t x[length][k];//malloc?
//    uint8_t s[length]; //probablemente sea con memeoria
    uint8_t **y= malloc(sizeof (uint8_t*)*length);
    uint8_t **x= malloc(sizeof (uint8_t*)*length);
    uint8_t *s= malloc(sizeof (uint8_t*)*length);
    for (int i = 0; i < (images[0]->header.biHeight * images[0]->header.biHeight)/k; ++i) {
        x[i]= malloc(sizeof (blocks[0][0][0])*k );
        y[i]= malloc(sizeof (blocks[0][0][0])*k );
        for (int j = 0; j < k; ++j) {
            y[i][j]=getYFromBlock( blocks[i][j][1], blocks[i][j][2], blocks[i][j][3]);
            x[i][j]=blocks[i][j][0];
        }
    }
    for (int i = 0; i < (images[0]->header.biHeight * images[0]->header.biHeight)/k; ++i) {
        s[i]=lagrange(k,x[i],y[i]);
        for (int j = 0; j < k; ++j) {
            y[i][j]=(y[i][j]*s[0])/x[i][j]; //chequear que sean esos x e y ?
            y[i][j]=div_galois(mult_galois(y[i][j],s[0]),x[i][j]); //chequear que sean esos x e y ?
        }
    }
    printf("asdasd");
    ImageBMP * tmp = read_bmp("../manejo_bmp/secret/James.bmp");
    tmp->pixels=s;
    write_bmp(tmp,"./secret.bmp");
//    return tmp;
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