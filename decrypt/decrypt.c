#include "decrypt.h"
#include "../manejo_bmp/bmp_handling.h"
#include "../lagrange/lagrange.h"
#include "../galois/galois.h"



void decrypt(int k, char path[]){
    //k es el k y path es donde estan las imagenes
    k=6;
    //magia para levantar las imagenes
    //ImageBMP **images; //malloc
//    images = read_bmps("../manejo_bmp/test_bmps/",k);
//    images=read_bmps("../manejo_bmp/secret",k);

   // ImageBMP * portadora1 = read_bmp("manejo_bmp/secret/Alfredshare.bmp");
   // ImageBMP * portadora2 = read_bmp("manejo_bmp/secret/Evashare.bmp");
   // ImageBMP * portadora3 = read_bmp("manejo_bmp/secret/Audreyshare.bmp");
   // ImageBMP * portadora4 = read_bmp("manejo_bmp/secret/Gustavoshare.bmp");
   // ImageBMP * portadora5 = read_bmp("manejo_bmp/secret/Marilynshare.bmp");
   // ImageBMP * portadora6 = read_bmp("manejo_bmp/secret/Jamesshare.bmp");

     ImageBMP * portadora1 = read_bmp("output/portadora0.bmp");
     ImageBMP * portadora2 = read_bmp("output/portadora1.bmp");
     ImageBMP * portadora3 = read_bmp("output/portadora2.bmp");
     ImageBMP * portadora4 = read_bmp("output/portadora3.bmp");
     ImageBMP * portadora5 = read_bmp("output/portadora4.bmp");
     ImageBMP * portadora6 = read_bmp("output/portadora5.bmp");


   // ImageBMP * portadora1 = read_bmp("portadoras/Alfred.bmp");
   // ImageBMP * portadora2 = read_bmp("portadoras/Eva.bmp");
   // ImageBMP * portadora3 = read_bmp("portadoras/Audrey.bmp");
   // ImageBMP * portadora4 = read_bmp("portadoras/Gustavo.bmp");
   // ImageBMP * portadora5 = read_bmp("portadoras/Facundo.bmp");
   // ImageBMP * portadora6 = read_bmp("portadoras/Albert.bmp");

    // hago el array de punteros con la imagenes, aca despues hay que hacer el read_bmps
    ImageBMP ** images = malloc(k*sizeof(ImageBMP *));
    images[0] = portadora1;
    images[1] = portadora2;
    images[2] = portadora3;
    images[3] = portadora4;
    images[4] = portadora5;
    images[5] = portadora6;


    uint8_t *** blocks= malloc(sizeof( uint8_t**) *k); //malloc
    for (int i = 0; i < k; ++i) {
        blocks[i]=split_portadora(images[i]);
    }
    //obtengo los bloques

    unsigned int length=(images[0]->header.biHeight * images[0]->header.biWidth);
    uint8_t *s = malloc(sizeof (uint8_t)*length);

    uint8_t xOne[k];
    uint8_t yOne[k];

    for (int i = 0; i < length/k; i++) {
        for (int j = 0; j < k; j++) {
            xOne[j]=blocks[j][i][0];
            yOne[j]=getYFromBlock( blocks[j][i][1], blocks[j][i][2], blocks[j][i][3]);
        }

        for (int l = 1; l < k; l++) {
                if (xOne[l] == 0){
                    uint8_t xTmp, yTmp;

                    xTmp=xOne[0];
                    yTmp=yOne[0];

                    xOne[0]=xOne[l];
                    yOne[0]=yOne[l];

                    xOne[l]=xTmp;
                    yOne[l]=yTmp;

                    break;
                }
            }


        s[i*k] = lagrange(k,xOne,yOne);
        for (int j = 1; j < k; j++) {
            for (int l = j; l < k; l++) {
                yOne[l] = div_galois(suma_galois(yOne[l], s[i*k+j-1]),xOne[l]);
            }
            s[i*k + j] = lagrange(k-j, xOne+j, yOne+j);
        }
    }

    free(images[0]->pixels);
    images[0]->pixels=s;

    for (int i = 0; i < k; i++){
        split_portadora_free(blocks[i],images[0]->header.biWidth,images[0]->header.biHeight);
    }

    free(blocks);



    write_bmp(images[0],"./secret.bmp");

    for (int i = 0; i < k; i++){
        free_image(images[i]);
    }

    free(images);


}




