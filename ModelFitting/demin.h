#pragma once

#include <vector>
#include <utility>

template<class T>
class DEMinimizer
{
private:

	//width and height of grid
	int g_width;
	int g_height;

	float lambda;

	int n_pts;
	std::vector<std::pair<int, int>> in_data; //data points on the grid
	std::vector<T> out_data;

	std::vector<T> f_vec; //function values to be computed (all done on a discrete grid -> laid out in 1D)

public:

	DEMinimizer(int width, int height, std::vector<std::pair<int, int>> domain, std::vector<T> range, float reg);
	T operator()(int i , int j) const;
};