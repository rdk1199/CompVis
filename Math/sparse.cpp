#include "sparse.h"

#include <iostream>

using std::cout;
using std::endl;

template<class T>
T SparseMatrix<T>::at(int i, int j) const
{
	if (!in_range(i, j))
	{
		cout << "out of bounds sparse matrix access " << endl;
		exit(1);
	}

	if (vals.count(i) && vals.at(i).count(j))
	{
		return vals.at(i).at(j);
	}

	return 0;
}

template<class T>
void SparseMatrix<T>::insert(int i, int j, T val)
{
	vals[i][j] = val;
}

template class SparseMatrix<float>;
template class SparseMatrix<double>;