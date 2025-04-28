#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>

size_t cap = 1024;
size_t n = 0;
double *signal;
double *cosine;
double *sine;

#define PI2 (2*M_PI)

int fft_inp(complex double* f, complex double* s, int n, int offset, int step) {
	if (n == 1) return 0;
	int step2 = 2*step;
	int n2 = n/2;
	fft_inp(f,s, n2, offset, step2);
	fft_inp(f,s, n2, offset+step, step2);
	complex double w = 1.0;
	complex double wk = cexp(PI2 * I / n);
	for (int i = 0;  i < n; i++) {
		s[offset+i*step] = f[(i % n2)*step2+offset] + w * f[offset+step + (i % n2)*step2];
		w *= wk;
	}
	for ( int i = 0; i < n; i++ ) f[offset+i*step] = s[offset+i*step]; 
	return 0;
}

int fft_inplace(complex double* f, size_t sz) {
	complex double *s = calloc(sz, sizeof(complex double));
	int res = fft_inp(f, s, sz, 0, 1);
	free(s);
	return res;
}

int fft(complex double* f, size_t sz) {
	if ( sz == 1 ) return 0;


	complex double* a = calloc(sz/2, sizeof(complex double));
	complex double* b = calloc(sz/2, sizeof(complex double));
	for(int i = 0; i < sz/2; i++) {
		a[i] = f[2*i];
		b[i] = f[2*i+1];
	}
	fft(a, sz/2);
	fft(b, sz/2);
	complex double w = 1.0;
	complex double wk = cexp(PI2* I / sz);
	for(int i = 0; i < sz; i++ ) {
		f[i] = a[i % (sz/2)] + w * b[i % (sz/2)];
		w *= wk;
	}
	free(a);
	free(b);
	return 0;
}


/** F(k, l) = |w^(k*x + l*y)| * f(x,y) 
 *
 *  F(k, l) = |w^(k*2m+l*y)| * f(2m, y) + |w^(k*(2m+1)+l*y)| * f(2m+1, y);
 */

/** Обычное (медленное) двумерное преобразование  Фурье */
int _ft2d(complex double* f, complex double* s, size_t w, size_t h) {
	complex double dwx = cexp(PI2*I/w);
	complex double dwy = cexp(PI2*I/h);
	complex double wyk = 1.0;
	complex double norm = 1.0/sqrt(w*h);
	for (int i = 0; i < h; i++ ) {
		int k = (h/2+i)%h;
		complex double wxk = 1.0;
		for (int j = 0; j < w; j++) {
			int l = (w/2+j)%w;
			int pos = k*w +l;
			s[pos] = 0.0;

			complex double wy = 1.0;
			for( int y = 0; y < h; y++ ) {
				complex double wx = 1.0;
				for ( int x = 0; x < w; x++ ) {
					s[pos] += wy*wx*f[y*w+x];
					wx *= wxk;
				}
				wy*=wyk;
			}
			//s[pos] *= norm;
			wxk *= dwx;
		}
		wyk *= dwy;
	}
	memcpy(f, s, w*h*sizeof(complex double));
	return 0;
}


int ft2d(complex double* f, size_t width, size_t height) {
	/** Инициализируем теневой буфер для промежуточных расчетов */
	complex double* s = calloc(width*height, sizeof(complex double));
	int res = _ft2d(f, s, width, height);
	free(s);
	return res;
}

/** Слайс -- описание среза массива */
typedef struct _slice {
	int offset; /**< Сдвиг */
	int step;   /**< Шаг */
	int n;      /**< Размер */
} slice;

/** Вычисляет координату по позиции в слайсе */
int slicen(slice* s, int i) {
	return s->offset + s->step *((i+s->n/2) % s->n);
}

/** функция вычисления позиции в массиве с учетом параметров слайсов */
int slice2d(int w, slice* sx, slice* sy, int i, int j) {
	return w*slicen(sy, i) + slicen(sx, j);
}

/** Копирует слайс и массива в массив */
void slicecpy(complex double* f, complex double* s, int w, slice sx, slice sy) {
	for (int i = 0; i < sy.n; i++) {
		for (int j = 0; j < sx.n; j++) {
			int pos = slice2d(w, &sx, &sy, i, j);
			f[pos] = s[pos];
		}
	}
}



