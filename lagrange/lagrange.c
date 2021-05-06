
#include <stdio.h>
#include "lagrange.h"


int main(){
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
}


//https://en.wikipedia.org/wiki/Lagrange_polynomial

double lagrange(int k, double X[], double Y[]){

	double prod = 1;
	double sum = 0;

	for (int i = 0; i < k; i++){
		
		for (int q = 0; q < k; q++){
			if (i != q){

				prod *= (-X[q]) / (X[i] - X[q]);
			}
			
		}

		sum += (Y[i] * prod);

		prod = 1;


	}


	return sum;

}