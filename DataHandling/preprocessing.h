#pragma once

#include "../Math/stats.h"


template<class T>
void center(std::vector<T>& data) //recenter all data to have a mean of 0
{
	T mean = stats::mean(data);

	for (int i = 0; i < data.size(); i++)
	{
		data[i] -= mean;
	}
}

template<class T>
void normalize(std::vector<T>& data) //scale data to have variance 1
{
	T mean = stats::mean(data);
	T var = p_variance(data, mean);

	for (int i = 0; i < data.size(); i++)
	{
		data[i] = data[i] / sqrt(var);
	}
}




