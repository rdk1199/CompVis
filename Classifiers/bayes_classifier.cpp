#include "bayes_classifier.h"

#include "../Math/stats.h"
#include "../Math/functions.h"

using std::vector;
using std::map;

using std::cout;
using std::endl;


template<class S, class T>
NaiveBayesClassifier<S,T>::NaiveBayesClassifier(int dim, const std::vector<T>& classes, const std::vector<std::vector<S>>& tr_data, const std::vector<T>& tr_classes, const std::vector<double>& p_classes) :
	_dim(dim),
	_classes(classes),
	_p_classes(p_classes),
	cov_inv(Matrix<S>(_dim, _dim))
{
	if (tr_data.size() != tr_classes.size())
	{
		cout << "NaiveBayesClassifier ERROR: training data and training classes must have same size" << endl;
		exit(1);
	}

	if (tr_data.size() == 0)
	{
		cout << "Training set for NaiveBayesClassifier must be nonempty!" << endl;
		exit(1);
	}

	for (int i = 0; i < classes.size(); i++) //class_to_idx gets filled in here
	{
		class_to_idx[classes[i]] = i;
	}

	if (p_classes.empty()) //if probabilities were not provided, estimate them from training data
	{
		compute_p_classes(tr_classes);
	}

	if (_p_classes.size() != _classes.size())
	{
		cout << "NaiveBayesClassifier ERROR: number of classes must match number of class probabilities" << endl;
		exit(1);
	}

	compute_params(tr_data, tr_classes);
}

template<class S, class T> //probabilities are computed based on frequency of occurence in training set
void NaiveBayesClassifier<S, T>::compute_p_classes(const vector<T>& tr_classes)
{
	vector<int> class_count(_classes.size(), 0);

	for (int i = 0; i < tr_classes.size(); i++)
	{
		class_count[class_to_idx[tr_classes[i]]]++;
	}

	_p_classes.resize(_classes.size());

	for (int i = 0; i < _classes.size(); i++)
	{
		_p_classes[i] = static_cast<double>(class_count[i]) / tr_classes.size();
	}

}

template<class S, class T>
void NaiveBayesClassifier<S, T>::compute_params(const vector<vector<S>>& tr_data, const vector<T>& tr_classes)
{
	vector<unsigned int> class_counts(_classes.size(), 0); //keep track of how many instances of each class there are

	vector<S> t_mean(_dim); //total mean of all sample points
	means.resize(_classes.size(), vector<S>(_dim)); //this should 0 fill -> will hold means for each individual class

	for (int i = 0; i < tr_data.size(); i++) //we have already checked that tr_data.size() == tr_classes.size() in constructor
	{
		if (tr_data[i].size() != _dim) //check that all feature vectors are the correct size
		{
			cout << "NaiveBayesClassifier ERROR: all training feature vectors must have dimension _dim" << endl;
			exit(1);
		}

		means[class_to_idx[tr_classes[i]]] += tr_data[i];
		class_counts[class_to_idx[tr_classes[i]]] += 1;

		t_mean += tr_data[i];
	}

	for (int i = 0; i < means.size(); i++) //set means of each class 
	{
		means[i] /= class_counts[i];
	}

	t_mean /= tr_data.size();

	//now compute variances


	vector<S> cov_vec(_dim); //this will contain the diagonal elements of the covariance matrix -> assuming independent features (naive)

	for (int i = 0; i < tr_data.size(); i++)
	{
		cov_vec += pow(tr_data[i] - t_mean, 2);
	}

	cov_vec /= tr_data.size();

	//finally set covariance matrix - it was already initiailized in constructor so no resize needed -first compute just covariance matrix 

	for (int i = 0; i < _dim; i++)
	{
		cov_inv[i][i] = cov_vec[i];
	}
	
	cov_det = cov_inv.det(); //this can be computed at the same time as the inverse - implement that later <- also this matrix is diagonal so no need to do either of these
	cov_inv = cov_inv.inverse();
}

template<class S, class T>
map<T, double> NaiveBayesClassifier<S,T>::prob_vec(vector<S> input)
{
	vector<S> px_ci(_classes.size()); //p(x|C_i)

	S px; //total probability of x

	for (int i = 0; i < _classes.size(); i++)
	{
		px_ci[i] = gaussian(input, cov_det, means[i], cov_inv);
		px += px_ci[i] * _p_classes[i];
	}

	map<T, double> out; //final probability vector

	for (int i = 0; i < _classes.size(); i++)
	{
		out[_classes[i]] = (px_ci[i] * _p_classes[i]) / px;
	}

	return out;
}


template<class S, class T>
T NaiveBayesClassifier<S, T>::operator()(vector<S> input)
{
	map<T, double> p_map = prob_vec(input);

	//will return class with highest probability
	//initialize max to some safe value
	T max_class = p_map.begin()->first;
	double max_prob = p_map.begin()->second;

	for (auto i = p_map.begin(); i != p_map.end(); i++) //just iterate through map to find max probability
	{
		if (i->second > max_prob)
		{
			max_class = i->first;
			max_prob = i->second;
		}
	}

	return max_class;
}


template class NaiveBayesClassifier<double, int>;