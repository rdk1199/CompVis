#pragma once

#include <cmath>
#include <iostream>


#define PI 3.1415926535897
#define DEG2RAD (PI/180.0)
#define RAD2DEG (180.0/PI)


struct complex
{
	float re = 0.0f;
	float im = 0.0f;

	complex() :
		re(0.0f),
		im(0.0f)
	{}

	complex(float real, float imag) :
		re(real),
		im(imag)
	{}

	complex(float real) :
		re(real),
		im(0.0f)
	{}

	complex conjugate() const { return { re, -im }; }
	float sq_modulus() const { return re * re + im * im; }
	float modulus() const { return sqrt(re * re + im * im); }
	float real() const { return re; }
	float imag() const { return im; }

	float operator=(float b) { re = b; im = 0.0f; return b; }

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
