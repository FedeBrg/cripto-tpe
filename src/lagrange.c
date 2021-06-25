#include <lagrange.h>
#include <galois.h>


//https://en.wikipedia.org/wiki/Lagrange_polynomial

uint8_t lagrange(int k, uint8_t X[], uint8_t Y[]){

	uint8_t prod = 1;
	uint8_t sum = 0;
    int counter=0;
	for (int i = 0; i < k; i++){
		
		for (int q = 0; q < k; q++){
			if (i != q){
				uint8_t a = suma_galois(X[i], X[q]);
                if (a!=0){
                    uint8_t b = div_galois(X[q],a);
                    prod = mult_galois(prod,b);
                }else{
                    counter++;
                }

			}
			
		}

		uint8_t c = mult_galois(Y[i], prod);
		sum = suma_galois(sum, c);

		prod = 1;

	}

	return sum;

}
