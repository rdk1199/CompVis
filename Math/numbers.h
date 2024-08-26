#pragma once

#include <cmath>
#include <iostream>

struct complex
{
	float re = 0;
	float im = 0;

	complex conjugate() const { return { re, -im }; }
	float sq_modulus() const { return re * re + im * im; }
	float modulus() const { return sqrt( re * re + im * im ); }
	float real() const { return re; }
	float imag() const { return im; }

	float operator=(float b) { re = b; im = 0; return b; }
};

inline complex operator+(const complex& a, const complex& b)
{
	return { a.re + b.re, a.im + b.im };
}

inline complex operator+=(complex& a, const complex& b)
{
	a = a + b;
	return a;
}

inline complex operator-(const complex& a, const complex& b)
{
	return { a.re - b.re, a.im - b.im };
}

inline complex operator-=(complex& a, const complex& b)
{
	a = a - b;
	return a;
}

inline complex operator*(const complex& a, const complex& b)
{
	return { a.re * b.re - a.im * b.im, a.re * b.im + b.re * a.im };
}

inline complex operator*=(complex& a, const complex& b)
{
	a = a * b;
	return a;
}

inline complex operator*(const float& a, const complex& b)
{
	return { a * b.re, a * b.im };
}

inline complex operator*(const complex& a, const float& b)
{
	return b * a;
}

inline complex operator/(const complex& a, const complex& b)
{
	return (1.0f / b.sq_modulus())* complex{a.re * b.re + a.im * b.im, a.im * b.re - a.re * b.im};
}

inline complex operator/=(complex& a, const complex& b)
{
	a = a / b;
	return a;
}

inline complex operator/(const complex& a, const float& b)
{
	return (1.0f / b) * a;
}

inline bool operator==(const complex& a, const complex& b)
{
	return a.re == b.re && a.im == b.im;
}

inline bool operator!=(const complex& a, const complex& b)
{
	return !(a == b);
}

/*
inline complex operator=(complex& a, const float& b)
{
	a.re = b;
	a.im = b;

	return a;
}*/

inline std::ostream& operator<<(std::ostream& stream, const complex& v)
{
	stream << v.re << " + " << v.im << "i";
	return stream;
}