uint8_t getYFromBlock( uint8_t w, uint8_t v, uint8_t u){

    //chequear pariedad
    if(((w&1)^(w&2)>>1^(w&4)>>2^(v&1)^(v&2)>>1^(v&4)>>2^(u&1)^(u&2)>>1)==((u&4)>>2)){
        uint8_t t=0; //ejemplo w=11111111 v=10101010 y u=00000000 => w&7 = 00000111, v&7=00000010 y u&3 = 00000000 => x=11101000
        t=t|((w&7)<<5); //and con 00000111 para obtener los ultimos 3 bits y los corro hacia la derecha
        t=t|((v&7)<<2); //and con 00000111 para obtener los ultimos 3 bits y corro hacia la dreche 2 veces
        t=t|((u&3)); // 3=00000011 ultimos dos bits
//        printf("pure: %x,%x,%x,%x\n",w,v,u,t);
//        printf("shifted: %x,%x,%x,%x\n",(w&7)<<5,(v&7)<<2,(u&3),t);

//        printf("parity match! t is: %x\n",t);
        return t;
    } else{
        uint8_t t=0; //ejemplo w=11111111 v=10101010 y u=00000000 => w&7 = 00000111, v&7=00000010 y u&3 = 00000000 => x=11101000
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

void test_lagrange(){
    uint8_t z[8]= {1, 2, 3, 4, 5, 6, 7, 8};
    uint8_t x[4][8]={{9,10,11,12,13,14,15,16},{17,18,19,20,21,22,24,25},{26,27,28,29,30,31,32,33},{34,35,36,37,38,39,40,41}};
    uint8_t y[4][8];
    uint8_t yInv[8][4];
    uint8_t xInv[8][4];
    uint8_t s[8];
    uint8_t k=4;
    uint8_t length=8;
    uint8_t polynomial[k];
    uint8_t blockNumber =0;

    for (int i = 0; i < length; ++i) {
        s[i]=0;
        for (int j = 0; j < k; ++j) {
            y[j][i]=0;
            yInv[j][i]=0;
            xInv[i][j]=x[j][i];
        }
    }


    // blaqoe 1 es s{1,2,3,4} bloque 2 s{5,6,7,8}
    //X0,0=9 X0,1=10 x1,0=17
    for (int i = 0; i < length; i+=k) {
        for(int j = 0; j < k; j++){
            // guardamos el valor decimal del pixel en el polinomio correspondiente
            polynomial[j] = z[i + j];
        }
        for (int j = 0; j < k; ++j) {
            for (int l = 0; l <k; ++l) {
                y[j][blockNumber]= suma_galois(y[j][blockNumber], mult_galois(polynomial[l], pot_galois(x[j][blockNumber], l)));
                yInv[blockNumber][j]= suma_galois(yInv[blockNumber][j], mult_galois(polynomial[l], pot_galois(x[j][blockNumber], l)));
            }
        }
        blockNumber++;
    }

    for (int i = 0; i < length; i+=k) {
        s[i]= lagrange(k, xInv[i/k], yInv[i/k]);
            for (int j = 1; j < k; ++j) {
                for (int l = 0; l < k; ++l) {
                    if (x[i/k][l] != 0) {
                        yInv[i/k][l] = div_galois(suma_galois(yInv[i/k][l], s[i+j-1]), xInv[i/k][l]);
                    } else {
                        yInv[i/k][l] = div_galois(suma_galois(yInv[i/k][l], s[i+j-1]), 255);;
                    }
                }
                s[i + j] = lagrange(k - j, xInv[i/k], yInv[i/k]);
            }
    }
    for (int i = 0; i < length; ++i) {
        printf("%d, ",s[i]);
    }


}