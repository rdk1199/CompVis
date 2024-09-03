#include <iostream>

#include "kernel_regression.h"
#include "../Math/solvers.h"
#include "../ImagProc/image.h"

using std::cout;
using std::endl;
using std::vector;

template<class S, class T>
ExactKernelRegression<S,T>::ExactKernelRegression(std::vector<S> domain_vals, std::vector<T> range_vals, float falloff, float(*basis_func)(float, float)) :
	n_pts(domain_vals.size()),
	basis(basis_func),
	_falloff(falloff),
	in(domain_vals),
	out(range_vals)
{

	if (in.size() != out.size())
	{
		cout << "invalid data given to ExactKernelRegression constructor - domain and range value vectors must be same size" << endl;
		exit(1);
	}


	Matrix<float> phi(n_pts, n_pts);

	for (int i = 0; i < n_pts; i++)
	{
		for (int j = 0; j < n_pts; j++)
		{
			phi[i][j] = (*basis)(abs(in[i] - in[j]), _falloff);
		}
	}

	weights = gauss_solve(phi, out);

}

template<class S, class T>
T ExactKernelRegression<S,T>::operator()(const S& x)
{
	T out = weights[0] * (*basis)(abs(x - in[0]), _falloff);

	for (int i = 1; i < n_pts; i++)
	{
		out = out + weights[i] * (*basis)(abs(x - in[i]), _falloff);
	}

	return out;
}

template class ExactKernelRegression<float, float>;
template class ExactKernelRegression<vector<float>, float>;
template class ExactKernelRegression<vector<float>, Color>;