int __ft2d(complex double* f, complex double* s, int w, slice sx, slice sy) {
	if ( sx.n == 1 && sy.n==1 ) {
		return 0;
	} 

	complex double dwx = cexp(PI2*I/sx.n);
	complex double dwy = cexp(PI2*I/sy.n);
	complex double wyk = 1.0;
	
	for (int i = 0; i < sy.n; i++ ) {
		complex double wxk = 1.0;
		for (int j = 0; j < sx.n; j++) {
			int pos = slice2d(w, &sx, &sy, i, j);
			s[pos] = 0.0;
			complex double wx = 1.0, wy = 1.0;
			for( int x = 0; x < sx.n; x++ ) {
				wy = 1.0;
				for ( int y = 0; y < sy.n; y++ ) {
					//int pos = slice2d(&sx, &sy, y, x);

					//fprintf(stderr, "In at %d is %lf, %lf: ", slice2d(&sx, &sy, y, x),  creal(f[slice2d(&sx, &sy, y, x)]),
					//	cimag(f[slice2d(&sx, &sy, y, x)]));
					s[pos] += wy*wx*f[slice2d(w, &sx, &sy, y, x)];
					wy*=wyk;
				}
				wx *= wxk;
			}
			//fprintf(stderr, "result at %d is %lf,%lf\n", pos,  creal(s[pos]), cimag(s[pos]));
			//s[pos] /= sqrt(sx.n*sy.n);
			wxk *= dwx;
		}
		wyk *= dwy;
	}
	slicecpy(f,s, w, sx, sy);
	return 0;

}


int _fft2dx(complex double* f, complex double* s, int w, slice sx, slice sy);

/** Разбивает множество точек на четные и нечетные по оси Y и делает БПФ */
int _fft2dy(complex double* f, complex double* s, int w, slice sx, slice sy) {
	if ( sy.n % 2 == 0) {
		/* Формируем четный и нечетный слайсы по оси Y */
		slice sy2 = (slice){sy.offset, sy.step*2, sy.n/2};
		slice sy2o = (slice){sy.offset+sy.step, sy.step*2, sy.n/2};
		/* Пробуем посчитать БПФ по оси Y */
		_fft2dx(f, s, w, sx, sy2);
		_fft2dx(f, s, w, sx, sy2o);
		/** Вращающий коэффициент по оси Y */
		complex double Ok = cexp(PI2*I/sy.n);
		complex double Om = 1.0;
		for (int i = 0; i < sy.n; i++ ) {
			for (int j = 0; j < sx.n; j++ ){
				s[slice2d(w, &sx, &sy, i, j)] = (f[slice2d(w,&sx, &sy2, i, j)] +Om * f[slice2d(w, &sx, &sy2o, i, j)]);
			}
			Om *= Ok;
		}
		/** Копируем значения слайса из теневого буфера обратно */
		slicecpy(f, s, w, sx, sy);
		return 0;
	} else {
		/* Если размер слайса не четный -- используем обычное ПФ */
		return __ft2d(f,s,w, sx, sy);
	}

}
/** Разбивает множество точек на четные и нечетный по оси X и делает БПФ */
int _fft2dx(complex double* f, complex double* s, int w, slice sx, slice sy) {
	if ( sx.n % 2 == 0 ) {
		/* Формируем четный и нечетный слайсы по оси X */
		slice sx2 = (slice){sx.offset, sx.step*2, sx.n/2};
		slice sx2o = (slice){sx.offset+sx.step, sx.step*2, sx.n/2};
		/* Попробуем посчитать БПФ по оси У */
		_fft2dy(f, s, w, sx2, sy);
		_fft2dy(f, s, w, sx2o, sy);
		/** Вращающий коэффициент по оси X */
		complex double Ok = cexp(PI2*I/sx.n);
		for ( int i = 0; i < sy.n; i++ ) {
			complex double Om  = 1.0;
			for ( int j = 0; j < sx.n; j++ ) {
				s[slice2d(w, &sx, &sy, i, j)] = (f[slice2d(w, &sx2, &sy, i, j)] + Om* f[slice2d(w, &sx2o, &sy, i, j)]);
				Om *= Ok;
			}
		}
		slicecpy(f, s, w, sx, sy);
		return 0;
	} else {
		/* Если размер слайса не четный -- используем обычное ПФ */
		return __ft2d(f, s, w, sx, sy);
	}
}




int fft2d(complex double* f, size_t width, size_t height, int variant) {
	complex double* s = calloc(width*height, sizeof(complex double));
	complex double norm = sqrt(width*height);
	slice sx, sy;
	fprintf(stderr, "Norm: %lf\n", creal(norm));
	switch(variant) {
	case 1:
		/** БПФ */
		sx = (slice){0, 1, width};
		sy = (slice){0, 1, height};
		_fft2dx(f, s, width, sx, sy);
		break;
	case 2:
		/** ПФ на слайсах */
		sx = (slice){0, 1, width};
		sy = (slice){0, 1, height};
		__ft2d(f, s, width, sx, sy);
		break;
	
	default:
		/** Классическая реализация ПФ */
		_ft2d(f, s, width, height);
		break;
	}
	/** Нормировка -- нужна для изображений */
	for(int i =0; i < width*height; i++ ) 
		f[i] /= norm;

	free(s);
	return 0;	
}


int pof2(int n) {
	int i = 0;
	while ( 1<<i <= n) i++;
	return 1<<(i-1);
}


