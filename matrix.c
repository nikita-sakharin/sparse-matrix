#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include "matrix.h"

struct matrix
{
	size_t row_count, *row, *col;
	long double complex *data;
};

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

	if ((*matrix)->row != NULL)
	{
		free((*matrix)->row);
	}

	if ((*matrix)->col != NULL)
	{
		free((*matrix)->col);
	}

	if ((*matrix)->data != NULL)
	{
		free((*matrix)->data);
	}

	free(*matrix);

	*matrix = NULL;

	return 0;
}

int matrix_elem_insert(Matrix * restrict matrix, size_t row_idx, size_t col_idx, long double complex value)
{

}

int matrix_elem_delete(Matrix * restrict matrix, size_t row_idx, size_t col_idx)
{
	if (matrix == NULL || value)
	{
		return EOF;
	}

	if (matrix->row && matrix->row != NULL && matrix->col != NULL && matrix->data != NULL)
	{
		if (matrix->row_count > row_idx)
		{	
			for (size_t i = matrix->row[row_idx]; i < matrix->row[row_idx + 1]; ++i)
			{
				if (matrix->col[i] == col_idx)
				{
					if (matrix->row[matrix->row_count] <= 1u)
					{
						matrix->row_count = 0u;

						if (matrix->row != NULL)
						{
							free(matrix->row);
						}
						matrix->row = NULL;

						if (matrix->col != NULL)
						{
							free(matrix->col);
						}
						matrix->col = NULL;

						if (matrix->data != NULL)
						{
							free(matrix->data);
						}
						matrix->data = NULL;
					}
					else
					{/*
						for (size_t j = i; j < matrix->row[matrix->row_count] - 1u; ++j)
						{
							matrix->data[j] = matrix->data[j + 1];
							matrix->col[j] = matrix->col[j + 1];
						}

						for (size_t j = i; j < matrix->row[matrix->row_count] - 1u; ++j)
						{
							matrix->data[j] = matrix->data[j + 1];
						}*/
					}
				}
			}
		}
	}
}

int matrix_elem_set(Matrix * restrict matrix, size_t row_idx, size_t col_idx, long double complex value)
{
	if (matrix == NULL)
	{
		return EOF;
	}

	else
	{
		if (!matrix->row_count || matrix->row == NULL || matrix->col == NULL || matrix->data == NULL)
		{
			matrix->row = calloc(row_idx + 2u, sizeof(size_t));
			if (matrix->row == NULL)
			{
				return EOF;
			}

			matrix->col = calloc(1u, sizeof(size_t));
			if (matrix->col == NULL)
			{
				free(matrix->row);
				matrix->row = NULL;
				return EOF;
			}

			matrix->data = calloc(1u, sizeof(long double complex));
			if (matrix->data == NULL)
			{
				free(matrix->row);
				matrix->row = NULL;

				free(matrix->col);
				matrix->col = NULL;

				return EOF;
			}

			matrix->row_count = row_idx + 1u;
			matrix->row[row_idx + 1u] = 1;
			matrix->col[0u] = col_idx;
			matrix->data[0u] = value;
		}
		else
		{
			if (matrix->row_count > row_idx)
			{
				for (size_t i = matrix->row[row_idx]; i < matrix->row[row_idx + 1u]; ++i)
				{
					if (matrix->col[i] == col_idx)
					{
						matrix->data[i] = value;

						return 0;
					}
				}
			}

			{
				long double complex *temp = realloc(matrix->data, (matrix->row[matrix->row_count] + 1u) * sizeof(long double complex));
				if (temp == NULL)
				{
					return EOF;
				}
				matrix->data = temp;
			}

			{
				size_t *temp = realloc(matrix->col, (matrix->row[matrix->row_count] + 1u) * sizeof(size_t));
				if (temp == NULL)
				{
					return EOF;
				}
				matrix->col = temp;
			}

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
			}

			for (size_t i = matrix->row[matrix->row_count] - 1u; i >= matrix->row[row_idx]; --i)
			{
				matrix->col[i + 1u] = matrix[i];
			}
		}
	}

	return 0;
}

long double complex matrix_elem_get(const Matrix * restrict matrix, size_t row_idx, size_t col_idx)
{
	if ()
	{
		/* code */
	}

	for (size_t i = matrix->row[row_idx]; i < matrix->row[row_idx + 1u]; ++i)
	{
		if (matrix->col[i] == col_idx)
		{
			return matrix->data[i];
		}
	}

	return 0.0l;
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

	if (matrix->row == NULL)
	{
		return 0u;
	}
	else
	{
		size_t result = 0u;

		for (size_t i = 0; i < matrix->row[matrix->row_count]; ++i)
		{
			if (result < matrix->col[i])
			{
				result = matrix->col[i];
			}
		}

		return ++result;
	}
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
			int local_error = fprintf(stream, format, matrix_elem_get(matrix, i, j));

			if (local_error < 0)
			{
				return EOF;
			}
		}
	}

	return 0;
}

void debug_fprintf(const Matrix * restrict matrix)
{
	printf("matrix = %p\n", (void *) matrix);
	if (matrix == NULL)
	{
		return;
	}

	printf("matrix->row_count = %zu\n", matrix->row_count);
	printf("matrix->row = %p, matrix->col = %p, matrix->data = %p\n", (void *) matrix->row, (void *) matrix->col, (void *) matrix->data);

	if (matrix->row == NULL || matrix->col == NULL || matrix->data == NULL)
	{
		return;
	}

	printf("matrix->row = {");
	for (size_t i = 0u; i <= matrix->row_count; ++i)
	{
		printf(" %zu,", matrix->row[i]);
	}
	printf(" }\n");

	printf("matrix->col  = {");
	for (size_t i = 0u; i < matrix->row[matrix->row_count]; ++i)
	{
		printf(" %010zu,", matrix->col[i]);
	}
	printf(" }\n");

	printf("matrix->data = {");
	for (size_t i = 0u; i < matrix->row[matrix->row_count]; ++i)
	{
		printf("%.2Lf + %.2Lfi,", creall(matrix->data[i]), cimagl(matrix->data[i]));
	}
	printf(" }\n");
}
