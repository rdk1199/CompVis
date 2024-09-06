#include "math_tests.h"

using std::vector;
using std::cout;
using std::endl;

void test_gauss_elimination()
{
	Matrix<float> A(1000, 1000);
	vector<float> b(1000);

	for (int i = 0; i < 1000; i++)
	{
		A[i][i] = i + 1;
		b[i] = 1000 - i;
	}


	vector<float> x = gauss_solve(A, b);

	cout << p_norm(A * x - b, 2) << endl;
}

void test_gauss_seidel(float epsilon)
{

	SparseMatrix<float> A(1000, 1000);
	vector<float> b(1000);

	for (int i = 0; i < 1000; i++)
	{
		A[i][i] = i+ 1;
		b[i] = 1000 - i;
	}


	vector<float> x = gauss_seidel_solve(A, b, .001);

//	vector<float> diff = A * x - b;

	cout << A*x - b << endl;
	cout << p_norm(A*x - b) << endl;
	




}

