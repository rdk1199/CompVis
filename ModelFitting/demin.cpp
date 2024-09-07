
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
void DEMinimizer<T>::construct_membrane_f_vec()
{
	SparseMatrix<double> hessian(g_width * g_height, g_width * g_height);
	vector<T> w_data(g_width * g_height);

	for (int i = 0; i < g_width; i++)
	{
		for (int j = 0; j < g_height; j++)
		{

			int px_ij = j * g_width + i;
			int px_ij1 = (j + 1) * g_width + i;
			int px_i1j = j * g_width + i + 1;

			hessian[px_ij][px_ij] += 2 * lambda;

			if (hessian.in_range(px_i1j, px_i1j))
			{
				hessian[px_i1j][px_i1j] += lambda;
				hessian[px_ij][px_i1j] -= lambda;
				hessian[px_i1j][px_ij] -= lambda;

			}

			if (hessian.in_range(px_ij1, px_ij1))
			{
				hessian[px_ij1][px_ij1] += lambda;
				hessian[px_ij][px_ij1] -= lambda;
				hessian[px_ij1][px_ij] -= lambda;
			}
		}
	}

	for (int i = 0; i < in_data.size(); i++)
	{
		int px_index = in_data[i].second * g_width + in_data[i].first;
		w_data[px_index] = constraint * out_data[i];

		hessian[px_index][px_index] += constraint; //add c_ij
	}

	f_vec = gauss_seidel_solve(hessian, w_data);
}

template<class T>
void DEMinimizer<T>::construct_thin_plate_f_vec()
{
	SparseMatrix<double> hessian(g_width * g_height, g_width * g_height);
	vector<T> w_data(g_width * g_height);

	for (int i = 0; i < g_width; i++)
	{
		for (int j = 0; j < g_height; j++)
		{

			int px_ij = j * g_width + i;
			int px_ij1 = (j + 1) * g_width + i;
			int px_i1j = j * g_width + i + 1;
			int px_i1j1 = (j + 1) * g_width + i + 1;

			int px_im1j = j * g_width + i - 1;
			int px_ijm1 = (j - 1) * g_width + i;

			hessian[px_ij][px_ij] += 10.0;

			if (hessian.in_range(px_i1j, px_i1j))
			{
				hessian[px_i1j][px_i1j] += 3.0;

				hessian[px_i1j][px_ij] -= 4.0;
				hessian[px_ij][px_i1j] -= 4.0;

			}

			if (hessian.in_range(px_im1j, px_im1j))
			{
				hessian[px_im1j][px_im1j] += 1.0;


				hessian[px_ij][px_im1j] -= 2.0;
				hessian[px_im1j][px_ij] -= 2.0;

				
			}

			if (hessian.in_range(px_ij1, px_ij1))
			{
				hessian[px_ij1][px_ij1] += 1.0;

				hessian[px_ij][px_ij1] -= 2.0;
				hessian[px_ij1][px_ij] -= 2.0;

			}

			if (hessian.in_range(px_ijm1, px_ijm1))
			{
				hessian[px_ijm1][px_ijm1] += 1.0;

				hessian[px_ij][px_ijm1] -= 2.0;
				hessian[px_ijm1][px_ij] -= 2.0;
			}

			if (hessian.in_range(px_im1j, px_i1j))
			{
				hessian[px_im1j][px_i1j] += 1.0;
				hessian[px_i1j][px_im1j] += 1.0;
			}

			if (hessian.in_range(px_ijm1, px_ij1))
			{
				hessian[px_ijm1][px_ij1] += 1.0;
				hessian[px_ij1][px_ijm1] += 1.0;				
			}

			if (hessian.in_range(px_ij1, px_ij1))
			{
				hessian[px_ij1][px_ij1] += 2.0;

				hessian[px_ij][px_ij1] -= 2.0;
				hessian[px_ij1][px_ij] -= 2.0;
			}

			if (hessian.in_range(px_i1j1, px_i1j1))
			{
				hessian[px_i1j1][px_i1j1] += 2.0;


				hessian[px_i1j][px_i1j1] -= 2.0;
				hessian[px_i1j1][px_i1j] -= 2.0;

				hessian[px_ij1][px_i1j1] -= 2.0;
				hessian[px_i1j1][px_ij1] -= 2.0;

				hessian[px_ij][px_i1j1] += 2.0;
				hessian[px_i1j1][px_ij] += 2.0;


				hessian[px_i1j][px_ij1] += 2.0;
				hessian[px_ij1][px_i1j] += 2.0;

			}
		}
	}

	hessian = lambda * hessian;

	for (int i = 0; i < in_data.size(); i++)
	{
		int px_index = in_data[i].second * g_width + in_data[i].first;
		w_data[px_index] = constraint * out_data[i];

		hessian[px_index][px_index] += constraint; //add c_ij
	}

	f_vec = gauss_seidel_solve(hessian, w_data);

	cout << p_norm(hessian * f_vec - w_data) << endl;
}

template<class T>
DEMinimizer<T>::DEMinimizer(int width, int height, std::vector<std::pair<int,int>> domain, std::vector<T> range, double reg, double force_fit, EnergyFunction e_func):
	g_width(width),
	g_height(height),
	in_data(domain),
	out_data(range),
	n_pts(in_data.size()),
	lambda(reg),
	constraint(force_fit)

{
	if (in_data.size() != out_data.size())
	{
		cout << "DEMinimizer constructor error : domain and range must be same size" << endl;
		exit(1);
	}

	switch (e_func)
	{
	case EnergyFunction::membrane:
			construct_membrane_f_vec();
	case EnergyFunction::thin_plate:
		construct_thin_plate_f_vec();
	}
}

template<class T>
T DEMinimizer<T>::operator()(int i, int j) const
{
	return f_vec[j * g_width + i];
}

template class DEMinimizer<Color>;