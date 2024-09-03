#pragma once

#include <iostream>

#include "linalg.h"


template<class T> //solve Ax = b using Gaussian elimination //FOR NOW this assumes a unique solution (i.e. invertible matrix)
inline std::vector<T> gauss_solve(Matrix<T> A, std::vector<T> b)
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