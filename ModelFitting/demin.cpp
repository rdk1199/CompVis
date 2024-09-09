
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
void DEMinimizer<T>::construct_membrane_f_vec(double tol, int max_iter)
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

			if (hessian.in_range(px_i1j, px_i1j))
			{
				hessian[px_ij][px_ij] += 1;
				hessian[px_i1j][px_i1j] += 1;
				hessian[px_ij][px_i1j] -= 1;
				hessian[px_i1j][px_ij] -= 1;

			}

			if (hessian.in_range(px_ij1, px_ij1))
			{
				hessian[px_ij][px_ij] += 1;
				hessian[px_ij1][px_ij1] += 1;
				hessian[px_ij][px_ij1] -= 1;
				hessian[px_ij1][px_ij] -= 1;
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


	

	f_vec = sor_solve(hessian, w_data, tol, max_iter, 1.75);

	//f_vec = vector<Color>(g_width * g_height, Color::red());

	/*
	cout << "total energy:" << f_vec * (hessian * f_vec) - 2 * f_vec * w_data + Color{ 255.0 * 255.0, 0, 0, 255.0 * 255.0 } << endl;

	cout << "smoothness energy: " << f_vec * (hessian * f_vec) - (f_vec[in_data[0].second * g_width + in_data[0].first] * f_vec[in_data[0].second * g_width + in_data[0].first]) << endl;
	cout << "data energy: " << (f_vec[in_data[0].second * g_width + in_data[0].first] * f_vec[in_data[0].second * g_width + in_data[0].first]) - 2 * f_vec * w_data + Color{ 255.0 * 255.0, 0, 0, 255.0 * 255.0 } << endl;
	*/
	
	/*
	Color total = Color::zero();

	for (int i = 0; i < g_width; i++)
	{
		for (int j = 0; j < g_height; j++)
		{

			int px_ij = j * g_width + i;
			int px_ij1 = (j + 1) * g_width + i;
			int px_i1j = j * g_width + i + 1;

			if (hessian.in_range(px_i1j, px_i1j))
			{
				total += (f_vec[px_i1j] - f_vec[px_ij]) * (f_vec[px_i1j] - f_vec[px_ij]);

			}

			if (hessian.in_range(px_ij1, px_ij1))
			{
				total += (f_vec[px_ij1] - f_vec[px_ij]) * (f_vec[px_ij1] - f_vec[px_ij]);
			}
		}
	}

	cout << "actual smoothness energy: " << total << endl;

	Color data_total = Color::zero();

	for (int i = 0; i < in_data.size(); i++)
	{
		data_total += (f_vec[in_data[0].second * g_width + in_data[0].first] - out_data[0]) * (f_vec[in_data[0].second * g_width + in_data[0].first] - out_data[0]);
	}

	cout << "actual data energy: " << data_total << endl;
	cout << "total energy: " << total + data_total << endl;
	//cout << w_data << endl;
	*/
}

template<class T>
void DEMinimizer<T>::construct_thin_plate_f_vec(double tol, int max_iter)
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


			if (hessian.in_range(px_i1j, px_im1j)) //first smoothness term
			{
				hessian[px_ij][px_ij] += 4.0;
				hessian[px_i1j][px_i1j] += 1.0;

				hessian[px_i1j][px_ij] -= 2.0;
				hessian[px_ij][px_i1j] -= 2.0;

				hessian[px_ij][px_im1j] -= 2.0;
				hessian[px_im1j][px_ij] -= 2.0;

				hessian[px_im1j][px_im1j] += 1.0;

				hessian[px_im1j][px_i1j] += 1.0;
				hessian[px_i1j][px_im1j] += 1.0;
			}

			if (hessian.in_range(px_ij1, px_ijm1)) //third smoothness term
			{
				hessian[px_ij][px_ij] += 4.0;

				hessian[px_ijm1][px_ijm1] += 1.0;

				hessian[px_ij1][px_ij1] += 1.0;

				hessian[px_ij][px_ijm1] -= 2.0;
				hessian[px_ijm1][px_ij] -= 2.0;

				hessian[px_ij][px_ij1] -= 2.0;
				hessian[px_ij1][px_ij] -= 2.0;

				hessian[px_ijm1][px_ij1] += 1.0;
				hessian[px_ij1][px_ijm1] += 1.0;
			}



			if (hessian.in_range(px_i1j1, px_i1j1)) //second smoothness term
			{
				hessian[px_ij][px_ij] += 2.0;
				hessian[px_i1j][px_i1j] += 2.0;


				hessian[px_i1j][px_ij] -= 2.0;
				hessian[px_ij][px_i1j] -= 2.0;

				hessian[px_ij1][px_ij1] += 2.0;

				hessian[px_ij][px_ij1] -= 2.0;
				hessian[px_ij1][px_ij] -= 2.0;

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

	f_vec = sor_solve(hessian, w_data, tol, max_iter, 1.75);
}

template<class T>
DEMinimizer<T>::DEMinimizer(int width, int height, std::vector<std::pair<int,int>> domain, std::vector<T> range, double reg, double force_fit, EnergyFunction e_func, double tol, int max_iter):
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
		construct_membrane_f_vec(tol, max_iter);
		break;
	case EnergyFunction::thin_plate:
		construct_thin_plate_f_vec(tol, max_iter);
		break;
	}
}

template<class T>
T DEMinimizer<T>::operator()(int i, int j) const
{
	return f_vec[j * g_width + i];
}

template class DEMinimizer<Color>;