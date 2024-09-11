#pragma once

#include <cmath>

#include "numbers.h"


inline double bicubic_spline(double x, double a = -1)
{
	double out;
	double abs_x = std::abs(x);

	if (abs_x < 1.0f)
	{
		out = 1.0f - (a + 3) * x * x + (a + 2) * abs_x * abs_x * abs_x;
	}

	else if (abs_x < 2.0f)
	{
		out = a * (abs_x - 1) * (abs_x - 2)*(abs_x - 2);
	}

	else
	{
		out = 0.0f;
	}

	return out;
}

inline double bicubic_spline_2d(double x, double y, double a = -1)
{
	return bicubic_spline(x, a) * bicubic_spline(y, a);
}

template<class T>
inline T gaussian_basis(T r, double c)
{
	return std::exp(-(r * r) / (c * c));
}

template<class T>
inline T hardy_mq(T r, double c)
{
	return std::sqrt(r * r + c * c);
}

template<class T>
inline T inv_mq(T r, double c)
{
	return 1 / hardy_mq(r, c);
}

template<class T>
inline T tp_spline(T r, double c)
{
	return r * r * std::log(r);
}

template<class T>
inline double gaussian(const std::vector<T>& x, T cov_det, const std::vector<T>& mean, const Matrix<T>& cov_inv)
{
	double dim = x.size();

	return (1.0 / std::pow(2 * PI, dim / 2.0)) * (1.0 / std::sqrt(abs(cov_det))) * exp(-0.5 * (x - mean) * ((cov_inv) * (x - mean)));

}

