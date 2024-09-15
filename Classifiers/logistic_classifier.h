

#include <vector>


template<class S>
class BinaryLogisticClassifier
{

private:

	int _dim;

	std::vector<S> weights;
	S bias; 

	std::vector<double> _p_classes;

	void compute_params(const std::vector<std::vector<S>>& tr_data, const std::vector<int>& tr_classes);

public:

	//constructor/trainer
	BinaryLogisticClassifier(int dim, const std::vector<std::vector<S>>& tr_data, const std::vector<int>& tr_classes, const std::vector<double>& p_classes = {});
	
	//to classify new data
	int operator()(const std::vector<S>& in); //classify input 
	std::vector<double> prob_vec(const std::vector<S>& in); //return probabilities of each class - p[0], p[1]
};