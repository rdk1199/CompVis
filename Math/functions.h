#pragma once

#include <cmath>

inline float bicubic_spline(float x, float a = -1)
{
	float out;
	float abs_x = std::abs(x);

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

inline float bicubic_spline_2d(float x, float y, float a = -1)
{
	return bicubic_spline(x, a) * bicubic_spline(y, a);
}

template<class T>
inline T gaussian_basis(T r, float c)
{
	return std::exp(-(r * r) / (c * c));
}

template<class T>
inline T hardy_mq(T r, float c)
{
	return std::sqrt(r * r + c * c);
}

template<class T>
inline T inv_mq(T r, float c)
{
	return 1 / hardy_mq(r, c);
}

template<class T>
inline T tp_spline(T r, float c)
{
	return r * r * std::log(r);
}



