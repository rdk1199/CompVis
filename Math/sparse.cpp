#include "sparse.h"

#include <iostream>

using std::cout;
using std::endl;

template<class T>
T SparseMatrix<T>::at(int i, int j) const
{
	if (vals[i].count(j))
	{
		return vals[i].at(j);
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