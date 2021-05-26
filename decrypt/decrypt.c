
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

//    images=read_bmps("../manejo_bmp/secret",k);

    ImageBMP * portadora1 = read_bmp("manejo_bmp/secret/Alfredshare.bmp");
    ImageBMP * portadora2 = read_bmp("manejo_bmp/secret/Evashare.bmp");
    ImageBMP * portadora3 = read_bmp("manejo_bmp/secret/Audreyshare.bmp");
    ImageBMP * portadora4 = read_bmp("manejo_bmp/secret/Gustavoshare.bmp");
    ImageBMP * portadora5 = read_bmp("manejo_bmp/secret/Marilynshare.bmp");
    ImageBMP * portadora6 = read_bmp("manejo_bmp/secret/Jamesshare.bmp");

    // hago el array de punteros con la imagenes, aca despues hay que hacer el read_bmps
    ImageBMP * carriers[6];
    carriers[0] = portadora1;
    carriers[1] = portadora2;
    carriers[2] = portadora3;
    carriers[3] = portadora4;
    carriers[4] = portadora5;
    carriers[5] = portadora6;

//    printf("1");


    images=carriers;

    uint8_t *** blocks= malloc(sizeof( uint8_t*) *k); //malloc
    for (int i = 0; i < k; ++i) {
        printf("Image: %d, size %d,%d\n",i+1,images[i]->header.biHeight,images[i]->header.biWidth);

        blocks[i]=split_portadora(images[i]);
    }
    //obtengo los bloques

    unsigned int length=(images[0]->header.biHeight * images[0]->header.biHeight)/4;
//    uint8_t y[length][k];//array con los valores de Y arrancando desde arriba a la dercha
//    uint8_t x[length][k];//malloc?
//    uint8_t s[length]; //probablemente sea con memeoria
    uint8_t **y= malloc(sizeof (uint8_t*)*length);
    uint8_t **x= malloc(sizeof (uint8_t*)*length);
    uint8_t *s= images[0]->pixels;

    printf("length is: %d\n",length);
    printf("size is: %d",sizeof (uint8_t*)*length);

    for (int i = 0; i < length; ++i) {
        x[i]= malloc(sizeof (uint8_t)*k );
        y[i]= malloc(sizeof (uint8_t)*k );
//        printf("\n%d\n",i);
        for (int j = 0; j < k; ++j) {
//            printf("%d",j);
            y[i][j]=getYFromBlock( blocks[j][i][1], blocks[j][i][2], blocks[j][i][3]);
            x[i][j]=blocks[j][i][0];
//            printf("x=%d, y=%d\n",x[i][j],y[i][j]);
        }
        if(i%1000==0){
            printf("%d\n",i);
        }
    }
    int div0errors=0;
    for (int i = 0; i < length; i+=8) {
        s[i]= lagrange_turbina(k,0,x[i],y[i]);
        if(i<length-8) {
            for (int j = 1; j < 8; ++j) {
                for (int l = 0; l < k; ++l) {
                    if (x[i + j][l] != 0) {
                        y[i + j][l] = div_galois(suma_galois(y[i + j][l], -s[i]),x[i + j][l]); //chequear que sean esos x e y ?
                    } else {
                        y[i + j][l] = div_galois(suma_galois(y[i + j][l], -s[i]), 255);;
                        div0errors++;
                    }
                }
                s[i + j] = lagrange_turbina(k, j, x[i + j], y[i + j]);
            }
            if (i % 800 == 0) {
                printf("%d, s=%d\n", i, s[i]);
            }
        }
    }


//    for (int i = 0; i < length; ++i) {
////        printf("antes de lagrange\n");
//        s[i]=lagrange_turbina(k,i%8,x[i],y[i]);
//
////        printf("despues de lagrange\n");
//        if(i%8==0 && i<length-8) {
//            for (int l = 1; l < 8; ++l) {
//                for (int j = 0; j < k; ++j) {
////            printf("iteration:%d,div:(%d*%d)/%d\n",i,y[i][j],s[0],x[i][j]);
//                    if (x[i+l][j] != 0) {
//                        y[i+l][j] = div_galois(suma_galois(y[i+l][j], -s[0]),x[i+l][j]); //chequear que sean esos x e y ?
//                    } else {
//                        y[i+l][j]=div_galois(suma_galois(y[i+l][j], -s[0]),255);;
//                        div0errors++;
//                    }
//                }
//            }
//        }
//        if(i%1000==0){
//            printf("%d, s=%d\n",i,s[i]);
//        }
//
//    }
    printf("\n");


    printf("finished\n");

//    ImageBMP * tmp = read_bmp("../manejo_bmp/secret/Jamesshare.bmp");
    printf("error: divs por 0 = %d\n",div0errors);
    images[0]->pixels=s;
    write_bmp(images[0],"./secret.bmp");
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
//        printf("pure: %x,%x,%x,%x\n",w,v,u,t);
//        printf("shifted: %x,%x,%x,%x\n",(w&7)<<5,(v&7)<<2,(u&3),t);

//        printf("parity match! t is: %x\n",t);
        return t;
    } else{
        unsigned char t=0; //ejemplo w=11111111 v=10101010 y u=00000000 => w&7 = 00000111, v&7=00000010 y u&3 = 00000000 => x=11101000
        t=t|((w&7)<<5); //and con 00000111 para obtener los ultimos 3 bits y los corro hacia la derecha
        t=t|((v&7)<<2); //and con 00000111 para obtener los ultimos 3 bits y corro hacia la dreche 2 veces
        t=t|((u&3));
//        printf("error!\n");
//        printf("pure: %x,%x,%x,%x\n",w,v,u,t);
//        printf("shifted: %x,%x,%x,%x\n",(w&7)<<5,(v&7)<<2,(u&3),t);
//        t=t|((w&7)<<5); //and con 00000111 para obtener los ultimos 3 bits y los corro hacia la derecha
//        t=t|((v&7)<<2); //and con 00000111 para obtener los ultimos 3 bits y corro hacia la dreche 2 veces
//        t=t|((u&3)); //ultimos dos bits
//        printf("no parity match!t is: %x\n",t);
        return t;
    }
}