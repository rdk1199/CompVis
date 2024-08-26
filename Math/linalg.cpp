
#include "linalg.h"

using std::vector;



template<class T>
std::vector<T> operator+(const std::vector<T>& a, const std::vector<T>& b)
{
	if (a.size() != b.size())
	{
		throw IllegalVectorOp();
	}

	vector<T> out;

	for (int i = 0; i < a.size(); i++)
	{
		out.push_back(a[i] + b[i]);
	}

	return out;
}


template<class T>
std::vector<T> operator-(const std::vector<T>& a, const std::vector<T>& b)
{
	if (a.size() != b.size())
	{
		throw IllegalVectorOp();
	}

	vector<T> out;

	for (int i = 0; i < a.size(); i++)
	{
		out.push_back(a[i] - b[i]);
	}

	return out;
}


template<class T>
T operator*(const std::vector<T>& a, const std::vector<T>& b) //dot product
{
	if (a.size() != b.size())
	{
		throw IllegalVectorOp();
	}

	T out;

	for (int i = 0; i < a.size(); i++)
	{
		out += a[i] * b[i];
	}

	return out;
}

template<class S, class T> //scalar multiplication
std::vector<T> operator*(const S& c, const std::vector<T>& v)
{
	vector<T> out;

	for (int i = 0; i < v.size(); i++)
	{
		out.push_back(c*v[i]);
	}

	return out;
}

template<class T>
inline std::ostream& operator<<(std::ostream& stream, const std::vector<T>& v)
{
	for (int i = 0; i < v.size(); i++)
	{
		stream << v[i] << " ";
	}

	return stream;
}


template std::vector<float> operator+<float>(const std::vector<float>& a, const std::vector<float>& b);
template std::vector<complex> operator+<complex>(const std::vector<complex>& a, const std::vector<complex>& b);

template std::vector<float> operator-(const std::vector<float>& a, const std::vector<float>& b);
template std::vector<complex> operator-(const std::vector<complex>& a, const std::vector<complex>& b);

template float operator*<float>(const std::vector<float>& a, const std::vector<float>& b);
template complex operator*<complex>(const std::vector<complex>& a, const std::vector<complex>& b);

//scalar multiplication
template std::vector<float> operator*(const float& c, const std::vector<float>& v); 
template std::vector<complex> operator*(const complex& c, const std::vector<complex>& v); 
template std::vector<complex> operator*(const float& c, const std::vector<complex>& v);

std::ostream& operator<<(std::ostream& stream, const std::vector<float>& v);
std::ostream& operator<<(std::ostream& stream, const std::vector<complex>& v);


