

#include <vector>

#include "../Math/linalg.h"

template<class S>
class BinaryLogisticClassifier //notation based off Bishop (2006) sec 4.3.2
{

private:

	int _dim;

	std::vector<S> weights;
	//S bias; 

	std::vector<S> compute_en_grad(const std::vector<std::vector<S>>& tr_data, const std::vector<int>& tr_classes, const std::vector<S>& y);
	Matrix<S> compute_hessian(const Matrix<S>& tr_data_matrix, const std::vector<S>& y);

	void compute_params(const std::vector<std::vector<S>>& tr_data, const std::vector<int>& tr_classes, int n_iter = 10);

public:

	//constructor/trainer
	BinaryLogisticClassifier(int dim, const std::vector<std::vector<S>>& tr_data, const std::vector<int>& tr_classes);
	
	//to classify new data
	int operator()(const std::vector<S>& in); //classify input 
	std::vector<double> prob_vec(const std::vector<S>& in); //return probabilities of each class - p[0], p[1]
};