#include <algorithm>

#include "stats.h"

using std::vector;

template<class T>
T median(vector<T>& vals)
{
	if (vals.empty())
	{
		return 0;
	}

	auto med = vals.begin() + vals.size() / 2;
	std::nth_element(vals.begin(), med, vals.end());


	return vals[vals.size() / 2];
	
}

template<class T>
T mean(const vector<T>& vals)
{
	T sum;

	for (int i = 0; i < vals.size(); i++)
	{
		sum = sum + vals[i];
	}

	return sum / vals.size();

}

template<class T>
T threshold(T val, T thresh)
{
	return val >= thresh ? 1 : 0;
}



template float mean(const vector<float>& vals);

template float median(vector<float>& vals);
template int median(vector<int>& vals);
template double median(vector<double>& vals);

template float threshold(float val, float thresh);
template int threshold(int val, int thresh);
template double threshold(double val, double thresh);


