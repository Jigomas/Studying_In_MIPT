#ifndef _TENSOR_H_
#define _TENSOR_H_

#include "matrix.h"


typedef struct _tensor {
	int (*forward)(struct _tensor* restrict t, Matrix* restrict *out);
	int (*backward)(struct _tensor* restrict t, Matrix* restrict grad);
	int clear;
	int proc;
	int nchildren __attribute__((aligned(64)));
	int refcount __attribute__((aligned(64)));
	Matrix * grad __attribute__((aligned(64)));
	Matrix * params __attribute__((aligned(64)));
	Matrix * cache __attribute__((aligned(64)));
	struct _tensor* children[2] __attribute__((aligned(64)));
} Tensor __attribute__((aligned(64)));

typedef int (*Forward)(Tensor* restrict t, Matrix* restrict *out);
typedef int (*Backward)(Tensor* restrict t, Matrix* restrict grad);
typedef int (*Optimize)(Matrix* params, Matrix* grad, void* opdata);


void tensor_reset(Tensor* t);

Tensor* tensor_sum(Tensor* a, Tensor* b);

Tensor* tensor_mul(Tensor* a, Tensor* b);

Tensor* tensor_mm(Tensor* a, Tensor* b);

Tensor* tensor_const(Matrix* x);

Tensor* tensor_sigma(Tensor* a);

Tensor* tensor_tanh(Tensor* a);

Tensor* tensor_softmax(Tensor* a);

Tensor* tensor_relu(Tensor* a);

Tensor* tensor_mat(Matrix * restrict x);


Tensor* tensor_mse(Tensor* outputs, Tensor* targets);

void tensor_clear(Tensor* t);

void tensor_set_params(Tensor* t, Matrix* p);

void tensor_free(Tensor* t);

void tensor_optimize(Tensor* t, Optimize opt, void* opdata);

int tensor_forward(Tensor* restrict t, Matrix* restrict* out);
int tensor_backward(Tensor* restrict t, Matrix* restrict grad);
#endif /* _TENSOR_H_ */
