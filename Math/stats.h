#pragma once

#include <vector>

template<class T>
T median(std::vector<T>& vals);

template<class T>
T mean(const std::vector<T>& vals);

template<class T>
T threshold(T val, T thresh); //binary function returns 1 <=> val >= thresh

