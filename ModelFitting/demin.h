#pragma once

#include <vector>
#include <utility>

enum class EnergyFunction {membrane, thin_plate};

template<class T>
class DEMinimizer
{
private:

	//width and height of grid
	int g_width;
	int g_height;

	double lambda;
	double constraint;

	int n_pts;
	std::vector<std::pair<int, int>> in_data; //data points on the grid
	std::vector<T> out_data;

	void construct_membrane_f_vec();
	void construct_thin_plate_f_vec();


	std::vector<T> f_vec; //function values to be computed (all done on a discrete grid -> laid out in 1D)

public:

	DEMinimizer(int width, int height, std::vector<std::pair<int, int>> domain, std::vector<T> range, double reg, double force_fit, EnergyFunction e_func);
	T operator()(int i , int j) const;
};