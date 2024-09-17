#include "logistic_classifier.h"

#include "../Math/functions.h"

using std::vector;

template<class S>
BinaryLogisticClassifier<S>::BinaryLogisticClassifier(int dim, const std::vector<std::vector<S>>& tr_data, const std::vector<int>& tr_classes):
	_dim(dim)
{
	compute_params(tr_data, tr_classes);
}

template<class S>
vector<double> BinaryLogisticClassifier<S>::prob_vec(const std::vector<S>& in)
{
	double p0 = logistic(weights * in);
	return { p0, 1.0 - p0 };
}

template<class S>
vector<S> BinaryLogisticClassifier<S>::compute_en_grad(const std::vector<std::vector<S>>& tr_data, const std::vector<int>& tr_classes, const vector<S>& y)
{
	vector<S> out(tr_data[0].size(), 0.0);

	for (int i = 0; i < tr_data.size(); i++)
	{
		out += (y[i] - tr_classes[i]) * tr_data[i];
	}

	return out;
}

template<class S>
Matrix<S> BinaryLogisticClassifier<S>::compute_hessian(const Matrix<S>& tr_data_matrix, const vector<S>& y)
{
	Matrix<S> R(y.size(), y.size());

	for (int i = 0; i < R.n_rows(); i++)
	{
		R[i][i] = y[i] * (1.0 - y[i]);
	}

	return tr_data_matrix.transpose() * R * tr_data_matrix;

}

template<class S> //
void BinaryLogisticClassifier<S>::compute_params(const std::vector<std::vector<S>>& tr_data, const std::vector<int>& tr_classes, int n_iter)
{
	Matrix<S> data_mat(tr_data);

	for (int i = 0; i < n_iter; i++)
	{
		vector<S> y;
		for (int i = 0; i < tr_data.size(); i++)
		{
			y.push_back(logistic(weights * tr_data[i]));
		}

		vector<S> en_grad = compute_en_grad(tr_data, tr_classes, y);
		Matrix<S> hessian = compute_hessian(data_mat, y);

		weights = weights - hessian.inverse() * en_grad;
	}

}

template class BinaryLogisticClassifier<double>;