#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "tensor.h"
#include "matrix.h"

Tensor* tensor_alloc() {
	Tensor* r = aligned_alloc(64, (sizeof(Tensor)+63) & ~63);
	if ( !r ) return NULL;
	memset(r, 0, sizeof(Tensor));
	return r;
}

Tensor* mk_tensor0(Forward fw, Backward bw, Shape* shape){
	Tensor* r = tensor_alloc();
	if ( !r) return NULL;
	r->clear =1;
	r->forward = fw;
	r->backward = bw;
	if ( shape) r->params = matrix_alloc(shape);
	return r;
}

Tensor* mk_tensor1(Forward fw, Backward bw, Tensor* restrict a, Shape* shape) {
	Tensor* r = tensor_alloc();
	if ( !r) return NULL;
	r->forward = fw;
	r->backward = bw;
	r->nchildren = 1;
	r->children[0] = a;
	a->refcount++;
	if (shape) r->params = matrix_alloc(shape);
	return r;
}

Tensor* mk_tensor2(Forward fw, Backward bw, Tensor* restrict a, Tensor* restrict b, Shape* shape) {
	Tensor* r = tensor_alloc();
	if ( !r) return NULL;
	r->forward = fw;
	r->backward = bw;
	r->nchildren = 2;
	r->children[0] = a;
	r->children[1] = b;
	a->refcount++;
	b->refcount++;
	if (shape) r->params = matrix_alloc(shape);
	return r;
}


int tensor_forward(Tensor* restrict t, Matrix* restrict * out ) {
	if (!t->cache)
		t->forward(t, &t->cache);
	t->clear = 0;
	*out = t->cache;
	return 0;
}

int sum_forward(Tensor* restrict t, Matrix* restrict *out) {
	Matrix *a, *b;
	if (tensor_forward(t->children[0], &a)) return -1;
	if (tensor_forward(t->children[1], &b)) return -2;
	*out = matrix_alloc(&a->shape);
	matrix_sum(*out, a, b);
	return 0;
}


int sum_backward(Tensor* restrict t, Matrix* restrict grad) {
	tensor_backward(t->children[1], grad);
	tensor_backward(t->children[0], grad);
	return 0;
}

/* общая схема обратного распространения

	tensor_reset(root); // рекурсивно сбрасываем состояние 
	tensor_backward(root); // 
	tensor_update(root); // обновляем значения через оптимизатор optimizer
	Tensor* queue = NULL;

	}



*/

int  tensor_backward(Tensor* t, Matrix* grad) {
	/* Накапливаем grad, backward должен вызваться не более refcount после reset */
	if ( t->proc-- > 0) {
		if ( grad) {
			if ( !t->grad ) 
				t->grad = matrix_alloc(&grad->shape);
						
			matrix_add(t->grad, grad);
		}
		/* Как только достигли refcount, спускаемся дальше */
		if ( t->proc == 0 && t->backward)
			return t->backward(t, t->grad);
	}
	return 0;
}

void tensor_reset(Tensor* t) {
	t->proc = t->refcount;
	if (t->clear) {
		fprintf(stderr, "Tensor %p already cleared\n", t);
		return;
	}

	if (t->grad ) {
		free(t->grad);
		t->grad = NULL;
	}
	t->clear = 1;
	for (int c = 0; c < t->nchildren; c++ )
		tensor_reset(t->children[c]);
	
}

void tensor_clear(Tensor* t) {
	t->proc = t->refcount;
	if (t->grad ) {
		free(t->grad);
		t->grad = NULL;
	}
	if ( t->cache ) {
		free(t->cache);
		t->cache = NULL;
	}
	t->clear = 1;
	for (int c = 0; c < t->nchildren; c++ )
		tensor_clear(t->children[c]);
	
}




Tensor* tensor_sum(Tensor* a, Tensor* b) {
	Tensor* r = mk_tensor2(sum_forward, sum_backward, a, b, NULL);
	return r;
}

int mul_forward(Tensor* t,  Matrix* restrict *out) {
	Matrix* a, *b;
	tensor_forward(t->children[0], &a);
	tensor_forward(t->children[1], &b);
	*out = matrix_alloc(&a->shape);
	matrix_mul(*out, a, b);
	return 0;
}

