#include "sparse.h"

#include <iostream>

using std::vector;
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


template<class T>
double SparseMatrix<T>::sassenfeld() const
{
	vector<double> p(1, 0.0);

	//compute p0

	for (auto j = vals[0].begin(); j != vals[0].end(); j++)
	{
		if (j->first != 0)
		{
			p[0] += abs(j->second);
		}
	}

	p[0] /= abs(at(0, 0));

	double p_max = p[0];

	//now compute the rest
	for (int i = 1; i < n_rows(); i++)
	{
		double p_i = 0;

		for (auto j = vals[i].begin(); j != vals[i].end(); j++) //there's a faster way to do this probably (than comparing each time, since the iterator goes in order anyways)
		{
			if (j->first < i)
			{
				p_i += abs(j->second) * p[j->first];
			}

			if (j->first > i)
			{
				p_i += abs(j->second);
			}
		}

		p_i /= abs(at(i, i));
		p_max = std::max(p_max, p_i);
		p.push_back(p_i);
 	}

	return p_max;
}

template class SparseMatrix<float>;
template class SparseMatrix<double>;