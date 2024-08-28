#pragma once

#include <vector>


#include "numbers.h"

class IllegalVectorOp : public std::exception
{
	virtual const char* what() const throw()
	{
		return "ERROR: invalid vector operation attempted";
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

class IllegalMatrixConstruction : public std::exception
{
	virtual const char* what() const throw()
	{
		return "ERROR: invalid matrix construction attempted";
	}
};

class IllegalMatrixOp : public std::exception
{
	virtual const char* what() const throw()
	{
		return "ERROR: invalid matrix operation attempted";
	}
};

template<class T>
class Matrix
{
private:
	std::vector<std::vector<T>>matrix;

public: 
	std::vector<T>& operator[](int i) { return matrix[i]; }
	std::vector<T> at(int i) const {  return matrix[i];	}; //get i'th row
	T at(int i, int j) const { return matrix[i][j]; };

	int n_rows() const { return matrix.size(); }
	int n_cols() const { return matrix.size()? matrix[0].size() : 0; }

	bool is_square() const { return matrix.size() ? matrix.size() == matrix[0].size() : true; }

	static Matrix<T> identity(int n); //n by n identity matrix

	Matrix<T>(int rows, int columns); //construct zero matrix of given size
	Matrix<T>(const std::vector<std::vector<T>>& entries); //construct matrix from values 

	Matrix<T> transpose() const;

};


template<class T> Matrix<T> operator+(const Matrix<T>& A, const Matrix<T>& B);
template<class T> Matrix<T> operator-(const Matrix<T>& A, const Matrix<T>& B);
template<class T> Matrix<T> operator*(const Matrix<T>& A, const Matrix<T>& B);
template<class T> Matrix<T> operator*(const T& c, const Matrix<T>& A);
template<class T> std::vector<T> operator*(const Matrix<T>& A, std::vector<T>& x);
template<class T> std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix);

