#pragma once
#include <poseChecker.h>
class RandomPoseBag
{
	private:
		std::vector<Pose> bag; // keep track of the poses that have been generated
		std::map<Pose, int> poseCount; // used to regenerate the poses when the bag is empty
	public:
	// add a constructor with the distribution of poses to be generated
	RandomPoseBag(const int left, const int right, const int forward, const int stop);
	RandomPoseBag(const int sameSize);
	RandomPoseBag();
	~RandomPoseBag();
	Pose getPose();
	void generate();
};

