
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
	m({})
{
	if (rows < 0 || columns < 0)
	{
		cout << "cannot create matrix with negative rows or columns" << endl;
		throw IllegalMatrixConstruction();

	}

	m.resize(rows);

	for (int i = 0; i < rows; i++)
	{
		m[i].resize(columns);
	}
}

template<class T>
Matrix<T>::Matrix(const vector<vector<T>>& entries) :
	m(entries)
{
	for (int i = 1; i < m.size(); i++)
	{
		if (m[i].size() != m[0].size())
		{
			cout << "cannot create matrix with different sized rows" << endl;
			throw IllegalMatrixConstruction();
		}
	}
}


template<class T>
Matrix<T> Matrix<T>::transpose() const
{
	Matrix<T> transpose(m[0].size(), m.size());

	for (int i = 0; i < m[0].size(); i++)
	{
		for (int j = 0; j < m.size(); j++)
		{
			transpose[i][j] = m[j][i];
		}
	}

	return transpose;
}

template<class T>
Matrix<T> Matrix<T>::identity(int n)
{
	Matrix<T> id(n, n);
	
	for (int i = 0; i < n; i++)
	{
		id[i][i] = 1;
	}

	return id;
}

template<class T>
Matrix<T> Matrix<T>::quick_inv_3() const
{
	if (n_rows() != 3 || n_cols() != 3)
	{
		cout << "cannot apply quick 3x3 inverse to non 3x3 matrix" << endl;
		exit(1);
	}

	T a = m[0][0];
	T b = m[0][1];
	T c = m[0][2];
	T d = m[1][0];
	T e = m[1][1];
	T f = m[1][2];
	T g = m[2][0];
	T h = m[2][1];
	T i = m[2][2];
	
	T A  = e*i - f*h;
	T B =  -(d*i - f * g);
	T C  = d*h - e*g;
	T D  = -(b * i - c * h);
	T E  = a*i - c * g;
	T F  = -(a*h - b*g);
	T G  = b*f - c*e;
	T H  =-(a*f - c *d);
	T I  = a*e - b*d;
	
	T det_A = a * A + b * B + c * C;

	if (det_A == 0) //return the zero matrix if non-invertible
	{
		return Matrix<T>(3, 3);
	}
	
	return (1.0f / det_A) * Matrix<T>({
		{A, D, G},
		{B, E, H},
		{C, F, I}});

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

template<class T>
Matrix<float> Matrix<T>::translate_2d(float dx, float dy)
{
	Matrix<float> out = Matrix<float>::identity(3);

	out[0][2] = dx;
	out[1][2] = dy;

	return out;
}

template<class T>
Matrix<float> Matrix<T>::rotate_2d(float angle)
{
	angle *= DEG2RAD; //convert to radians

	float cos = std::cos(angle);
	float sin = std::sin(angle);

	Matrix<float> out(3, 3);

	out[0][0] = cos;
	out[0][1] = -sin;
	out[1][0] = sin;
	out[1][1] = cos;
	out[2][2] = 1;

	return out;
}

template<class T>
Matrix<float> Matrix<T>::scale_2d(float sx, float sy)
{
	Matrix<float> out = Matrix<float>::identity(3);

	out[0][0] = sx;
	out[1][1] = sy;

	return out;
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


template class Matrix<float>;
template class Matrix<complex>;

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