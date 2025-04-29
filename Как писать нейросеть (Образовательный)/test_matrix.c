#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"
#include "tensor.h"

int init_suite1() {
	return 0;

}


int clean_suite1() {
	return 0;
}


void test_shape_cardinality() {
	Shape shape3 = {3, {5, 11, 1}};
	Shape shape1 = {1, {3}};
	Shape shape2 = {2, {7, 8}};

	CU_ASSERT(cardinality(&shape1) == 3);
	CU_ASSERT(cardinality(&shape2) == 56);
	CU_ASSERT(cardinality(&shape3) == 55);

}

void test_shape_ll() {
	Shape a = {3, {4,5,6}};
	Shape b = {3, {8,7,6}};
	Shape s;
	shape_ll(&s, &a, &b);
	CU_ASSERT(s.rank == 4);
	CU_ASSERT(s.dims[0] == 4);
	CU_ASSERT(s.dims[1] == 5);
	CU_ASSERT(s.dims[2] == 8);
	CU_ASSERT(s.dims[3] == 7);
}

void test_shape_lf() {
	Shape a = {3, {4,5,6}};
	Shape b = {2, {6,3}};
	Shape s;
	shape_lf(&s, &a, &b);
	CU_ASSERT(s.rank == 3);
	CU_ASSERT(s.dims[0] == 4);
	CU_ASSERT(s.dims[1] == 5);
	CU_ASSERT(s.dims[2] == 3);
}

void test_shape_ff() {
	Shape a = {2, {4,5}};
	Shape b = {2, {4,8}};
	Shape s;
	shape_ff(&s, &a, &b);
	CU_ASSERT(s.rank == 2);
	CU_ASSERT(s.dims[0] == 5);
	CU_ASSERT(s.dims[1] == 8);
}



static CU_TestInfo test_shape[] = {
	{"shape cardinality", test_shape_cardinality},
	{"shape ll", test_shape_ll},
	{"shape lf", test_shape_lf},
	{"shape ff", test_shape_ff},
	CU_TEST_INFO_NULL,
};

static void test_matrix_alloc() {
	Shape a = {3, {4, 7, 8}};
	Matrix* m = matrix_alloc(&a);
	CU_ASSERT(m != NULL);
	CU_ASSERT(m->shape.rank == a.rank);
	CU_ASSERT(m->shape.dims[0] = a.dims[0]);
	CU_ASSERT(m->shape.dims[1] = a.dims[1]);
	CU_ASSERT(m->shape.dims[2] = a.dims[2]);
	free(m);

}

static Matrix* m = NULL;
static Matrix* a = NULL;
static Matrix* b = NULL;

static int two_matrix_init() {
	Shape s = {2, {2, 2}};
	m = matrix_alloc(&s);
	if ( !m ) return 1;
	b = matrix_alloc(&s);
	if ( !b ) return 1;
	return 0;
}

static int ab_matrix_init() {
	Shape s = {2, {3, 3}};
	a = matrix_alloc(&s);
	if ( !a ) return 1;
	b = matrix_alloc(&s);
	if ( !b ) return 1;
	m = NULL;
	for (int i = 0; i < cardinality(&s); i++) {
		a->values[i] = i;
		b->values[i] = i+9;
	}
	return 0;
}


static int two_matrix_cleanup() {
	if ( m) free(m);
	m = NULL;
	if ( b) free(b);
	b = NULL;
	return 0;
}

static int three_matrix_cleanup() {
	if ( m ) free(m);
	if ( b ) free(b);
	if ( a ) free(a);
	return 0;
}

static void cleanup_result() {
	if ( m ) free(m);
	m = NULL;
}

static void test_matrix_add() {
	for (int i =0; i < cardinality(&b->shape); i++ )
		b->values[i] = i;
	matrix_add(m, b);
	for (int i = 0; i< cardinality(&b->shape); i++ ) 
		CU_ASSERT(b->values[i] == m->values[i]);
}

