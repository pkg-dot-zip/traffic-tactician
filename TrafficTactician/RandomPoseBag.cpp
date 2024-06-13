#include "RandomPoseBag.h"
#include <easylogging++.h>
#include <RandomNumberGenerator.h>


RandomPoseBag::RandomPoseBag(int size = 100)
{
	if (size <= 4)
	{
		LOG(ERROR) << "RandomPoseBag size must be greater than 4. Defaulting to 100." << std::endl;
		size = 100;
	}
	generate(size);
}

RandomPoseBag::~RandomPoseBag() = default;

Pose RandomPoseBag::getPose()
{
	if (bag.size() < 4)
	{
		LOG(ERROR) << "RandomPoseBag has less than 4 poses. Refilling bag." << std::endl;
		generate(100);
	}

	Pose pose = bag.back();
	bag.pop_back();

	return pose;
}

void RandomPoseBag::generate(int bagSize)
{
	addToBag(POSE_MOVE_LEFT, bagSize / 4);
	addToBag(POSE_MOVE_RIGHT, bagSize / 4);
	addToBag(POSE_MOVE_FORWARD, bagSize / 4);
	addToBag(POSE_STOP, bagSize / 4);

	// shuffle the bag
	RandomNumberGenerator randomGenerator(0, bag.size() - 1);
	std::shuffle(bag.begin(), bag.end(), randomGenerator.getGenerator());

	LOG(INFO) << "RandomPoseBag has been refilled." << std::endl;
}

void RandomPoseBag::addToBag(Pose pose, int count) {
	for (int i = 0; i < count; ++i) {
		bag.push_back(pose);
	}
}

