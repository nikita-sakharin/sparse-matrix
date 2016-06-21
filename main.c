#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "matrix.h"

long double complex read_complex(void);

int main(void)
{

	Matrix *matrix = matrix_create();
	if (matrix == NULL)
	{
		printf("Error\n");
	}

	int code = 0;
	while (code != EOF)
	{
		puts("Press S   for set element  to  matrix.");
		puts("Press G   for get element from matrix.");
		puts("Press M   for print max nonzero row in matrix.");
		puts("Press EOF for finishing.");

		code = getchar();
		if (code == '\n')
		{
			continue;
		}
		getchar();

		switch (code)
		{

		case 'S':
		{
			size_t i = 0u, j = 0u;
			scanf("%zu%zu", &i, &j);

			long double complex num = read_complex();
			if (num != num)
			{
				puts("Error reading was detected.\n");
				continue;
			}

			int result = matrix_elem_set(matrix, i, j, num);
			if (result == EOF)
			{
				puts("Error seting was detected.\n");
				continue;
			}

			puts("Success.\n");
			break;
		}

		case 'G':
		{
			size_t i = 0u, j = 0u;
			scanf("%zu%zu", &i, &j);
			getchar();

			long double complex num = matrix_elem_get(matrix, i, j);
			if (num != num)
			{
				puts("Error geting was detected.\n");
				continue;
			}

			printf("%Lf%+Lfi\n", creall(num), cimagl(num));

			puts("Success.\n");
			break;
		}

		case 'M':
		{
			matrix_sum_max_row_fprint(stdout, "row = %zu, sum = %5.2Lf%+5.2Lfi\n", matrix);
			break;
		}

		case EOF:
		{
			puts("Finishing.\n");
			break;
		}

		default:
		{
			puts("Wrong code.\n");
			break;
		}

		}
	}

	if (matrix_destroy(&matrix) == EOF)
	{
		puts("Error destroing.\n");
	}

	return 0;
}

#define N 65535u

long double complex read_complex(void)
{
	clearerr(stdin);

	char temp[N] = { '\0' };

	char *result = fgets(temp, N, stdin);
	if (result == NULL || ferror(stdin))
	{
		return NAN + NAN * I;
	}

	if (temp[strlen(temp) - 1u] == '\n')
	{
		temp[strlen(temp) - 1u] = '\0';
	}

	size_t j = 0u;
	for (size_t i = 0u; temp[i]; ++i)
	{
		if (isdigit(temp[i]) || temp[i] == '.' || temp[i] == '+' || temp[i] == '-' || temp[i] == 'i')
		{
			temp[j++] = temp[i];
		}
		else if (!isspace(temp[i]))
		{
			return NAN + NAN * I;
		}
	}
	temp[j] = '\0';

	bool is_imag = false, is_two = false;
	for (size_t i = 1u; temp[i]; ++i)
	{
		if (temp[i] == 'i')
		{
			is_imag = true;
		}
		else if (temp[i] == '+' || temp[i] == '-')
		{
			is_two = true;
		}
	}

	if (is_imag && is_two)
	{
		long double real = 0.0l, imag = 0.0l;
		sscanf(temp, "%Lf%Lfi", &real, &imag);
		return real + imag * I;
	}
	else if (is_imag && !is_two)
	{
		long double imag = 0.0l;
		sscanf(temp, "%Lfi", &imag);
		return imag * I;
	}
	else
	{
		long double real = 0.0l;
		sscanf(temp, "%Lf", &real);
		return real;
	}
}