static void test_matrix_mul() {
	for (int i =0; i < cardinality(&b->shape); i++ )
		b->values[i] = i;
	matrix_mul(m, b, b);
	for (int i = 0; i< cardinality(&b->shape); i++ ) 
		CU_ASSERT(b->values[i]*b->values[i] == m->values[i]);
}

static void test_matrix_wrapped_sum() {
	b->shape.dims[1] = 1; /* уменьшим размерность до 2x1 */
	for (int i = 0; i < cardinality(&b->shape); i++ ) 
		b->values[i] = i;
	for(int i = 0; i < cardinality(&m->shape); i++)
		m->values[i] = 0;
	matrix_add(m, b);
	CU_ASSERT(cardinality(&m->shape) == 4);
	CU_ASSERT(cardinality(&b->shape) == 2);
	CU_ASSERT(m->values[0] == b->values[0]);
	CU_ASSERT(m->values[1] == b->values[1]);
	CU_ASSERT(m->values[2] == b->values[0]);
	CU_ASSERT(m->values[3] == b->values[1]);
	
}

static CU_TestInfo test_matrix[] = {
	{"matrix allocation", test_matrix_alloc},
	{"matrix addition", test_matrix_add},
	{"matrix elementwise mul", test_matrix_mul},
	{"matrix wrapped sum", test_matrix_wrapped_sum},
	CU_TEST_INFO_NULL,
};

static void test_pre_product() {
	CU_ASSERT(a->shape.rank == 2);
	CU_ASSERT(b->shape.rank == 2);
	CU_ASSERT(a->shape.dims[0] == 3);
	CU_ASSERT(a->shape.dims[1] == 3);
	CU_ASSERT(b->shape.dims[0] == 3);
	CU_ASSERT(b->shape.dims[1] == 3);
	CU_ASSERT(a->values[0] == 0);
	CU_ASSERT(a->values[1] == 1);
	CU_ASSERT(b->values[0] == 9);
	CU_ASSERT(b->values[1] == 10);

}

static void test_product_lf() {
	CU_ASSERT_FATAL(matrix_product_lf(&m, a, b) == 0);
	CU_ASSERT_FATAL(m->shape.rank == 2);
	CU_ASSERT(m->shape.dims[0] == a->shape.dims[0]);
	CU_ASSERT(m->shape.dims[1] == b->shape.dims[1]);
	CU_ASSERT(m->values[0] == 12+2*15);
	CU_ASSERT(m->values[1] == 13+2*16);
	CU_ASSERT(m->values[2] == 14.+2.*17.);
	CU_ASSERT(m->values[3] == 3.*9 + 4.*12 + 5.*15.);
	CU_ASSERT(m->values[4] == 3.*10 + 4.*13 + 5.*16.);
}

static void test_product_ll() {
	CU_ASSERT_FATAL(matrix_product_ll(&m, a, b) == 0);
	CU_ASSERT_FATAL(m->shape.rank == 2);
	CU_ASSERT(m->shape.dims[0] == a->shape.dims[0]);
	CU_ASSERT(m->shape.dims[1] == b->shape.dims[0]);
	CU_ASSERT(cardinality(&m->shape) == 9);
	CU_ASSERT(m->values[0] == 10+2*11);
}

static void test_product_ff() {
	CU_ASSERT_FATAL(matrix_product_ff(&m, a, b) == 0);
	CU_ASSERT_FATAL(m->shape.rank == 2);
	CU_ASSERT(m->shape.dims[0] == a->shape.dims[1]);
	CU_ASSERT(m->shape.dims[1] == b->shape.dims[1]);
	CU_ASSERT(cardinality(&m->shape) == 9);
	CU_ASSERT(m->values[0] == 3*12+6*15);
}



static CU_TestInfo test_dot_product[] = {
	{"pre-product test", test_pre_product},
	{"product ll", test_product_ll},
	{"product lf", test_product_lf},
	{"product ff", test_product_ff},
	CU_TEST_INFO_NULL,
};


