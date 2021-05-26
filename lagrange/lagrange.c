
#include "lagrange.h"


/*int main(){
int n = 3;
 	double x[(2*n)-1];
 	double y[(2*n)-1];
 	int c = 0;
 	
 	for (int i = -n; i <= n; i++){
 		if (i!=0){
 			x[c] = i;
 			y[c] = 10+(2*i)-(3*i*i)+(5*i*i*i);
 			printf("y(%d) = %f \n",i,y[c] );
 			c++;
 		}
 		
 	}

	printf("%f\n",lagrange((2*n)-1,x,y) );
}*/


//https://en.wikipedia.org/wiki/Lagrange_polynomial

uint8_t lagrange(int k, uint8_t X[], uint8_t Y[]){

	uint8_t prod = 1;
	uint8_t sum = 0;
    int counter=0;
	for (int i = 0; i < k; i++){
		
		for (int q = 0; q < k; q++){
			if (i != q){
                //printf("A\n");
				uint8_t a = suma_galois(X[i], -X[q]);
				//printf("B\n");
				//printf("%d\n",a );
                if (a!=0){
                    uint8_t b = div_galois(-X[q],a);
                    //printf("C\n");
                    prod = mult_galois(prod,b);
                    //printf("D\n");
                }else{
                    counter++;
                }

			}
			
		}
//		if (counter!=0)
//        printf("%d ",counter);

		uint8_t c = mult_galois(Y[i], prod);
		sum = suma_galois(sum, c);


		prod = 1;


	}


	return sum;

}

uint8_t lagrange_turbina(int k,int r, uint8_t X[], uint8_t Y[]){

    uint8_t prod = 1;
    uint8_t sum = 0;
    int counter=0;
    for (int i = 0; i < k-r; i++){

        for (int q = 0; q < k-r; q++){
            if (i != q){
                //printf("A\n");
                uint8_t a = suma_galois(X[i], -X[q]);
                //printf("B\n");
                //printf("%d\n",a );
                if (a!=0){
                    uint8_t b = div_galois(-X[q],a);
                    //printf("C\n");
                    prod = mult_galois(prod,b);
                    //printf("D\n");
                }else{
                    counter++;
                }

            }

        }
//		if (counter!=0)
//        printf("%d ",counter);

        uint8_t c = mult_galois(Y[i], prod);
        sum = suma_galois(sum, c);


        prod = 1;


    }

    sum= mult_galois(sum,((k-1-r)%2==0?1:-1));
    return sum;

}