int mul_backward(Tensor* t, Matrix* restrict grad) {
	Matrix* a, *x;
	a = matrix_alloc(&grad->shape);
	for (int i= 0; i < 2; i++) {
		tensor_forward(t->children[i], &x);
		matrix_mul(a, x, grad);
		tensor_backward(t->children[1-i], a);
	}
	return 0;
}

Tensor* tensor_mul(Tensor* a, Tensor* b) {
	Tensor* r = mk_tensor2(mul_forward, mul_backward, a, b, NULL);
	return r;
}


/* out = sigma(inp + t)
 * backward = grad*sigma', local_grad =grad*sigma'
 */
int sigma_forward(Tensor* t, Matrix* restrict *out) {
	Matrix* a;
	tensor_forward(t->children[0], &a);
	Matrix* r = matrix_alloc(&a->shape);
	for( int i =0; i< cardinality(&r->shape); i++ ) 
		r->values[i] = 1.0/(1.0+expf(-a->values[i]));
	*out = r;
	return 0;
}

/* Размерность grad: Batch,Sigma... */
int sigma_backward(Tensor* t, Matrix* restrict grad) {
	Matrix* b = matrix_alloc(&grad->shape);
	Matrix* out;
	tensor_forward(t, &out); // используем кеш
	for(int i = 0; i < cardinality(&b->shape); i++) { 
		b->values[i] = out->values[i]*(1.0-out->values[i])*grad->values[i];
	}
	tensor_backward(t->children[0], b);
	free(b);
	return 0;
}


int tanh_forward(Tensor* t, Matrix* restrict * out) {
	Matrix* a;
	tensor_forward(t->children[0], &a);
	Matrix* r = matrix_alloc(&a->shape);
	for (int i = 0; i< cardinality(&r->shape); i++)
		r->values[i] = tanhf(r->values[i]);
	*out = r;
	return 0;
}

int tanh_backward(Tensor* t, Matrix* restrict grad) {
	Matrix* b = matrix_alloc(&grad->shape);
	Matrix* out;
	tensor_forward(t, &out);
	for(int i = 0; i < cardinality(&b->shape); i++) 
		b->values[i] = (1.0 - out->values[i]*out->values[i])*grad->values[i];
	tensor_backward(t->children[0], b);
	free(b);
	return 0;
} 

Tensor* tensor_sigma(Tensor* a) {
	return mk_tensor1(sigma_forward, sigma_backward, a, NULL);
}

Tensor* tensor_tanh(Tensor* a) {
	return mk_tensor1(tanh_forward, tanh_backward, a, NULL);
}

int mse_forward(Tensor* t, Matrix* restrict *out) {
	Matrix *a __attribute__((aligned(64)));
	Matrix *b __attribute__((aligned(64)));
	tensor_forward(t->children[0], &a);
	tensor_forward(t->children[1], &b);
	if ( cardinality(&a->shape) != cardinality(&b->shape) ) 
		return -fprintf(stderr, "Shapes are different for tensors %p and %p\n",
				t->children[0], t->children[1]);
	Shape out_shape = {1, {1}}; // скаляр
	*out = matrix_alloc(&out_shape);
	int batch_size = a->shape.dims[0];
	for ( int i = 0; i < cardinality(&a->shape); i++) {
		(*out)->values[0] += (a->values[i] - b->values[i])*(a->values[i] - b->values[i]);
	}
	(*out)->values[0] /= batch_size;

	return 0;
}

int mse_backward(Tensor* t, Matrix* restrict grad) {
	Matrix* a, *b;
	tensor_forward(t->children[0], &a); // outputs
	tensor_forward(t->children[1], &b); // targets
	if ( cardinality(&a->shape) != cardinality(&b->shape) ) 
		return -fprintf(stderr, "Shapes are different for tensors %p and %p\n",
				t->children[0], t->children[1]);
	Matrix* g = matrix_alloc(&a->shape);
	for (int i = 0; i < cardinality(&a->shape); i++)
		g->values[i] = 2*(b->values[i] - a->values[i]);
	tensor_backward(t->children[0], g); // Транслируем только первому ребенку (outputs)
	//tensor_backward(t->children[1], g);
	free(g);
	return 0;
}