void test_tensor_mat() {
	Tensor* tm = tensor_mat(a);
	CU_ASSERT_FATAL(tm != NULL);
	Matrix *r;
	CU_ASSERT_FATAL(tensor_forward(tm, &r) == 0);
	CU_ASSERT(memcmp(r->values, a->values, sizeof(float)*cardinality(&r->shape)) == 0);
	free(r);

}

void test_tensor_mm() {
	Tensor* x = tensor_mat(a);
	Tensor* y = tensor_mat(b);
	Tensor* z = tensor_mm(x, y);
	Matrix* f;
	CU_ASSERT_FATAL(tensor_forward(z, &f) == 0);
	CU_ASSERT_FATAL(f != NULL);
	CU_ASSERT_FATAL(matrix_product_ll(&m, a, b) == 0);
	CU_ASSERT(m->shape.rank == f->shape.rank);
	CU_ASSERT(memcmp(f->values, m->values, sizeof(float)*cardinality(&m->shape)) == 0);
	tensor_free(z);
}

void test_tensor_sum() {
	Tensor* x = tensor_mat(a);
	CU_ASSERT_FATAL(x != NULL);
	Tensor* y = tensor_mat(b);
	CU_ASSERT_FATAL(y != NULL);
	Tensor* z = tensor_sum(x, y);
	CU_ASSERT_FATAL(z != NULL);
	CU_ASSERT(x->refcount == 1);
	CU_ASSERT(y->refcount == 1);
	Matrix * f = NULL;
	CU_ASSERT_FATAL(tensor_forward(z, &f) == 0);
	CU_ASSERT_FATAL(f != NULL);
	m = matrix_alloc(&a->shape);
	CU_ASSERT_FATAL(m != NULL);
	CU_ASSERT_FATAL(matrix_sum(m, a, b) == 0);
	CU_ASSERT_FATAL(m->shape.rank == f->shape.rank);
	CU_ASSERT(memcmp(f, m, sizeof(Matrix) + sizeof(float)*cardinality(&m->shape)) == 0);
	for( int i = 0; i < cardinality(&m->shape); i++)
		m->values[i] = 1.0;
	
	z->refcount++;
	tensor_reset(z);
	CU_ASSERT_FATAL(x->proc == 1);
	CU_ASSERT_FATAL(y->proc == 1);

	CU_ASSERT(tensor_backward(z, m) == 0);
	CU_ASSERT_FATAL(x->grad != NULL);
	CU_ASSERT_FATAL(y->grad != NULL);
	CU_ASSERT(memcmp(x->grad, m, sizeof(Matrix) + sizeof(float)*cardinality(&m->shape)) == 0);
	CU_ASSERT(memcmp(y->grad, m, sizeof(Matrix) + sizeof(float)*cardinality(&m->shape)) == 0); 

}
	

static CU_TestInfo test_tensor[] = {
	{"tensor_mat", test_tensor_mat},
	{"tensor_sum", test_tensor_sum},
	{"tensor_mm", test_tensor_mm},
	CU_TEST_INFO_NULL,
};

static CU_SuiteInfo suites[] = {
	{"shape suite", NULL, NULL, NULL, NULL, test_shape},
	{"matrix suite", two_matrix_init, two_matrix_cleanup, NULL, NULL, test_matrix},
	{"dot product suite", ab_matrix_init, three_matrix_cleanup, NULL, cleanup_result, test_dot_product},
	{"tensor suite", ab_matrix_init, NULL, NULL, NULL, test_tensor},
	CU_SUITE_INFO_NULL,
};



int main() {
	CU_pSuite pSuite = NULL;
	if (CU_initialize_registry()) {
		fprintf(stderr, "CAnnot initialize registry\n");
		return -1;
	}
	fprintf(stderr, "Generating suites\n");
	CU_register_suites(suites);
	fprintf(stderr, "Starting...\n");
	CU_console_run_tests();
	CU_cleanup_registry();
	fprintf(stderr, "OK\n");
}

