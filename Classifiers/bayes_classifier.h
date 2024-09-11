#pragma once

#include <vector>
#include <map>

#include "../Math/linalg.h"

template<class S, class T> //classify feature vectors with features of type S into classes of type T
class NaiveBayesClassifier
{
private:

	int _dim; //dimension of feature vectors
	std::vector<T> _classes; //class labels

	std::map<T, int> class_to_idx; //useful to have


	std::vector<std::vector<S>> means; //training vector means (one for each class)

	Matrix<S> cov_inv; //single covariance matrix for now -> store the INVERSES, since that's what's actually used
	S cov_det; //determinant of covariance matrix cov_mat

	std::vector<double> _p_classes; //class probabilities - can be given as input or computed


	//training helper functions
	void compute_p_classes(const std::vector<T>& tr_classes);
	void compute_params(const std::vector<std::vector<S>>& tr_data, const std::vector<T>& tr_classes); //computes means and covariances


public:

	NaiveBayesClassifier(int dim, const std::vector<T>& classes, const std::vector<std::vector<S>>& tr_data, const std::vector<T>& tr_classes, const std::vector<double>& p_classes = {}); //constructor/trainer

	std::map<T, double> prob_vec(std::vector<S> input); //get probabilities of each class given input feature vector
	T operator()(std::vector<S> input); //just classify input



};
