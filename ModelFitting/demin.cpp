
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
DEMinimizer<T>::DEMinimizer(int width, int height, std::vector<std::pair<int,int>> domain, std::vector<T> range, float reg):
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

	SparseMatrix<float> hessian(width * height, width * height);
	vector<T> w_data(width * height);


	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{

			int px_ij = j* width + i;
			int px_ij1 = (j + 1) * width + i;
			int px_i1j = j * width + i + 1;

			hessian[px_ij][px_ij] = 1 + 2 * lambda;

			if (hessian.in_range(px_i1j, px_i1j))
			{
				hessian[px_i1j][px_i1j] = lambda;
				hessian[px_ij][px_i1j] = -2 * lambda;
			}

			if (hessian.in_range(px_ij1, px_ij1))
			{
				hessian[px_ij1][px_ij1] = lambda;
				hessian[px_ij][px_ij1] = -2 * lambda;
			}
		}		
	}


	for (int i = 0; i < domain.size(); i++)
	{
		int px_index = height * in_data[i].first + in_data[i].second;
		w_data[px_index] = out_data[i];
	}

	f_vec = gauss_seidel_solve(hessian, w_data, .001);
}

template<class T>
T DEMinimizer<T>::operator()(int i, int j) const
{
	return f_vec[j * g_width + i];
}

template class DEMinimizer<Color>;