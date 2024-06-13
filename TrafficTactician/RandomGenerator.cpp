#include "RandomGenerator.h"

namespace RandomGenerator
{
	std::random_device rd_;
	std::mt19937 generator_ = std::mt19937(rd_());

	std::uniform_int_distribution<> dis_;

	int getRandomNumber(int min, int max) {
		dis_ = std::uniform_int_distribution<>(min, max);
		return dis_(generator_);
	}
}
