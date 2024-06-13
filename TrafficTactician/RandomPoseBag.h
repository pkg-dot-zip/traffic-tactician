#pragma once
#include <poseChecker.h>
class RandomPoseBag
{
	std::vector<Pose> bag;
public:
	RandomPoseBag(int size);
	~RandomPoseBag();

	Pose getPose();
	void addToBag(Pose pose, int count);
	void generate(int size);
};

