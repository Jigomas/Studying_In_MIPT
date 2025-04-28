#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "ft.h"


int main(int argc, char* argv[]) {
	int width, height;
	complex double* f;
	scanf("%d", &width);
	scanf("%d", &height);
	f = calloc(width*height, sizeof(complex double));
	for (int i = 0; i < width*height; i++) {
		double x;
		scanf("%lf", &x);
		f[i] = x;
	}
	fft2d(f, width, height, argc>0?atoi(argv[1]):0);
	for ( int y = 0; y < height; y++ ){
		for( int x = 0; x < width; x++ ) {
			printf("%lf ", cabs(f[y*width+x]));
		}
		puts("");
	}
	free(f);
	return 0;
}


