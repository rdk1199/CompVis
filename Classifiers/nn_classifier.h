#pragma once

#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <queue>
#include <map>
#include <algorithm>


template<class S, class T> //classifies objects of type S into categories of type T (ints, strings, etc.)
class NNClassifier
{

private:
	
	double (*_dist)(const S&, const S&); //distance function for nearest neighbor classification
	std::vector<T> _classes; //class labels

	std::vector<S> _tr_data; //input data points
	std::vector<T> _tr_classes; //input classes - each element corresponds to element in _tr_data
	
public:
	//constructor
	NNClassifier(std::vector<T> classes, std::vector<S> tr_data, std::vector<T> tr_classes, double (*dist)(const S&, const S&));

	T operator()(const S& input, unsigned int k); //k-nearest neighbor classification of input


};

template<class S, class T>
NNClassifier<S,T>::NNClassifier(std::vector<T> classes, std::vector<S> tr_data, std::vector<T> tr_classes, double (*dist)(const S&, const S&)):
	_classes(classes), 
	_dist(dist), 
	_tr_data(tr_data),
	_tr_classes(tr_classes)
{
	if (tr_data.empty())
	{
		std::cout << "ERROR : training data vector for NNClassifier must be nonempty" << std::endl;
		exit(1);
	}

	if (tr_data.size() != tr_classes.size())
	{
		std::cout << "ERROR: NNClassifier training data vector and training class vector must have same size" << std::endl;
		exit(1);
	}

}

template<class T>
struct LabelDistance
{
	T label;
	double dist;
};

template<class T>
class CompareLabelDistance
{
public:
	bool operator()(LabelDistance<T>& a, LabelDistance<T>& b)
	{
		return a.dist < b.dist;
	}
};

template<class S, class T>
T NNClassifier<S,T>::operator()(const S& input, unsigned int k) // this is the most naive algorithm possible; could do KD-trees or something else to make this much faster (it's painfully slow for now)
{
	k = std::min(k, static_cast<unsigned int>(_tr_data.size()));

	std::priority_queue<LabelDistance<T>, std::vector<LabelDistance<T>>, CompareLabelDistance<T>> queue;

	std::map<T, int> class_count;

	for (int i = 0; i < _tr_data.size(); i++) //first find k nearest neighbors
	{
		double distance = _dist(input, _tr_data[i]);

		if (queue.size() < k) //if there aren't even k elements in the queue yet, push in no matter what
		{
			queue.push(LabelDistance<T>{ _tr_classes[i], distance });
			class_count[_tr_classes[i]] += 1; //count the labels of the nearest neighbors as we go
			continue;
		}
	
		if (distance < queue.top().dist) //here, queue.size() == k -> replace biggest element in queue if a smaller one is found
		{
			class_count[queue.top().label] -= 1; //remove old max element
			queue.pop();

			class_count[_tr_classes[i]] += 1;  //add in new element
			queue.push(LabelDistance<T>{ _tr_classes[i], distance }); 
		}
	}
	
	T out = _classes[1];

	int max = 0;
	for (auto i = class_count.begin(); i != class_count.end(); i++) //now get max
	{
		if (i->second > max)
		{
			max = i->second;
			out = i->first;
		}
	}

	return out;
}

