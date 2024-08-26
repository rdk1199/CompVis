#pragma once

#include <vector>


#include "numbers.h"

class IllegalVectorOp : public std::exception
{
	virtual const char* what() const throw()
	{
		return "invalid vector operation attempted";
	}
};

template<class T>
std::vector<T> operator+(const std::vector<T>& a, const std::vector<T>& b);

template<class T>
std::vector<T> operator-(const std::vector<T>& a, const std::vector<T>& b);

template<class T>
T operator*(const std::vector<T>& a, const std::vector<T>& b); //dot product

template<class S, class T>
std::vector<T> operator*(const S& c, const std::vector<T>& v); //scalar multiplication

template<class T>
inline std::ostream& operator<<(std::ostream& stream, const std::vector<T>& v);