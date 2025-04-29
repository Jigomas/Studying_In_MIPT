#include <stdlib.h>
#include <stdio.h>
#include "tensor.h"
#include "matrix.h"
#include "optimizer.h"

struct _optimizer {
	int step;
	Optimize optfunc;
};

struct _sgd_optimizer {
	struct _optimizer base;
	float alpha;
};


int sgd_optimize_step(Matrix* params, Matrix* grad, void* data) {
	struct _sgd_optimizer* opt = (struct _sgd_optimizer*)data;
	if ( cardinality(&grad->shape) % cardinality(&params->shape) != 0 )
		return -fprintf(stderr, "Different params and grad shapes: %d (%dx%d) and %d (%dx%d)\n",
				params->shape.rank, params->shape.dims[0], params->shape.dims[1],
				grad->shape.rank, grad->shape.dims[0], grad->shape.dims[1]);
	int ps = cardinality(&params->shape);
	for( int i = 0; i< cardinality(&grad->shape); i++ ) {
		params->values[i % ps] -= opt->alpha* grad->values[i];
	}
	return 0;

}


Optimizer* optimizer_sgd(float alpha){
	struct _sgd_optimizer* opt = malloc(sizeof(struct _sgd_optimizer));
	if ( !opt ) return NULL;
	opt->base.step = 0;
	opt->base.optfunc = sgd_optimize_step;
	opt->alpha = alpha;
	return (Optimizer*)opt;
}

int optimizer_step(Optimizer* opt, Tensor* t) {
	opt->step++;
	tensor_optimize(t, opt->optfunc, (void*)opt);
	return 0;
}

void optimizer_free(Optimizer* opt) {
	free(opt);
}
