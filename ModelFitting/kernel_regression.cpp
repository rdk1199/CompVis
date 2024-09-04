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
	in_data(domain_vals),
	out_data(range_vals)
{

	if (in_data.size() != out_data.size())
	{
		cout << "invalid data given to ExactKernelRegression constructor - domain and range value vectors must be same size" << endl;
		exit(1);
	}


	Matrix<float> phi(n_pts, n_pts);

	for (int i = 0; i < n_pts; i++)
	{
		for (int j = 0; j < n_pts; j++)
		{
			phi[i][j] = (*basis)(abs(in_data[i] - in_data[j]), _falloff);
		}
	}

	weights = gauss_solve(phi, out_data);

}

template<class S, class T>
T ExactKernelRegression<S,T>::operator()(const S& x)
{
	T out = weights[0] * (*basis)(abs(x - in_data[0]), _falloff);

	for (int i = 1; i < n_pts; i++)
	{
		out = out + weights[i] * (*basis)(abs(x - in_data[i]), _falloff);
	}

	return out;
}


template<class S, class T>
RidgeKernelRegression<S, T>::RidgeKernelRegression(std::vector<S> domain_vals, std::vector<T> range_vals, float lambda, float falloff, float(*basis_func)(float, float)) :
	n_pts(domain_vals.size()),
	basis(basis_func),
	_lambda(lambda),
	_falloff(falloff),
	in_data(domain_vals),
	out_data(range_vals)
{
	if (in_data.size() != out_data.size())
	{
		cout << "invalid data given to ExactKernelRegression constructor - domain and range value vectors must be same size" << endl;
		exit(1);
	}

	Matrix<float> phi(n_pts, n_pts);

	for (int i = 0; i < n_pts; i++)
	{
		for (int j = 0; j < n_pts; j++)
		{
			phi[i][j] = (*basis)(abs(in_data[i] - in_data[j]), _falloff);
		}
	}

	weights = gauss_solve(phi.transpose() * phi + _lambda * Matrix<float>::identity(n_pts), phi.transpose() * out_data); //should do this with Cholesky factorization (later)

}

template<class S, class T>
T RidgeKernelRegression<S, T>::operator()(const S& x)
{
	T out = weights[0] * (*basis)(abs(x - in_data[0]), _falloff);

	for (int i = 1; i < n_pts; i++)
	{
		out = out + weights[i] * (*basis)(abs(x - in_data[i]), _falloff);
	}

	return out;
}

template<class S, class T>
NWKernelRegression<S, T>::NWKernelRegression(std::vector<S> domain_vals, std::vector<T> range_vals, float falloff, float(*basis_func)(float, float)) :
	n_pts(domain_vals.size()),
	basis(basis_func),
	_falloff(falloff),
	in_data(domain_vals),
	out_data(range_vals)
{

	if (in_data.size() != out_data.size())
	{
		cout << "invalid data given to NWKernelRegression constructor - domain and range value vectors must be same size" << endl;
		exit(1);
	}
}


template<class S, class T>
T NWKernelRegression<S, T>::operator()(const S& x)
{
	float weight_sum = (*basis)(abs(x - in_data[0]), _falloff);
	T out = (*basis)(abs(x - in_data[0]), _falloff)* out_data[0];

	for (int i = 1; i < n_pts; i++)
	{
		weight_sum += (*basis)(abs(x - in_data[i]), _falloff);
		out = out + (*basis)(abs(x - in_data[i]), _falloff) * out_data[i];
	}

	return (1.0f/weight_sum) * out;
}

template class ExactKernelRegression<float, float>;
template class ExactKernelRegression<vector<float>, float>;
template class ExactKernelRegression<vector<float>, Color>;

template class RidgeKernelRegression<vector<float>, Color>;

template class NWKernelRegression<vector<float>, Color>;