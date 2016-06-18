#ifndef _MATRIX_H_
#define _MATRIX_H_

typedef struct matrix Matrix;

Matrix *matrix_create(void);

int matrix_destroy(Matrix ** restrict);

size_t matrix_get_row_count(const Matrix * restrict);

size_t matrix_get_col_count(const Matrix * restrict matrix);

int matrix_elem_set(Matrix * restrict, size_t, size_t, long double complex);

long double complex matrix_elem_get(const Matrix * restrict, size_t, size_t);

int matrix_fprintf(FILE * restrict, const char * restrict, const Matrix * restrict);

int matrix_sum_max_row_fprint(FILE * restrict, const char * restrict, const Matrix * restrict);

#endif