Tensor* tensor_mse(Tensor *a, Tensor* b) {
	return mk_tensor2(mse_forward, mse_backward, a, b, NULL);
}

int relu_forward(Tensor* t, Matrix* restrict * out) {
	Matrix* a;
	tensor_forward(t->children[0], &a);
	Matrix* r = matrix_alloc(&a->shape);
	for(int i =0; i < cardinality(&a->shape); i++) 
		r->values[i] = a->values[i]>0?a->values[i]:0;
	*out = r;
	return 0;
}


int relu_backward(Tensor* t, Matrix* grad) {
	Matrix* b = matrix_alloc(&grad->shape);
	Matrix* out;
	tensor_forward(t, &out);
	for(int i = 0; i<cardinality(&b->shape); i++)
		b->values[i] = (out->values[i]>0)*grad->values[i];
	tensor_backward(t->children[0], b);
	free(b);
	return 0;
}

Tensor* tensor_relu(Tensor* a) {
	return mk_tensor1(relu_forward, relu_backward, a, NULL);
}


void tensor_free(Tensor* t) {
	if ( !t) return;
	if ( --t->refcount ) return;
	if (t->cache) free(t->cache);
	if (t->params) free(t->params);
	if (t->grad) free(t->grad);
	for( int i = 0; i < t->nchildren; i++)
		if (t->children[i]) tensor_free(t->children[i]);
	free(t);
}

int m_forward(Tensor* restrict t, Matrix * restrict *out) {
	Matrix* r;
	if (!t->params) return -fprintf(stderr, "No params for output tensor %p\n", t);
	Shape s __attribute__((aligned(64))) = t->params->shape;
	r = matrix_alloc(&s);
	matrix_add(r, t->params);
	*out = r;
	return 0;
}

int m_backward(Tensor* restrict t, Matrix* restrict grad) {
	//matrix_add(t->params, t->grad);
	return 0;
}

Tensor* tensor_mat(Matrix* restrict  x) {
	Tensor* t = mk_tensor0(m_forward, m_backward, NULL);
	t->params = x;
	return t;
}
/* mmfs[B,O] = inp[B, I] <ll> w[O, I]  */

int mm_forward(Tensor* restrict t, Matrix* restrict* out) {
	Matrix* a, *b;
	tensor_forward(t->children[0], &a);
	tensor_forward(t->children[1], &b);
	if(matrix_product_ll(out, a, b)) return -fprintf(stderr, "Error while llprod at %p\n", t);
	return 0;
}
/** Grad[B, O]
 *
 * inpGrad[B, I] = Grad[B,O] <lf> w[O, I]
 * wGrad[O,I] = Grad[B, O] <ff> inp[B, I]
 */
int mm_backward(Tensor* restrict t, Matrix* restrict grad) {
	Matrix* a, *b, *r;
	tensor_forward(t->children[0], &a);
	tensor_forward(t->children[1], &b);
	if (matrix_product_lf(&r, grad, b)) return -fprintf(stderr, "Cannot make lf product for %p\n", t);

	tensor_backward(t->children[0], r);
	free(r);

	if ( matrix_product_ff(&r, grad, a)) return -fprintf(stderr, "Cannot make ff product for %p\n", t);

	tensor_backward(t->children[1], r);
	free(r);
	return 0;
}


Tensor* tensor_mm(Tensor* a, Tensor* b) {
	return mk_tensor2(mm_forward, mm_backward, a, b, NULL);	
}


void tensor_optimize(Tensor* t, Optimize opt, void* data) {
	if ( !t->grad ) return;
	if ( t->params) {
		if( opt(t->params, t->grad, data) )
			fprintf(stderr, "Error while optimizing %p\n", t);
		if ( t->cache ) {
			free(t->cache);
			t->cache = NULL;
		}
	}
	/* Сбрасываем градиент */
	free(t->grad);
	t->grad = NULL;
	for ( int i = 0; i < t->nchildren; i++)
		tensor_optimize(t->children[i], opt, data);

}

void tensor_set_params(Tensor* t, Matrix* params) {
	if ( t->params ) free(t->params);
	if ( t->cache && t->cache != t->params) free(t->cache);
	t->clear = 1;
	t->cache = NULL;
	t->params = params;
}
