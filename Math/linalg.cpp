
#include <iostream>

#include "linalg.h"


using std::vector;
using std::cout;
using std::endl;

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

	T out = T{ 0.0f };

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


template<class T>
Matrix<T>::Matrix(int rows, int columns) :
	matrix({})
{
	if (rows < 0 || columns < 0)
	{
		cout << "cannot create matrix with negative rows or columns" << endl;
		throw IllegalMatrixConstruction();

	}

	matrix.resize(rows);

	for (int i = 0; i < rows; i++)
	{
		matrix[i].resize(columns);
	}
}

template<class T>
Matrix<T>::Matrix(const vector<vector<T>>& entries) :
	matrix(entries)
{
	for (int i = 1; i < matrix.size(); i++)
	{
		if (matrix[i].size() != matrix[0].size())
		{
			cout << "cannot create matrix with different sized rows" << endl;
			throw IllegalMatrixConstruction();
		}
	}
}


template<class T>
Matrix<T> Matrix<T>::transpose() const
{
	Matrix<T> transpose(matrix[0].size(), matrix.size());

	for (int i = 0; i < matrix[0].size(); i++)
	{
		for (int j = 0; j < matrix.size(); j++)
		{
			transpose[i][j] = matrix[j][i];
		}
	}

	return transpose;
}


template<class T>
Matrix<T> operator+(const Matrix<T>& A, const Matrix<T>& B)
{
	if (A.n_rows() != B.n_rows() || A.n_cols() != B.n_cols())
	{
		throw IllegalMatrixOp();
	}

	Matrix<T> sum(A.n_rows(), A.n_cols());

	for (int i = 0; i < A.n_rows(); i++)
	{
		for (int j = 0; j < A.n_cols(); j++)
		{
			sum[i][j] = A.at(i, j) + B.at(i, j);
		}
	}

	return sum;
}

template<class T>
Matrix<T> operator-(const Matrix<T>& A, const Matrix<T>& B)
{
	if (A.n_rows() != B.n_rows() || A.n_cols() != B.n_cols())
	{
		throw IllegalMatrixOp();
	}

	Matrix<T> diff(A.n_rows(), A.n_cols());

	for (int i = 0; i < A.n_rows(); i++)
	{
		for (int j = 0; j < A.n_cols(); j++)
		{
			diff[i][j] = A.at(i, j) - B.at(i, j);
		}
	}

	return diff;
}

template<class T>
Matrix<T> operator*(const Matrix<T>& A, const Matrix<T>& B)
{
	if (A.n_cols() != B.n_rows())
	{
		throw IllegalMatrixOp();
	}

	Matrix<T> product(A.n_rows(), B.n_cols());

	for (int i = 0; i < product.n_rows(); i++)
	{
		for (int j = 0; j < product.n_cols(); j++)
		{
			product[i][j] = 0;

			for (int k = 0; k < A.n_cols(); k++)
			{
				product[i][j] += A.at(i, k) * B.at(k, j);
			}
		}
	}

	return product;
}

template<class T>
Matrix<T> operator*(const T& c, const Matrix<T>& A)
{
	Matrix<T> product(A.n_rows(), A.n_cols());

	for (int i = 0; i < product.n_rows(); i++)
	{
		for (int j = 0; j < product.n_cols(); j++)
		{
			product[i][j] = c * A.at(i, j);
		}
	}

	return product;
}

template<class T>
std::vector<T> operator*(const Matrix<T>& A, std::vector<T>& x)
{
	if (x.size() != A.n_cols())
	{
		throw IllegalMatrixOp();
	}

	std::vector<T> product(A.n_rows());

	for (int i = 0; i < product.size(); i++)
	{
		product[i] = 0;
		for (int j = 0; j < x.size(); j++)
		{
			product[i] += A.at(i, j) * x[j];
		}
	}

	return product;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix)
{
	for (int i = 0; i < matrix.n_rows(); i++)
	{
		for (int j = 0; j < matrix.n_cols(); j++)
		{
			os << matrix.at(i, j) << " ";
		}

		os << '\n';
	}

	return os;
}

//vector operations
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


//matrix operations
template Matrix<float> operator+(const Matrix<float>& A, const Matrix<float>& B);
template Matrix<complex> operator+(const Matrix<complex>& A, const Matrix<complex>& B);

template Matrix<float> operator*(const Matrix<float>& A, const Matrix<float>& B);
template Matrix<complex> operator*(const Matrix<complex>& A, const Matrix<complex>& B);

template Matrix<float> operator*(const float& c, const Matrix<float>& A);
template Matrix<complex> operator*(const complex& c, const Matrix<complex>& A);

template std::vector<float> operator*(const Matrix<float>& A, std::vector<float>& x);
template std::vector<complex> operator*(const Matrix<complex>& A, std::vector<complex>& x);


template std::ostream& operator<<(std::ostream& os, const Matrix<float>& matrix);
template std::ostream& operator<<(std::ostream& os, const Matrix<complex>& matrix);