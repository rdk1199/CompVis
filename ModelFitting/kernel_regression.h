#pragma once

#include <vector>

//S is domain data type, T is range data type - might make these all children of some parent class later
template<class S, class T>
class ExactKernelRegression
{
private:

	int n_pts; //number of data points being interpolated
	double (*basis)(double, double);
	double _falloff; //basis function falloff parameter "c"

	//input data points : vectors will have same length -  x in domain_pts[i], f(x) in values[i] 
	std::vector<S> in_data;
	std::vector<T> out_data;

	std::vector<T> weights; //weights of domain functions - to be calculated

public:

	ExactKernelRegression(std::vector<S> domain_vals, std::vector<T> range_vals, double falloff, double(*basis_func)(double, double)); //constructor computes weights based off given data
	T operator()(const S& x) const; //evaluates regression function at given input x

};

template<class S, class T>
class RidgeKernelRegression 
{
private:

	int n_pts; 
	double (*basis)(double, double);
	double _falloff;
	double _lambda;

	std::vector<S> in_data;
	std::vector<T> out_data;

	std::vector<T> weights;

public:
	
	RidgeKernelRegression(std::vector<S> domain_vals, std::vector<T> range_vals, double lambda, double falloff, double(*basis_func)(double, double));
	T operator()(const S& x) const;

};

template<class S, class T>
class NWKernelRegression //Nadaraya-Watson
{
	private:
		int n_pts;
		double (*basis)(double, double);
		double _falloff;

		std::vector<S> in_data;
		std::vector<T> out_data;

		

	public:
		NWKernelRegression(std::vector<S> domain_vals, std::vector<T> range_vals, double falloff, double(*basis_func)(double, double)); //constructor computes weights based off given data
		T operator()(const S& x) const; //evaluates regression function at given input x

};