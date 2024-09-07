#include "rand_util.h"

#include <ctime>
#include <algorithm>


RandEngine g_rand;

void rand_init(int seed)
{
	g_rand = RandEngine(seed);
}

RandEngine::RandEngine(int seed) :
	_rand(std::minstd_rand(seed ? seed : time(NULL)))
{}

int RandEngine::rand_int(int l, int u)
{
	if (l > u)
	{
		int tmp = l;
		l = u;
		u = tmp;
	}

	return l + _rand() % (u - l + 1);
}

float RandEngine::rand_float(float l, float u)
{
	if (l > u)
	{
		float tmp = l;
		l = u;
		u = tmp;
	}

	return l + (u - l) * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
}

bool RandEngine::flip(float p)
{
	p = std::clamp(p, 0.0f, 1.0f);

	return rand_float() <= p;

}

int RandEngine::draw(vector<float> dist)
{
	float p = rand_float();

	float total_prob = 0.0f;
	for (int i = 0; i < dist.size(); i++)
	{
		total_prob += dist[i];
		if (p <= total_prob)
		{
			return i;
		}
	}

	return dist.size() - 1; //may encounter some precision issues
}


