//
// Created by MINH NHUT on 7/30/2023.
//

#ifndef DATAVISUALIZATION2_RANDOM_HPP
#define DATAVISUALIZATION2_RANDOM_HPP

#include <random>
#include <vector>
#include <chrono>
#include <algorithm>

namespace Random {

int getInt(int Min, int Max);

std::vector<int> getArray(int minLength, int maxLength, int minVal, int maxVal);

template<typename T>
void shuffle(std::vector<T>& list) {
	std::shuffle(
	    list.begin(), list.end(),
	    std::default_random_engine(std::chrono::steady_clock::now().time_since_epoch().count()));
}
};  // namespace Random

#endif  //DATAVISUALIZATION2_RANDOM_HPP
