
#include <iostream>
#include <utility>

#include "demin.h"
#include "../Math/linalg.h"
#include "../Math/solvers.h"
#include "../ImagProc/image.h"

using std::vector;
using std::cout;
using std::endl;

template<class T>
DEMinimizer<T>::DEMinimizer(int width, int height, std::vector<std::pair<int,int>> domain, std::vector<T> range, double reg):
	g_width(width),
	g_height(height),
	in_data(domain),
	out_data(range),
	n_pts(in_data.size()),
	lambda(reg)
{
	if (in_data.size() != out_data.size())
	{
		cout << "DEMinimizer constructor error : domain and range must be same size" << endl;
		exit(1);
	}

	SparseMatrix<double> hessian(width * height, width * height);
	vector<T> w_data(width * height);

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{

			int px_ij = j* width + i;
			int px_ij1 = (j + 1) * width + i;
			int px_i1j = j * width + i + 1;

			hessian[px_ij][px_ij] += 2 * lambda;

			if (hessian.in_range(px_i1j, px_i1j))
			{
				hessian[px_i1j][px_i1j] += lambda;
				hessian[px_ij][px_i1j] -= 2 * lambda;
			}

			if (hessian.in_range(px_ij1, px_ij1))
			{
				hessian[px_ij1][px_ij1] += lambda;
				hessian[px_ij][px_ij1] -= 2 * lambda;
			}
		}		
	}

	double c = 5;
	
	vector<double> r_data(width * height);
	vector<double>  g_data (width*height);
	vector<double>  b_data (width*height);
	vector<double>  a_data (width*height);

	for (int i = 0; i < domain.size(); i++)
	{
		int px_index = in_data[i].second * width + in_data[i].first;
		w_data[px_index] = c * out_data[i];


		r_data[px_index] = c * out_data[i].r;
		g_data[px_index] = c * out_data[i].g;
		b_data[px_index] = c * out_data[i].b;
		a_data[px_index] = c * out_data[i].a;



		hessian[px_index][px_index] += c; //add c_ij
	}



	

	vector<double> r_vec = gauss_seidel_solve(hessian, r_data);
	vector<double> g_vec = gauss_seidel_solve(hessian, g_data);
	vector<double> b_vec = gauss_seidel_solve(hessian, b_data);
	vector<double> a_vec = gauss_seidel_solve(hessian, a_data);


	f_vec.resize(r_data.size());
	
	for (int i = 0; i < f_vec.size(); i++)
	{
		f_vec[i] = { r_vec[i], g_vec[i], b_vec[i], a_vec[i] };
	}

	//= gauss_solve(hessian, w_data);


	//cout << p_norm(hessian * r_vec - r_data) << endl;
	//cout << p_norm(hessian * g_vec - g_data) << endl;
	//cout << p_norm(hessian * b_vec - b_data) << endl;
	//cout << p_norm(hessian * a_vec - a_data) << endl;
	//
	//cout << hessian.n_cols() << endl;

	/*
	Matrix<double> red_aug = hessian.attach_col(b_data).red_row_ech();

	for (int i = 0; i < red_aug.n_rows(); i++)
	{
		b_vec[i] = red_aug[i].back();
	}

	cout << "new: " << p_norm(hessian * b_vec - b_data) << endl;

	//cout << red_aug << endl;
	*/
	
	cout << p_norm(hessian * f_vec - w_data) << endl;

	//cout << hessian * f_vec - w_data <<endl;

	//cout << hessian;

}

template<class T>
T DEMinimizer<T>::operator()(int i, int j) const
{
	return f_vec[j * g_width + i];
}

template class DEMinimizer<Color>;