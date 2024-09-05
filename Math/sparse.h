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

	std::map<int, std::map<int, T>> vals; //keep rows separate -> this makes iteration a bit more complicated but it's conceptually easier? maybe


public:

	int n_rows() const { return rows; }
	int n_cols() const { return cols; }

	bool in_range(int i, int j) const { return 0 <= i && i < rows && 0 <= j && j < cols; }

	T at(int i, int j) const; //returns vals[{i,j}] if it exists, otherwise just returns 0 (does not create anything new in vals)
	void insert(int i, int j, T val); //inserts val at position i,j 
	std::map<int, T>& operator[](int i){ return vals[i]; }
	const std::map<int, std::map<int, T>>& values() const { return vals; }

	SparseMatrix(int r, int c) : rows(r), cols(c) {}; //r rows, c cols //zero matrix
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

	for (auto i = A.values().begin(); i != A.values().end(); i++)
	{
		for (auto j = i->second.begin(); j != i->second.end(); j++)
		{
			sum[i->first][j->first] += j->second;
		}
	}

	for (auto i = B.values().begin(); i != B.values().end(); i++)
	{
		for (auto j = i->second.begin(); j != i->second.end(); j++)
		{
			sum[i->first][j->first] += j->second;
		}
	}

	return sum;
}


template<class T> SparseMatrix<T> operator-(const SparseMatrix<T>& A, const SparseMatrix<T>& B)
{
	SparseMatrix<T> diff;

	for (auto i = A.values().begin(); i != A.values().end(); i++)
	{
		for (auto j = i->second.begin(); j != i->second.end(); j++)
		{
			diff[i->first][j->first] += j->second;
		}
	}

	for (auto i = B.values().begin(); i != B.values().end(); i++)
	{
		for (auto j = i->second.begin(); j != i->second.end(); j++)
		{
			diff[i->first][j->first] -= j->second;
		}
	}

	return diff;
}

template<class T> SparseMatrix<T> operator*(const T& c, const SparseMatrix<T>& A)
{
	SparseMatrix<T> prod;

	for (auto i = A.values().begin(); i != A.values().end(); i++)
	{
		for (auto j = i->second.begin(); j != i->second.end(); j++)
		{
			prod[i->first][j->first] = c * j->second;
		}
	}

	return prod;
}


template<class T> SparseMatrix<T> operator*(const SparseMatrix<T>& A, const SparseMatrix<T>& B);

template<class S, class T> std::vector<T> operator*(const SparseMatrix<S>& A, const std::vector<T>& x);

