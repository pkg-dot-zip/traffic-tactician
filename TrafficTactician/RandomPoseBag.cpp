#include "RandomPoseBag.h"
#include <algorithm>
#include <random>
#include <easylogging++.h>

// sets the distribution of poses to be generated
RandomPoseBag::RandomPoseBag(const int left, const int right, const int forward, const int stop)
{
	poseCount[POSE_MOVE_LEFT] = left;
	poseCount[POSE_MOVE_RIGHT] = right;
	poseCount[POSE_MOVE_FORWARD] = forward;
	poseCount[POSE_STOP] = stop;

	generate();
}

// set the distribution of poses to be generated to the same size
RandomPoseBag::RandomPoseBag(const int sameSize = 2)
{
	poseCount[POSE_MOVE_LEFT] = sameSize;
	poseCount[POSE_MOVE_RIGHT] = sameSize;
	poseCount[POSE_MOVE_FORWARD] = sameSize;
	poseCount[POSE_STOP] = sameSize;

	generate();
}

RandomPoseBag::~RandomPoseBag() = default;

Pose RandomPoseBag::getPose()
{

	if (!bag.empty())
	{
		Pose pose = bag.back(); // get the last element
		bag.pop_back(); // remove the last element
		return pose;
	}
	else
	{
		LOG(INFO) << "RandomPoseBag is empty. Refilling bag." << std::endl;
		generate();

		return getPose();
	}
	return Pose();
}

void RandomPoseBag::generate()
{
	// create a random pose bag using the distribution of poseCount
	for (const auto& element : poseCount) {
		std::cout << element.first << ": " << element.second << std::endl;
		Pose pose = element.first;
		int count = element.second;
		for (size_t i = 0; i < count; i++)
		{
			bag.push_back(pose);
		}
	}

	// Create a random number generator object
	std::random_device rd;
	std::mt19937 gen(rd());

	// Shuffle the vector using std::shuffle
	std::shuffle(bag.begin(), bag.end(), gen);

	LOG(INFO) << "RandomPoseBag has been refilled." << std::endl;
}


