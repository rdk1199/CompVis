#pragma once

#include <iostream>

#include "linalg.h"
#include "sparse.h"

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
inline std::vector<T> gauss_solve(const Matrix<S>& A, std::vector<T> b) //solve Ax = b using Gaussian elimination, where elements of b have possibly different type from elements of A //just copy b so we can modify the copy
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

		if (abs(out[i_max][c]) < 1e-8) //zero column
		{
			c++;
			continue;
		}

		out.swap_rows(r, i_max);
		T tmp = b[r];
		b[r] = b[i_max];
		b[i_max] = tmp;

		out.multiply_row(r, 1.0 / out[r][c]);
		out[r][c] = 1.0; //make sure this is exactly 1
		b[r] = b[r] * 1.0 / out[r][c];

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


template<class S, class T>
inline std::vector<T> gauss_seidel_solve(const SparseMatrix<S>& A, std::vector<T> b, double err = 1e-4, int max_iter = 2000) //apply Gauss-Seidel iteration to solve Ax = b -> assumes A satisfies convergence conditions, so be careful
{
	std::vector<T> soln(A.n_rows());

	for (int iter = 0; iter < max_iter; iter++)
	{
		for (int i = 0; i < soln.size(); i++)
		{
			soln[i] = b[i];

			std::map<int, S> ith_row = A.at(i);

			for (auto j = ith_row.begin(); j != ith_row.end(); j++)
			{
				if (j->first != i) //this check slows things down
				{
					soln[i] -= j->second * soln[j->first];
				}
			}

			soln[i] = soln[i] / A.at(i, i);
		}
	}	

	return soln;
}