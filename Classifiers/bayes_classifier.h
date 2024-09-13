#pragma once

#include <set>
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
	S log_cov_det; //log of determinant of covariance matrix cov_mat -> better to work with log because determinant can be really big or small

	std::vector<double> _p_classes; //class probabilities - can be given as input or computed

	//data modifications
	std::set<int> removed_dimensions; //some components of the feature vectors may prove useless (i.e. are constant), so are removed.
	double scale = 1.0; //data will be rescaled so max variance is 1 -> 

	//training helper functions
	void compute_p_classes(const std::vector<T>& tr_classes);
	void compute_params(const std::vector<std::vector<S>>& tr_data, const std::vector<T>& tr_classes); //computes means and covariances
	void reduce_dimensions(std::vector<S>& cov_vec, double epsilon = 1e-8); //inspects covariance vector after training for any 0's (i.e. constant features) and removes those dimensions from all class parameters - also reduces dimension of cov_vec and computes scaling factor

public:

	NaiveBayesClassifier(int dim, const std::vector<T>& classes, const std::vector<std::vector<S>>& tr_data, const std::vector<T>& tr_classes, const std::vector<double>& p_classes = {}); //constructor/trainer

	std::map<T, double> prob_vec(const std::vector<S>& input); //get probabilities of each class given input feature vector
	T operator()(const std::vector<S>& input); //just classify input



};
