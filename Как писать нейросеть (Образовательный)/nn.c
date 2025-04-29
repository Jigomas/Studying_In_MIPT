#include <math.h>
#include <stdio.h>
#include <stdlib.h>


enum activation {
	ACT_SIGMA = 1,
	ACT_TANH = 2,
	ACT_SOFTMAX = 3,
	ACT_RELU = 4

};


typedef struct _actfunc {
	int (*forward)(float* restrict x, size_t n);
	int (*backward)(float* restrict  x, size_t n);
} actfunc;

int sigma_forward(float* restrict x, size_t n) {
	for (size_t i = 0; i < n; i++)
		x[i] =  1.0/(1+expf(-x[i]));
	return 0;
}

int sigma_backward(float*  restrict x, size_t n) {
	for (size_t i = 0; i < n; i++ ) 
		x[i] = x[i]*(1-x[i]);
	return 0;
}


struct _actfunc activations[] = {
	[1] = {sigma_forward, sigma_backward},
	//[2] = {tanh_forward, tanh_backward},
	//[3] = {softmax_forward, softwax_backward},
	//[4] = {relu_forward, relu_backward}
};

typedef struct _layer {
	size_t inputs;
	size_t outputs;
	enum activation actfunc;
	float params[] __attribute__((aligned(64)));  //(inputs+1)*outputs

} Layer __attribute__((aligned(64)));


float normal_distrib() {
	float d = 0.0;
	for( int i = 0; i < 5; i++ ) {
		d += (float)(rand()-RAND_MAX/2)/(float)RAND_MAX;
	}
	return d;
}

Layer * create_layer(size_t inputs, size_t outputs, enum activation act) {
	Layer* l = aligned_alloc(64, (inputs+1)*outputs*sizeof(float)+sizeof(Layer)+16);
	if ( !l ) return NULL;
	*l = (Layer){.inputs = inputs, .outputs=outputs, .actfunc = act};
	for( size_t i = 0; i < inputs+1; i++ )
		for(size_t o = 0; o < outputs; o++ )
			l->params[o*inputs+i] = normal_distrib();			

	return l;
}

int layer_forward(Layer* restrict l, float *restrict inputs, float* restrict outputs) {
	for (size_t o = 0; o < l->outputs; o++ ) {
		outputs[o] = 0.0;
		for (size_t i = 0; i < l->inputs; i++ )
			outputs[o] += l->params[o*l->inputs + i]*inputs[i];
		outputs[o] += l->params[o*(l->inputs+1)]; /** add bias */
	}
	activations[l->actfunc].forward(outputs, l->outputs);
	return 0;
}




int main() {
	Layer *l = create_layer(3, 1, ACT_SIGMA);
	if ( !l ) {
		fprintf(stderr, "Cannot create layer\n");
		return -1;
	}
	float x[3] __attribute__((aligned(64)))  = {0.0, 0.0, 0.0};
	float y[1] __attribute__((aligned(64)));
	layer_forward(l, x, y);
	printf("x1 = %f x2 =%f x3=%f y=%f\n",x[0], x[1], x[2], y[0]);
	return 0;

}
