#pragma once



#include <random>
#include <set>
#include <iostream>
#include <iterator>
#include <vector>

using std::set;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

//helper class that implements some basic random number functions
class RandEngine
{
private: 

	std::minstd_rand _rand;

public:

	RandEngine(int seed = 0);

	int rand_int(int l = 0, int u = 1); //uniform random integer in [l,u]
	float rand_float(float l = 0.0f, float u = 1.0f); //uniform float in [l,u]
	bool flip(float p = 0.5f); //"flip" as in coin flip; returns true with probability p; p is clamped to [0,1]
	int draw(vector<float> dist); //draws an integer in the range [0, dist.size() -1] based on the distribution in dist; assumes elements of dist form a probability distribution

	template<class T>
	T pick(const set<T>& opts) //pick an element uniformly at random from opts; error if set is empty
	{
		if (opts.empty())
		{
			cerr << "cannot pick an element from an empty set" << endl;
			exit(-1);
		}

		unsigned int r_index = rand_int(0, opts.size() - 1);

		auto it = opts.begin();
		std::advance(it, r_index);
		return *it;
	}

	template<class T>
	set<T> pick_subset(set<T> opts, unsigned int num) //pick a subset of size num u.a.r. from opts
	{
		if (num == 0 || num > opts.size())
		{
			cerr << "invalid arguments passed to pick subset" << endl;
			exit(-1);
		}

		set<T> out;
		for (int i = 0; i < num; i++)
		{
			T elt = pick(opts);
			opts.erase(elt);
			out.insert(elt);
		}

		return out;
	}

};

extern RandEngine g_rand;
extern void rand_init(int seed = 0);


