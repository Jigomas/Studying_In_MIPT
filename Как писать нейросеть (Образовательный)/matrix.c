#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "matrix.h"



int cardinality(Shape* x) {
	if ( !x->rank ) return 0;
	int c = 1;
	for(int i = 0; i< x->rank && i< 10; i++ ) 
		c *= x->dims[i];
	return c;
}


int shape_ll(Shape* r, Shape* a, Shape* b) {
	if ( a->dims[a->rank-1] != b->dims[b->rank-1]) 
		return -fprintf(stderr, "Shapes dont conform: %d (%dx%d) and %d (%dx%d) for ll\n",
				a->rank, a->dims[0], a->dims[1],
				b->rank, b->dims[0], b->dims[1]);

	r->rank = 0;
	for(int i = 0; i < a->rank-1; i++) r->dims[r->rank++] = a->dims[i];
	for(int i = 0; i < b->rank-1; i++) r->dims[r->rank++] = b->dims[i];
	return 0;
}

int shape_lf(Shape* r, Shape* a, Shape* b) {
	if ( b->rank < 2 || a->dims[a->rank-1] != b->dims[b->rank-2])
		return -fprintf(stderr, "Shapes dont conform: %d (%dx%d) and %d (%dx%d) for lf\n",
				a->rank, a->dims[0], a->dims[1],
				b->rank, b->dims[0], b->dims[1]);
	r->rank = 0;
	for(int i = 0; i < a->rank-1; i++) r->dims[r->rank++] = a->dims[i];
	for(int i = 0; i < b->rank; i++)
		if ( i != b->rank-2) r->dims[r->rank++] = b->dims[i];
	return 0;
}

int shape_ff(Shape* r, Shape* a, Shape* b) {
	if ( a->dims[0] != b->dims[0] ) return -1;
	r->rank = 0;
	for (int i = 1; i < a->rank; i++) r->dims[r->rank++] = a->dims[i];
	for (int i = 1; i < b->rank; i++) r->dims[r->rank++] = b->dims[i];
	return 0;
}

int matrix_add(Matrix* restrict a, Matrix* restrict b) {
	int bc = cardinality(&b->shape);
	for( int i = 0; i < cardinality(&a->shape); i++ ) 
		a->values[i] += b->values[i % bc ];
	return 0;
}

int matrix_sum(Matrix* restrict r, Matrix* restrict a, Matrix* restrict b) {
	int bc = cardinality(&b->shape);
	for (int i = 0; i < cardinality(&a->shape); i++ )
		r->values[i] = a->values[i]+b->values[i % bc];
	return 0;
}


int matrix_mul(Matrix* restrict r, Matrix* restrict a, Matrix* restrict b) {
	int bc = cardinality(&b->shape);
	for (int i = 0; i < cardinality(&a->shape); i++) 
		r->values[i] = a->values[i]*b->values[i % bc];
	return 0;
}

Matrix* restrict  matrix_alloc(Shape* shape) {
	Matrix* m = aligned_alloc(64, (sizeof(Matrix)+cardinality(shape)*sizeof(float)+63) & ~63);
	if(!m) return NULL;
	m->shape = *shape;
	memset(m->values, 0, cardinality(shape)*sizeof(float));
	return m;
}




int matrix_product_lf(Matrix* restrict* r, Matrix* restrict a, Matrix* restrict b) {
	if( a->shape.dims[a->shape.rank-1] != b->shape.dims[b->shape.rank-2])
		return -1;
	int common = a->shape.dims[a->shape.rank-1];
	Shape shape;
	if (shape_lf(&shape, &a->shape, &b->shape)) return -1;
	Matrix* z = matrix_alloc(&shape);
	int bs = cardinality(&b->shape)/common;
	int as = cardinality(&a->shape)/common;
	for (int i = 0; i<as; i++)
		for (int j = 0; j<bs; j++) 
			for (int k = 0; k < common; k++) 
				z->values[i*bs+j] += a->values[i*common+k]*b->values[k*bs+j];
	*r = z;
	return 0;
}

int matrix_product_ll(Matrix* restrict* r, Matrix* restrict a, Matrix* restrict b){
	Shape shape;
	if (shape_ll(&shape, &a->shape, &b->shape)) return -1;
	int common = a->shape.dims[a->shape.rank-1];
	Matrix* z = matrix_alloc(&shape);
	int as = cardinality(&a->shape)/common;
	int bc = cardinality(&b->shape);
	int bs = bc/common;
	for (int i = 0; i<as; i++ )
		for (int j =0; j< bs; j++)
			for (int k = 0; k<common; k++) 
				z->values[i*bs+j] += a->values[common*i+k] * b->values[common*j+k];
			
	*r = z;
	return 0;
}

