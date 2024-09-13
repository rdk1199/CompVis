#include <cfloat>

#include "bayes_classifier.h"

#include "../Math/stats.h"
#include "../Math/functions.h"

using std::vector;
using std::map;
using std::set;

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
		cout << "\r initial processing training image " << i + 1<< "/" << tr_data.size() << "      ";
		if (tr_data[i].size() != _dim) //check that all feature vectors are the correct size
		{
			cout << "NaiveBayesClassifier ERROR: all training feature vectors must have dimension _dim" << endl;
			exit(1);
		}

		means[class_to_idx[tr_classes[i]]] += tr_data[i];
		class_counts[class_to_idx[tr_classes[i]]] += 1;

		t_mean += tr_data[i];
	}

	cout << endl;

	for (int i = 0; i < means.size(); i++) //set means of each class 
	{
		means[i] /= class_counts[i];
	}

	t_mean /= tr_data.size();

	//now compute variances


	vector<S> cov_vec(_dim); //this will contain the diagonal elements of the covariance matrix -> assuming independent features (naive)

	for (int i = 0; i < tr_data.size(); i++)
	{
		cout << "\r variance computation; training image " << i + 1 << "/" << tr_data.size() << "       ";
		cov_vec += pow(tr_data[i] - t_mean, 2);
	}
	cov_vec /= tr_data.size();
	cout << endl;

	
	//DIMENSIONALITY REDUCTION - any features with variance 0 are useless, and thus can be eliminated to ensure a non-singular covariance matrix

	int old_dim = _dim;
	reduce_dimensions(cov_vec);

	cout << "removed "<< old_dim - _dim << " dimensions; _dim is now " << _dim << endl;
	//finally set covariance matrix - it was already initiailized in constructor (and resized in reduce_dimensions) so no resize needed

	log_cov_det = 0.0;
	//since the matrix is diagonal, the inverse is just the same matrix with the diagonal elements inverted, and the determinant is just the product of these elements
	for (int i = 0; i < _dim; i++)
	{
		log_cov_det += std::log(cov_vec[i]);
		cov_inv[i][i] = 1.0/cov_vec[i];
	}

	cout << "log(det): " << log_cov_det << endl;
}

template<class S, class T>
void NaiveBayesClassifier<S, T>::reduce_dimensions(std::vector<S>& cov_vec, double epsilon)
{
	int new_dim = _dim;
	S max_var = 0;


	for (int i = 0; i < cov_vec.size(); i++)
	{
		if (abs(cov_vec[i]) < epsilon)
		{
			removed_dimensions.insert(i);
			new_dim--;
		}

		max_var = cov_vec[i] > max_var ? cov_vec[i] : max_var;
	}

	_dim = new_dim;
	//scale = 10.0 / max_var;

	//now have to recompute means to get rid of excess dimensions //also rescale means at the same time
	for (int i = 0; i < means.size(); i++)
	{
		vector<S> new_means_i;
		for (int j = 0; j < means[i].size(); j++)
		{
			if (!removed_dimensions.count(j))
			{
				new_means_i.push_back(means[i][j]);
			}
		}

		means[i] = scale * new_means_i;
	}

	//resize cov_inv
	cov_inv = Matrix<S>(_dim, _dim);

	//finally, remove excess dimensions from cov_vec and rescale for use in training
	vector<S> new_cov_vec;

	for (int i = 0; i < cov_vec.size(); i++)
	{
		if (!removed_dimensions.count(i))
		{
			new_cov_vec.push_back(scale * cov_vec[i]);
		}
	}

	cov_vec = new_cov_vec;
}

template<class S, class T>
map<T, double> NaiveBayesClassifier<S,T>::prob_vec(const vector<S>& input)
{
	//must first remove excess dimensions from and rescale input;

	vector<S> new_input;

	for (int i = 0; i < input.size(); i++)
	{
		if (!removed_dimensions.count(i))
		{
			new_input.push_back(scale * input[i]);
		}
	}

	vector<double> log_px_ci(_classes.size()); //log(p(x|C_i))

	
	double max_log = -DBL_MAX; 

	for (int i = 0; i < _classes.size(); i++)
	{
		log_px_ci[i] = log_gaussian(new_input, log_cov_det, means[i], cov_inv);
		max_log = std::max(max_log, log_px_ci[i]);
		//px += px_ci[i] * _p_classes[i];
	}

	double px = 0; //total probability of x
	for (int i = 0; i < _classes.size(); i++)
	{
		log_px_ci[i] -= max_log; //subtract value of max for better numerical stability
		px += std::exp(log_px_ci[i]) *(_p_classes[i]);
	}

	map<T, double> out; //final probability vector

	for (int i = 0; i < _classes.size(); i++)
	{
		out[_classes[i]] = (std::exp(log_px_ci[i]) * _p_classes[i]) / px;

		if (out[_classes[i]] != out[_classes[i]])
		{
			cout << "out probability is NaN!" << endl << "new_input: " << new_input << endl << "px: " << px << endl << "log_px_ci: " << log_px_ci << endl;
			exit(1);
		}

	}

	return out;
}


template<class S, class T>
T NaiveBayesClassifier<S, T>::operator()(const vector<S>& input)
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