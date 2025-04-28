#include <stdio.h>
#include <math.h>
#include <complex.h>
#include "ft.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"



int main(int argc, char* argv[]) {
	int width, height, channels;

	if (argc < 3 ) {
		fprintf(stderr, "Usage: %s <infile.png> <outfile.png>\n", argv[0]);
		exit(0);
	}
	unsigned char* image  = stbi_load(argv[1], &width, &height, &channels, 0);
	if( image == NULL ) {
		fprintf(stderr, "Cannot load image %s\n", argv[1]);
		exit(1);
	}

	unsigned char* outimage = calloc(width*height*channels, sizeof(unsigned char));

	/* Processing channels */
	for ( int c = 0; c < channels; c++ ) {
		complex double* f = calloc(width*height, sizeof(complex double));
		for (int y = 0; y < height; y++ ) {
			for (int x = 0; x < width; x++ ) {
				f[y*width + x] = image[(y*width + x)*channels + c];
			}
		}
		fft2d(f, width, height, 1);
		for (int y = 0; y < height; y++ ) {
			for ( int x = 0; x < width; x++ ) {
				outimage[(y*width + x)*channels + c] = cabs(f[y*width + x]);
			}
		}
		free(f);
	}
	/* writing outimage */
	if (!stbi_write_png(argv[2], width, height, channels, outimage, width*channels)) {
		fprintf(stderr, "cannot write png %s\n", argv[2]);
		stbi_image_free(image);
		free(outimage);
	}
	free(outimage);
	stbi_image_free(image);
	return 0;
}