int matrix_product_ff(Matrix* restrict* r, Matrix* restrict a, Matrix* restrict b){
	Shape shape;
	if ( shape_ff(&shape, &a->shape, &b->shape)) return -1;
	int common = a->shape.dims[0];
	Matrix* z = matrix_alloc(&shape);
	int ac = cardinality(&a->shape)/common;
	int bc = cardinality(&b->shape)/common;
	for ( int i = 0; i < ac; i++ ) 
		for ( int j = 0; j< bc; j++) 
			for ( int k = 0; k < common; k++) 
				z->values[i*bc+j] += a->values[k*ac+i]*b->values[k*bc+j];
	*r = z;
	return 0;
}

int matrix_batch_mean(Matrix* restrict *r, Matrix* restrict a) {
	Shape shape = {0};
	for (int i = 1; i < a->shape.rank; i++ )
		shape.dims[shape.rank++] = a->shape.dims[i];
	*r = matrix_alloc(&shape);
	int common = a->shape.dims[0];
	int as = cardinality(&shape);
	for (int i = 0; i < as; i++ ) {
		for(int j = 0; j<common; j++)
			(*r)->values[i] += a->values[j*as+i];
		(*r)->values[i] /= common;
	}
	return 0;
}

Matrix* matrix_load(FILE* f) {
	Shape s;
	if (fscanf(f, "%d", &s.rank) != 1)
		return !fprintf(stderr, "Cannot read rank from file\n");
	if (s.rank > 10) 
		return !fprintf(stderr, "Wrong rank: %d\n", s.rank);
	for (int i = 0; i < s.rank; i++ ) {
		if (fscanf(f, "%d", &s.dims[i]) != 1)
			return !fprintf(stderr, "Cannot read dimension %d/%d\n", i, s.rank);
	}
	Matrix* m = matrix_alloc(&s);
	if ( !m ) return !fprintf(stderr, "Cannot allocate space for matrix\n");
	
	for ( int i = 0; i < cardinality(&s); i++) {
		if ( fscanf(f, "%f", &m->values[i]) != 1) {
			free(m);
			return !fprintf(stderr, "Cannot read value %d/%d\n", i, cardinality(&s));
		}
	}
	return m;


}

int matrix_save(Matrix* m, FILE* f) {
	fprintf(f, "%d", m->shape.rank);
	for(int i = 0; i < m->shape.rank; i++)
		fprintf(f, " %d", m->shape.dims[i]);
	fprintf(f, "\n");
	int c = cardinality(&m->shape);
	for(int i = 0; i<c; i++) 
		fprintf(f, "%f%c", m->values[i], (i%10 == 9 || i==c-1)?'\n':' '); 
	return 0;
}

int matrix_split_xy(Matrix* restrict m, Matrix* restrict *inp, Matrix* restrict *outp, int ycols) {
	int wcols = m->shape.dims[m->shape.rank-1];
	if (ycols >= wcols )
		return -fprintf(stderr, "Number of y columns %d not less than %d\n",
				ycols, m->shape.dims[m->shape.rank-1]);
	
	Shape x = {0};
	Shape y = {0};
	y.rank = x.rank = m->shape.rank;
	int xcols = wcols-ycols;
	for (int i = 0; i < m->shape.rank; i++ ) {
		x.dims[i] = i<m->shape.rank-1?m->shape.dims[i]:xcols;
		y.dims[i] = i<m->shape.rank-1?m->shape.dims[i]:ycols;
	}
	*inp = matrix_alloc(&x);
	if ( !*inp ) return -fprintf(stderr, "Cannot allocate input matrix\n");
	*outp = matrix_alloc(&y);
	if ( !*outp) return -fprintf(stderr, "Cannot allocate output matrix\n");

	for (int i = 0; i < cardinality(&m->shape)/wcols; i++) {
		for (int j = 0; j< xcols; j++) 
			(*inp)->values[i*xcols+j] = m->values[i*wcols+j];
		for (int j = 0; j < ycols; j++)
			(*outp)->values[i*ycols+j] = m->values[i*wcols+xcols+j];
	}
	return 0;
}

int matrix_slice(Matrix* restrict m, Matrix* restrict *slice, int start, int end) {
	if ( start < 0 || start > m->shape.dims[0]-1) return -1;
	if ( end <= start || end > m->shape.dims[0]) return -1;
	Shape s __attribute__((aligned(64))) = m->shape;
	s.dims[0] = end-start;
	int c = cardinality(&m->shape)/m->shape.dims[0];
	*slice = matrix_alloc(&s);
	if ( !*slice) return -1;
	memcpy((*slice)->values, m->values+c*start, c*(end-start)*sizeof(float));
	return 0;

}
