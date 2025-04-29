#ifndef _OPTIMIZER_H_
#define _OPTIMIZER_H_
#include "tensor.h"


typedef struct _optimizer Optimizer;

Optimizer* optimizer_sgd(float alpha);
int optimizer_step(Optimizer*, Tensor* t);
void optimizer_free(Optimizer*);

#endif /* _OPTIMIZER_H_ */
