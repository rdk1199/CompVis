#pragma once

#include <vector>

//S is domain data type, T is range data type
template<class S, class T>
class ExactKernelRegression
{
private:

	int n_pts; //number of data points being interpolated
	float (*basis)(float, float);
	float _falloff; //basis function falloff parameter "c"

	//input data points : vectors will have same length -  x in domain_pts[i], f(x) in values[i] 
	std::vector<S> in;
	std::vector<T> out;

	std::vector<T> weights; //weights of domain functions - to be calculated

public:

	ExactKernelRegression(std::vector<S> domain_vals, std::vector<T> range_vals, float falloff, float(*basis_func)(float, float)); //constructor computes weights based off given data
	T operator()(const S& x); //evaluates regression function at given input x

};