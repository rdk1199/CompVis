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
inline float abs(const std::vector<T>& v) //magnitude
{
	return std::sqrt(v * v);
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
	std::vector<std::vector<T>>m;


	
public: 
	std::vector<T>& operator[](int i) { return m[i]; }  //get i'th row
	T at(int i, int j) const { return m[i][j]; };

	int n_rows() const { return m.size(); }
	int n_cols() const { return m.size()? m[0].size() : 0; }

	bool is_square() const { return m.size() ? m.size() == m[0].size() : true; }
	bool in_range(int i, int j) const { return 0 <= i && i < n_rows() && 0 <= j && j < n_cols(); }

	static Matrix<T> identity(int n); //n by n identity matrix

	Matrix<T>(int rows, int columns); //construct zero matrix of given size
	Matrix<T>(const std::vector<std::vector<T>>& entries); //construct matrix from values 

	Matrix<T> transpose() const;

	Matrix<T> quick_inv_3() const; //quickly invert a 3x3 matrix -> will not function on other sizes
	
	Matrix<T> submatrix(int r1, int r2, int c1, int c2) const; //submatrix of entries in rows r1 ... r2 and cols c1 ... c2

	//elementary row operations - for efficiency, these don't do bounds checking - careful!
	void swap_rows(int r1, int r2); 
	void multiply_row(int row, T val);
	void add_multiple_of_row(int r_dest, int r_src, T mult); //r_dest += mult * r_src

	//misc operations
	Matrix<T> attach_col(std::vector<T> b) const; //attach column vector b to right of matrix - size of b must = n_rows of matrix

	//inversion/Gaussian elimination stuff
	Matrix<T> row_ech(T* det = nullptr) const; //row_ech computes determinant on the fly and places it in det if det is non-null and matrix is square
	Matrix<T> red_row_ech(T* det = nullptr) const;

	//square matrices only
	T det() const; //does row_ech purely to determine determinant
	Matrix<T> id_augment() const; //augment a square matrix with the identity
	Matrix<T> inverse() const;

	//2D transformation matrices (homogeneous coordinates, so return 3x3 matrix)
	static Matrix<float> translate_2d(float dx, float dy);
	static Matrix<float> rotate_2d(float angle); //angle in degrees
	static Matrix<float> scale_2d(float sx, float sy);


};


template<class T> Matrix<T> operator+(const Matrix<T>& A, const Matrix<T>& B);
template<class T> Matrix<T> operator-(const Matrix<T>& A, const Matrix<T>& B);
template<class T> Matrix<T> operator*(const Matrix<T>& A, const Matrix<T>& B);
template<class T> Matrix<T> operator*(const T& c, const Matrix<T>& A);
template<class T> std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix);


//matrix vector multiplication
template<class S, class T> 
inline std::vector<T> operator*(const Matrix<S>& A, const std::vector<T>& x)
{
	if (x.size() != A.n_cols())
	{
		throw IllegalMatrixOp();
	}

	std::vector<T> product(A.n_rows());

	for (int i = 0; i < product.size(); i++)
	{
		product[i] = A.at(i, 0) * x[0];
		for (int j = 1; j < x.size(); j++)
		{
			product[i] = product[i] + A.at(i, j) * x[j];
		}
	}

	return product;
}