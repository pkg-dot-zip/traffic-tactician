
#include <mutex>

#include "poseChecker.h"
Pose currentPose;
std::mutex mtx;

void setInputPose(const Pose& poseToSetTo)
{
	std::lock_guard lck(mtx);
	currentPose = poseToSetTo;
}

Pose& getInputPose()
{
	std::lock_guard lck(mtx);
	return currentPose;
}