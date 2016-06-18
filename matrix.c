#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include "matrix.h"

typedef struct pair Pair;

struct pair
{
	size_t column;
	long double complex value;
};

struct matrix
{
	size_t row_count, *row;
	Pair *data;
};

int matrix_elem_insert(Matrix * restrict matrix, size_t row_idx, size_t col_idx, long double complex value);

int matrix_elem_delete(Matrix * restrict matrix, size_t row_idx, size_t col_idx);

Matrix *matrix_create(void)
{
	return (Matrix *) calloc(1u, sizeof(Matrix));
}

int matrix_destroy(Matrix ** restrict matrix)
{
	if (matrix == NULL)
	{
		return EOF;
	}

	if (*matrix == NULL)
	{
		return EOF;
	}

	if ((*matrix)->data != NULL)
	{
		free((*matrix)->data);
	}

	if ((*matrix)->row != NULL)
	{
		free((*matrix)->row);
	}

	free(*matrix);

	*matrix = NULL;

	return 0;
}

size_t matrix_get_row_count(const Matrix * restrict matrix)
{
	if (matrix == NULL)
	{
		return (size_t) -1;
	}

	return matrix->row_count;
}

size_t matrix_get_col_count(const Matrix * restrict matrix)
{
	if (matrix == NULL)
	{
		return (size_t) -1;
	}

	if (!matrix->row_count || matrix->row == NULL || matrix->data == NULL)
	{
		return 0u;
	}
	else
	{
		size_t result = 0u;

		for (size_t i = 0; i < matrix->row[matrix->row_count]; ++i)
		{
			if (result < matrix->data[i].column)
			{
				result = matrix->data[i].column;
			}
		}

		return ++result;
	}
}

int matrix_elem_set(Matrix * restrict matrix, size_t row_idx, size_t col_idx, long double complex value)
{
	if (matrix == NULL)
	{
		return EOF;
	}

	if (value)
	{
		return matrix_elem_insert(matrix, row_idx, col_idx, value);
	}
	else
	{
		return matrix_elem_delete(matrix, row_idx, col_idx);
	}
}

long double complex matrix_elem_get(const Matrix * restrict matrix, size_t row_idx, size_t col_idx)
{
	if (matrix == NULL)
	{
		return NAN + NAN * I;
	}

	if (!matrix->row_count || matrix->data == NULL || matrix->row == NULL)
	{
		return 0.0l;
	}

	for (size_t i = matrix->row[row_idx]; i < matrix->row[row_idx + 1u]; ++i)
	{
		if (matrix->data[i].column == col_idx)
		{
			return matrix->data[i].value;
		}
	}

	return 0.0l;
}

int matrix_fprintf(FILE * restrict stream, const char * restrict format, const Matrix * restrict matrix)
{
	if (stream == NULL || format == NULL || matrix == NULL)
	{
		return EOF;
	}

	size_t row = matrix_get_row_count(matrix), col = matrix_get_col_count(matrix);
	for (size_t i = 0u; i < row; ++i)
	{
		for (size_t j = 0u; j < col; ++j)
		{
			int local_error = fprintf(stream, format, creall(matrix_elem_get(matrix, i, j)), cimagl(matrix_elem_get(matrix, i, j)));
			if (local_error < 0)
			{
				return EOF;
			}
		}
		int local_error = fputc('\n', stream);
		if (local_error < 0)
		{
			return EOF;
		}
	}

	return 0;
}

int matrix_sum_max_row_fprint(FILE * restrict stream, const char * restrict format, const Matrix * restrict matrix)
{
	if (stream == NULL || format == NULL || matrix == NULL)
	{
		return EOF;
	}

	size_t max_nonzero_count = 0u;
	for (size_t i = 0u, row = matrix_get_row_count(matrix); i < row; ++i)
	{
		size_t nonzero_count = 0u;
		for (size_t j = 0u, col = matrix_get_col_count(matrix); j < col; ++j)
		{
			long double complex temp = matrix_elem_get(matrix, i, j);
			if (temp != temp)
			{
				return EOF;
			}
			else if (temp)
			{
				++nonzero_count;
			}
		}
		if (nonzero_count > max_nonzero_count)
		{
			max_nonzero_count = nonzero_count;
		}
	}

	for (size_t i = 0u, row = matrix_get_row_count(matrix); i < row; ++i)
	{
		size_t nonzero_count = 0u;
		for (size_t j = 0u, col = matrix_get_col_count(matrix); j < col; ++j)
		{
			long double complex temp = matrix_elem_get(matrix, i, j);
			if (temp != temp)
			{
				return EOF;
			}
			else if (temp)
			{
				++nonzero_count;
			}
		}
		if (nonzero_count == max_nonzero_count)
		{
			long double complex sum = 0.0l;
			for (size_t j = 0u, col = matrix_get_col_count(matrix); j < col; ++j)
			{
				long double complex temp = matrix_elem_get(matrix, i, j);
				if (temp != temp)
				{
					return EOF;
				}
				sum += temp;
			}

			int local_error = fprintf(stdout, format, i, sum);
			if (local_error < 0)
			{
				return EOF;
			}
		}
	}
	return 0;
}

