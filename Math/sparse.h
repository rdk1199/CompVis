#pragma once

#include <map>
#include <utility>
#include <vector>
#include <iostream>

template<class T> //sparse matrix - only holds nonzero entries, all others assumed to be zero;
class SparseMatrix 
{
private:

	int rows;
	int cols;

	std::vector<std::map<int, T>> vals; //keep rows separate -> this makes iteration a bit more complicated but it's conceptually easier? maybe


public:

	int n_rows() const { return rows; }
	int n_cols() const { return cols; }

	bool in_range(int i, int j) const { return 0 <= i && i < rows && 0 <= j && j < cols; }

	//these don't check bounds - careful!
	T at(int i, int j) const; //returns vals[{i,j}] if it exists, otherwise just returns 0 (does not create anything new in vals)
	const std::map<int, T>& at(int i) const { return vals[i]; } //return the ith row map

	void insert(int i, int j, T val); //inserts val at position i,j 
	std::map<int, T>& operator[](int i){ return vals[i]; }
	const std::vector<std::map<int, T>>& values() const { return vals; }

	SparseMatrix(int r, int c) : rows(r), cols(c), vals(std::vector<std::map<int, T>>(r)) {}; //r rows, c cols //zero matrix
};


template<class T> 
inline SparseMatrix<T> operator+(const SparseMatrix<T>& A, const SparseMatrix<T>& B)
{
	if (A.n_rows() != B.n_rows() || A.n_cols() != B.n_cols())
	{
		std::cout << "can not add sparse matrices of different sizes" << std::endl;
		exit(1);
	}

	SparseMatrix<T> sum(A.n_rows(), A.n_cols());

	for (int i = 0; i < A.n_rows(); i++)
	{
		for (auto j = A.at(i).begin(); j != A.at(i).end(); j++)
		{
			sum[i][j->first] += j->second;
		}
	}

	for (int i = 0; i < B.n_rows(); i++)
	{
		for (auto j = B.at(i).begin(); j != B.at(i).end(); j++)
		{
			sum[i][j->first] += j->second;
		}
	}

	return sum;
}


template<class T>
inline SparseMatrix<T> operator-(const SparseMatrix<T>& A, const SparseMatrix<T>& B)
{
	if (A.n_rows() != B.n_rows() || A.n_cols() != B.n_cols())
	{
		std::cout << "can not subtract sparse matrices of different sizes" << std::endl;
		exit(1);
	}

	SparseMatrix<T> diff(A.n_rows(), A.n_cols());

	for (int i = 0; i < A.n_rows(); i++)
	{
		for (auto j = A.at(i).begin(); j != A.at(i).end(); j++)
		{
			diff[i][j->first] += j->second;
		}
	}

	for (int i = 0; i < B.n_rows(); i++)
	{
		for (auto j = B.at(i).begin(); j != B.at(i).end(); j++)
		{
			diff[i][j->first] -= j->second;
		}
	}

	return diff;
}


template<class T> SparseMatrix<T> operator*(const T& c, const SparseMatrix<T>& A)
{
	SparseMatrix<T> prod(A.n_rows(), A.n_cols());

	for (int i = 0; i < A.n_rows(); i++)
	{
		for (auto j = A.at(i).begin(); j != A.at(i).end(); j++)
		{
			prod[i][j->first] = c * j->second;
		}
	}

	return prod;
}

template<class S, class T> std::vector<T> operator*(const SparseMatrix<S>& A, const std::vector<T>& x)
{
	if (x.size() != A.n_cols())
	{
		std::cout << "invalid sparse matrix - vector multiplication" << std::endl;
		exit(1);
	}

	std::vector<T> prod(A.n_rows());


	for (int i = 0; i < prod.size(); i++)
	{
		for (auto j = A.at(i).begin(); j != A.at(i).end(); j++)
		{
			prod[i] = prod[i] + x[j->first] * j->second;
		}
	}

	return prod;
}





