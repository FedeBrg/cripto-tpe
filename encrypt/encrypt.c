//
// Created by manuel.rodriguez on 09/05/2021.
//

#include "../lagrange/lagrange.h"
#include "../galois/galois.h"
#include "../manejo_bmp/bmp_handling.h"
#include "encrypt.h"
#include <math.h>

// pasar int a decimal y almacena en out
void int_to_bin_digit(unsigned int in, int count, int* out)
{
    unsigned int mask = 1U << (count-1);
    int i;
    for (i = 0; i < count; i++) {
        out[i] = (in & mask) ? 1 : 0;
        in <<= 1;
    }
}

// pasar de binario a decimal
int bin_to_dec(int src[], int bits){
    int i, n, sum = 0;
    for (i = 0; i < bits; i++) {
        n = src[i];
        sum += (n * (1 << (bits - (i + 1))));
    }
    return sum;
}

// determina la paridad de un binario
int get_parity(int src[]){
    int counter = 0; 

    for(int i = 0; i < 8; i++){
        if(src[i] == 1){
            counter++;
        }
    }

    return counter % 2;
}

// Funcion de encripcion, la escribo toda aca, despues hay que modularizarlo
ImageBMP * * encrypt(uint8_t n, uint8_t k, char * directoryName, ImageBMP * secretImageBmp){
    
    // de la imagen secreta, tomo sus pixeles
    uint8_t * secretImage = secretImageBmp->pixels;

    // la cantidad de pixeles
    uint32_t size = secretImageBmp->header.biHeight * secretImageBmp->header.biWidth;

    // determino la cantidad de bloques de la imagen secreta
    uint32_t secretBlocksQuantity = size/k;

    // ######## TODO ESTO ES PORQUE NO ME ANDA EL READ_BMPS, LO DEJO PARA SEGUIR PROBANDO YO ########

    /*// leo las imagenes portadoras
    ImageBMP * portadora1 = read_bmp("../manejo_bmp/test_bmps/Alfred.bmp");
    ImageBMP * portadora2 = read_bmp("../manejo_bmp/test_bmps/Eva.bmp");
    ImageBMP * portadora3 = read_bmp("../manejo_bmp/test_bmps/Facundo.bmp");
    ImageBMP * portadora4 = read_bmp("../manejo_bmp/test_bmps/Gustavo.bmp");
    ImageBMP * portadora5 = read_bmp("../manejo_bmp/test_bmps/Marilyn.bmp");
    ImageBMP * portadora6 = read_bmp("../manejo_bmp/test_bmps/James.bmp");

    // hago el array de punteros con la imagenes, aca despues hay que hacer el read_bmps
    ImageBMP * carriers[n];
    carriers[0] = portadora1;
    carriers[1] = portadora2;
    carriers[2] = portadora3;
    carriers[3] = portadora4;
    carriers[4] = portadora5;
    carriers[5] = portadora6;*/

    // ######## TODO ESTO ES PORQUE NO ME ANDA EL READ_BMPS, LO DEJO PARA SEGUIR PROBANDO YO ########

    // leo las bmps que seran utilizadas como portadoras
    ImageBMP ** carriers = read_bmps(directoryName, n);

    // defino el polinomio que luego sera evaluado
    uint8_t polynomial[k];

    // variable auxiliar para mantener nocion de la cantidad de bloques que tiene la imagen
    uint32_t blockNumber = 0;

    // array de punteros que luego tendran los bloques de cada una de las n portadoras
    uint8_t ** allCarriersBlocks[n];

    // el flag nos va a servir para marcar cuando cambiamos un valor del pixel X y asi poder escribirlo
    uint8_t xValueChanged = 0;

    // genero un array de n posiciones con los bloques en modo XWVU de todas las portadoras
    for(int i = 0; i < n; i++){
        allCarriersBlocks[i] = split_portadora(carriers[i]);
    }

    // con las lineas de abajo vamos a calcular las F(x) de cada bloque de la imagen secreta

    // mientras estemos dentro del tamaño de la imagen...
    for(int f = 0; f < size; f += k){

        // por cada k bytes de la imagen secreta (un bloque)
        for(int j = 0; j < k; j++){
            // guardamos el valor decimal del pixel en el polinomio correspondiente
            polynomial[j] = secretImage[f + j];
        }

        // llegando a este punto, tenemos todas las F guardadas en el array allPolynomial
        // tenemos N imagenes portadoras. hay que evaluar cada F en el bloque XWVU correspondiente de cada imagen

        // declaro una variable para llevar la cuenta del valor de F(Xi)
        uint32_t polynomialEvaluated = 0;

        // es un array de arrays de dos posiciones, que van a ser todos los pares <Xi, F(Xi)>, para las n portadoras
        // por cada bloque hay un X, por lo que va a haber un par por bloque
        uint8_t pairs[n][secretBlocksQuantity][2];

        // obtenemos el tamaño de una imagen portadora, asumiendo que todos los tamaños son iguales, calculo solo con la primera
        //uint32_t totalCarrierBlocks = carriers[0]->header.biWidth * carriers[0]->header.biHeight / 4;

        // con estos 4 pixeles, el primero es X, el segundo W, el tercero V y el cuarto U.
        // tenemos que agarrar los bits de X y evaluarlos en el polinomio. en este caso el primer polinomio porque estamos usando el primer bloque

        // entonces, por cada valor de x, tenemos que evaluar el polinomio correspondiente
        // por cada portadora que esta en allCarriersBlocks
        for(int i = 0; i < n; i++){

            // obtengo el valor de x de la i-esima portadora
            uint8_t xValue = allCarriersBlocks[i][blockNumber][0];

            // con este for vamos a verificar que el xValue de este bloque no sea igual a 
            // ningun otro xValue previamente analizado
            for(int u = 0; u < i; u++){
                if(allCarriersBlocks[u][blockNumber][0] == xValue){
                    // marcamos el flag para decir que cambiamos el xValue
                    xValueChanged = 1;

                    // incrementamos el xValue para que no sea igual al anterior
                    xValue += 1;
                    
                    // si es mayor o igual a 255 tengo que reiniciar el valor del pixel
                    if(xValue >= 255){
                        xValue = 0;
                    }

                    // si encuentro uno igual quiero volver a analizar con todos los pixeles anteriores
                    u = 0;
                }
            }

            // hago la evaluacion en p-esimo polinomio
            for(int u = 0; u < k; u++){
                polynomialEvaluated = suma_galois(polynomialEvaluated, mult_galois(polynomial[u], pot_galois(xValue, u)));
            }

            // guardo el valor del polinomio evaluado junto con el valor original del pixel X
            pairs[i][blockNumber][0] = xValue;
            pairs[i][blockNumber][1] = polynomialEvaluated;

            // reseteo el valor de la evaluacion de F(Xi)
            polynomialEvaluated = 0;

            // llegado este punto, tenemos en pairs[][] el par <Xi, F(Xi)> de la i-esima portadora

            // ahora tengo que convertir en bits los valores de los pixeles W, V y U
            // y dentro de ellos ir guardando los bits de X

            //obtengo el valor binario de F(Xi)
            int binaryFX[8];
            int_to_bin_digit(pairs[i][blockNumber][1], 8, binaryFX);

            // obtengo el valor de W y calculo el valor binario del mismo
            int binaryW[8];
            int_to_bin_digit(allCarriersBlocks[i][blockNumber][1], 8, binaryW);

            // dentro de W piso los ultimos 3 bits con el binario de F(Xi)
            binaryW[5] = binaryFX[0];
            binaryW[6] = binaryFX[1];
            binaryW[7] = binaryFX[2];

            // obtengo el valor de V y calculo el valor binario del mismo
            int binaryV[8];
            int_to_bin_digit(allCarriersBlocks[i][blockNumber][2], 8, binaryV);

            // dentro de V piso los ultimos 3 bits con el binario de F(Xi)
            binaryV[5] = binaryFX[3];
            binaryV[6] = binaryFX[4];
            binaryV[7] = binaryFX[5];

            // obtengo el valor de U y calculo el valor binario del mismo
            int binaryU[8];
            int_to_bin_digit(allCarriersBlocks[i][blockNumber][3], 8, binaryU);

            // dentro de U piso los ultimos 2 bits con el binario de F(Xi)
            binaryU[6] = binaryFX[6];
            binaryU[7] = binaryFX[7];

            // calculo la paridad de binaryFX y lo pongo en el binario de U
            int parity = get_parity(binaryFX);
            binaryU[5] = parity;

            // con estos binarios, ahora tengo que calcular el decimal
            // con el decimal calculado, tengo que pisar los bytes de blocksFromCarrier

            // tomo el blocksFromCarrier correspondiente, y piso los decimales de W,V y U
            allCarriersBlocks[i][blockNumber][1] = bin_to_dec(binaryW, 8);
            allCarriersBlocks[i][blockNumber][2] = bin_to_dec(binaryV, 8);
            allCarriersBlocks[i][blockNumber][3] = bin_to_dec(binaryU, 8);

            // si resulta que cambiamos el xValue, tenemos que guardar el nuevo valor en el pixel X
            if(xValueChanged){
                allCarriersBlocks[i][blockNumber][0] = xValue;
                xValueChanged = 0;
            }
        }

        // incrementamos el contador de bloque
        blockNumber++;
    }


    // cuando salgo de ese loop, ya tengo las portadoras con todos sus bloques modificados
    for(int i = 0; i < n; i++){
        // con los bytes modificados, llamo a la funcion que crea las portadoras modificadas
        uint8_t * mergedCarrier = merge_portadora(allCarriersBlocks[i], carriers[i]->header.biWidth, carriers[i]->header.biHeight);

        // piso los pixeles de la carrier image con los pixeles nuevos
        carriers[i] -> pixels = mergedCarrier;

    }

    return carriers;

}