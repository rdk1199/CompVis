#pragma once

#include <cmath>
#include <iostream>


#define PI 3.1415926535897
#define DEG2RAD (PI/180.0)
#define RAD2DEG (180.0/PI)


struct complex
{
	double re = 0.0f;
	double im = 0.0f;

	complex() :
		re(0.0f),
		im(0.0f)
	{}

	complex(double real, double imag) :
		re(real),
		im(imag)
	{}

	complex(double real) :
		re(real),
		im(0.0f)
	{}

	complex conjugate() const { return { re, -im }; }
	double sq_modulus() const { return re * re + im * im; }
	double modulus() const { return sqrt(re * re + im * im); }
	double real() const { return re; }
	double imag() const { return im; }

	double operator=(double b) { re = b; im = 0.0f; return b; }

};

inline complex abs(complex c) { return complex(c.modulus(), 0); }

inline bool operator>(const complex& a, const complex& b) //compare real parts (useful for storing magnitude as a complex number)
{
	return a.re > b.re;
}

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

inline complex operator-(const complex& a)
{
	return { -a.re, -a.im };
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

inline complex operator*(const double& a, const complex& b)
{
	return { a * b.re, a * b.im };
}

inline complex operator*(const complex& a, const double& b)
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

inline complex operator/(const complex& a, const double& b)
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
inline complex operator=(complex& a, const double& b)
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
