#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "tensor.h"
#include "optimizer.h"

#define MIN(a, b) ((a)<(b)?(a):(b))

void matrix_rand(Matrix* m, float dev) {
	for (int i = 0; i < cardinality(&m->shape); i++)
		m->values[i] = 2*dev*(float)rand()/RAND_MAX-dev;
	
}


int main() {
	Shape hidden_shape = {2, {2, 3}};
	Shape ws2 = {2, {1, 2}};
	Shape bias_shape = {1, {2}};
	Shape output_bias_shape = {1, {1}};


	Matrix* hidden_wgts = matrix_alloc(&hidden_shape);
	Matrix* hidden_bias = matrix_alloc(&bias_shape);
	Matrix* output_wgts = matrix_alloc(&ws2);
	Matrix* output_bias = matrix_alloc(&output_bias_shape);
	matrix_rand(hidden_wgts, 1.1);
	matrix_rand(hidden_bias, 1.1);
	matrix_rand(output_wgts, 1.1);
	matrix_rand(output_bias, 1.1);
	FILE *fsample = fopen("examples.dat", "r");
	if ( !fsample) -fprintf(stderr, "Cannot open file examples\n");
	
	Matrix *samples_mat = matrix_load(fsample);
	fclose(fsample);
	
	Matrix* restrict inputs_mat, * restrict targets_mat;
	// Разделим матрицу на X и Y
	// последний столбец -- Y
	matrix_split_xy(samples_mat, &inputs_mat, &targets_mat, 1);
	fprintf(stderr, "X: %dx%d, Y: %dx%d\n", inputs_mat->shape.dims[0],
			inputs_mat->shape.dims[1],
			targets_mat->shape.dims[0],
			targets_mat->shape.dims[1]);

	Tensor* inputs = tensor_mat(NULL); // пока не заполняем
	
	/* Конструируем топологию (inputs x hidden) + hiddenbias) =>
	 * =>  relu => x outputs + output_bias => sigma =>
	 * => mse
	 */

	Tensor* relu_layer = tensor_relu(tensor_sum(
				tensor_mm(inputs, tensor_mat(hidden_wgts)),
				tensor_mat(hidden_bias)));


	Tensor* result = tensor_sigma(tensor_sum(
				tensor_mm(relu_layer, tensor_mat(output_wgts)),
				tensor_mat(output_bias)));

	Tensor* outputs = tensor_mat(NULL); // пока не заполняем
	Tensor* loss = tensor_mse(result, outputs);
	loss->refcount++;
	int batch_size = 4;
	int total_samples = samples_mat->shape.dims[0];

	Optimizer* optimizer = optimizer_sgd(-2.5);

	/* Цикл по батчам */
	for( int epoch = 0; epoch < 10000; epoch++ ) {
		for(int i = 0; i< total_samples; i+= batch_size) {
			Matrix* input_slice;
			Matrix* output_slice;
			// загружаем батч
			matrix_slice(inputs_mat, &input_slice, i, MIN(i+batch_size, total_samples));
			matrix_slice(targets_mat, &output_slice, i, MIN(i+batch_size, total_samples));
			// устанавливаем X и Y
			tensor_set_params(inputs, input_slice);
			tensor_set_params(outputs, output_slice);
			
			// Запускаем forward-вычисления
			// и накапливаем градиент
			tensor_clear(loss); // предварительно очищаем кеш
			Matrix* error;
			tensor_forward(loss, &error); // error должен быть скаляром
										  // но grad у loss -- вектор размера batch x outputs
			fprintf(stdout, "Epoch %d Batch %d Loss: %f\n", epoch, i, error->values[0]);
			// Начинаем обратное распространение градиента
			// loss сам умеет конструировать градиент
			tensor_backward(loss, NULL);
			tensor_reset(inputs); // сбрасываем градиент, чтобы матрица не обновлялась
			tensor_reset(outputs);
			// и подстраиваем веса
			optimizer_step(optimizer, result);
		}
		char buf[256];
		sprintf(buf, "./weights_%d.dat", epoch);
		FILE* qwe = fopen(buf, "w+");
		matrix_save(output_wgts, qwe);
		matrix_save(output_bias, qwe);
		matrix_save(hidden_wgts, qwe);
		matrix_save(hidden_bias, qwe);
		fclose(qwe);
	}
	tensor_free(loss); // выгружаем все тензоры рекурсивно
	optimizer_free(optimizer);
	return 0;
}