int matrix_elem_insert(Matrix * restrict matrix, size_t row_idx, size_t col_idx, long double complex value)
{
	if (matrix == NULL)
	{
		return EOF;
	}

	if (!matrix->row_count || matrix->row == NULL || matrix->data == NULL)
	{
		matrix->row = calloc(row_idx + 2u, sizeof(size_t));
		if (matrix->row == NULL)
		{
			return EOF;
		}

		matrix->data = calloc(1u, sizeof(Pair));
		if (matrix->data == NULL)
		{
			free(matrix->row);
			matrix->row = NULL;

			return EOF;
		}

		matrix->row_count = row_idx + 1u;
		matrix->row[row_idx + 1u] = 1;
		matrix->data[0u] = (Pair) { col_idx, value };

		return 0;
	}

	if (matrix->row_count > row_idx)
	{
		for (size_t i = matrix->row[row_idx]; i < matrix->row[row_idx + 1u]; ++i)
		{
			if (matrix->data[i].column == col_idx)
			{
				matrix->data[i] = (Pair) { col_idx, value };
				return 0;
			}
		}
	}

	Pair *temp = realloc(matrix->data, (matrix->row[matrix->row_count] + 1u) * sizeof(Pair));
	if (temp == NULL)
	{
		return EOF;
	}
	matrix->data = temp;

	if (matrix->row_count <= row_idx)
	{
		size_t *temp = realloc(matrix->row, (row_idx + 2u) * sizeof(size_t));
		if (temp == NULL)
		{
			return EOF;
		}
		matrix->row = temp;

		for (size_t i = matrix->row_count + 1u; i < row_idx + 2u ; ++i)
		{
			matrix->row[i] = matrix->row[matrix->row_count];
		}

		matrix->row_count = row_idx + 1u;
		++matrix->row[matrix->row_count];
		matrix->data[matrix->row[row_idx]] = (Pair) { col_idx, value };

		return 0;
	}

	for (size_t i = row_idx + 1u; i <= matrix->row_count; ++i)
	{
		++matrix->row[i];
	}

	for (size_t i = matrix->row[row_idx]; i < matrix->row[row_idx + 1u]; ++i)
	{
		if (col_idx < matrix->data[i].column || i == matrix->row[row_idx + 1u] - 1u)
		{
			for (size_t j = matrix->row[matrix->row_count] - 2u; j >= i && j < (size_t) -1; --j)
			{
				matrix->data[j + 1] = matrix->data[j];
			}
			matrix->data[i] = (Pair) { col_idx, value };
			break;
		}
	}

	return 0;
}

int matrix_elem_delete(Matrix * restrict matrix, size_t row_idx, size_t col_idx)
{
	if (matrix == NULL)
	{
		return EOF;
	}

	if (!matrix->row || matrix->row == NULL || matrix->data == NULL)
	{
		return 0;
	}

	if (matrix->row_count <= row_idx)
	{
		return 0;
	}

	for (size_t i = matrix->row[row_idx]; i < matrix->row[row_idx + 1]; ++i)
	{
		if (matrix->data[i].column == col_idx)
		{
			if (matrix->row[matrix->row_count] <= 1u)
			{
				matrix->row_count = 0u;

				if (matrix->row != NULL)
				{
					free(matrix->row);
				}
				matrix->row = NULL;

				if (matrix->data != NULL)
				{
					free(matrix->data);
				}
				matrix->data = NULL;

				return 0;
			}

			for (size_t j = i; j < matrix->row[matrix->row_count] - 1u; ++j)
			{
				matrix->data[j] = matrix->data[j + 1u];
			}

			for (size_t j = row_idx + 1u; j <= matrix->row_count; ++j)
			{
				--matrix->row[j];
			}

			Pair *temp = realloc(matrix->data, matrix->row[matrix->row_count] * sizeof(Pair));
			if (temp == NULL)
			{
				return EOF;
			}
			matrix->data = temp;

			if (matrix->row_count == row_idx + 1u)
			{
				size_t j = matrix->row_count;
				while (matrix->row[j] >= matrix->row[matrix->row_count])
				{
					--j;
				}

				matrix->row_count = j + 1u;

				size_t *temp = realloc(matrix->row, (j + 2u) * sizeof(Pair));
				if (temp == NULL)
				{
					return EOF;
				}
				matrix->row = temp;
			}
			break;
		}
	}

	return 0;
}

void debug_fprintf(const Matrix * restrict matrix)
{
	printf("matrix = %p\n", (void *) matrix);
	if (matrix == NULL)
	{
		putchar('\n');
		return;
	}

	printf("matrix->row_count = %zu\n", matrix->row_count);
	printf("matrix->row = %p, matrix->data = %p\n", (void *) matrix->row, (void *) matrix->data);

	if (!matrix->row_count || matrix->row == NULL || matrix->data == NULL)
	{
		putchar('\n');
		return;
	}

	printf("matrix->row = {");
	for (size_t i = 0u; i <= matrix->row_count; ++i)
	{
		printf(" %zu,", matrix->row[i]);
	}
	printf(" }\n");

	printf("matrix->col = {");
	for (size_t i = 0u; i < matrix->row[matrix->row_count]; ++i)
	{
		printf(" %zu,", matrix->data[i].column);
	}
	printf(" }\n");

	printf("matrix->data = {");
	for (size_t i = 0u; i < matrix->row[matrix->row_count]; ++i)
	{
		printf(" %.2Lf + %.2Lfi,", creall(matrix->data[i].value), cimagl(matrix->data[i].value));
	}
	printf(" }\n");
	putchar('\n');
}
