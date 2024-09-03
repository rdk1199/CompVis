#pragma once

#include <iostream>

#include "linalg.h"


template<class T> //solve Ax = b using Gaussian elimination //FOR NOW this assumes a unique solution (i.e. invertible matrix)
inline std::vector<T> gauss_solve(const Matrix<T>& A, const std::vector<T>& b)
{
	if (A.n_rows() != b.size())
	{
		std::cout << "Cannot gauss_solve matrix A and vector b with different number of rows: A has " << A.n_rows() << " rows, b has " << b.size() << " columns " << std::endl;
		exit(1);
	}

	Matrix<T> reduced = A.attach_col(b).red_row_ech();


	std::vector<T> out;

	for (int i = 0; i < b.size(); i++)
	{
		out.push_back(reduced[i].back());
	}

	return out;
}


template<class S, class T>
inline std::vector<T> gauss_solve(const Matrix<S>& A, std::vector<T> b) //solve Ax = b using Gaussian elimination, where elements of b have different type from elements of A //just copy b so we can modify the copy
{
	if (A.n_rows() != b.size())
	{
		std::cout << "Cannot gauss_solve matrix A and vector b with different number of rows: A has " << A.n_rows() << " rows, b has " << b.size() << " columns " << std::endl;
		exit(1);
	}

	Matrix<S> out(A);

	int r = 0;
	int c = 0;


	while (r < A.n_rows() && c < A.n_cols())
	{
		int i_max = r;

		for (int i = r + 1; i < A.n_rows(); i++)
		{
			if (abs(out[i][c]) > abs(out[i_max][c])) //find the largest possible pivot in the column
			{
				i_max = i;
			}
		}

		if (out[i_max][c] == 0) //no viable pivot in this column
		{
			c++;
			continue;
		}

		out.swap_rows(r, i_max);
		T tmp = b[r];
		b[r] = b[i_max];
		b[i_max] = tmp;

		out.multiply_row(r, 1.0f / out[r][c]);
		out[r][c] = 1.0f; //make sure this is exactly 1
		b[r] = b[r] * 1.0f / out[r][c];

		for (int i = 0; i < r; i++)
		{
			S scale = -out[i][c];
			out.add_multiple_of_row(i, r, scale);
			out[i][c] = 0; //just make sure these are exactly zero
			b[i] = b[i] + scale * b[r];
		}

		for (int i = r + 1; i < A.n_rows(); i++) //just split up the for loop so we don't have to check i != r every time
		{
			S scale = -out[i][c];
			out.add_multiple_of_row(i, r, scale);
			out[i][c] = 0; //just make sure these are exactly zero
			b[i] = b[i] + scale * b[r];
		}

		r++;
		c++;
	}

	return b;

}