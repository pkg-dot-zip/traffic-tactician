#pragma once
#include <random>

class RandomNumberGenerator {

private:
	std::random_device rd_;
	std::mt19937 generator_;
	std::uniform_int_distribution<> dis_;
public:
	// Constructor to define the range for random number generation
	RandomNumberGenerator(int min, int max) {
		generator_ = std::mt19937(rd_());
		dis_ = std::uniform_int_distribution<>(min, max);
	}

	// Generates a random number within the defined range
	int getRandomNumber() {
		return dis_(generator_);
	}

	std::mt19937 getGenerator() {
		return generator_;
	}
};

