#ifndef _MATRIX_H_
#define _MATRIX_H_

typedef struct _shape {
	int rank;
	int dims[10];
} Shape;


typedef struct _matrix {
	Shape shape __attribute__((aligned(64)));
	float values[] __attribute__((aligned(64)));
} Matrix __attribute__((aligned(64)));


int cardinality(Shape* x);

int shape_ll(Shape* r, Shape* a, Shape* b);

int shape_lf(Shape* r, Shape* a, Shape* b);

int shape_ff(Shape* r, Shape* a, Shape* b);


int matrix_add(Matrix* restrict a, Matrix* restrict b);


int matrix_sum(Matrix* restrict r, Matrix* restrict a, Matrix* restrict b);


int matrix_mul(Matrix* restrict r, Matrix* restrict a, Matrix* restrict b);


Matrix* matrix_alloc(Shape* shape);


Matrix* matrix_load(FILE * f);
int matrix_save(Matrix* t, FILE* f);


int matrix_product_lf(Matrix* restrict* r, Matrix* restrict a, Matrix* restrict b);

int matrix_product_ll(Matrix* restrict* r, Matrix* restrict a, Matrix* restrict b);


int matrix_product_ff(Matrix* restrict* r, Matrix* restrict a, Matrix* restrict b);

int matrix_batch_mean(Matrix* restrict *r, Matrix* restrict a);

int matrix_split_xy(Matrix* restrict samples, Matrix* restrict *inp, Matrix* restrict *outp, int ycols);

int matrix_slice(Matrix* restrict orig, Matrix* restrict *slice, int slicestart, int sliceend);

#endif /* _MATRIX_H_ */
