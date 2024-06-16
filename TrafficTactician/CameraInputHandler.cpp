#include <mutex>
#include "easylogging++.h"
#include "poseChecker.h"
#include "poseRetriever.h"

Pose currentPose;
std::mutex mtx;

namespace cameraInputHandler
{
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

	void initCameraInput(int cameraDevice = -1)
	{
		LOG(INFO) << "Launching thread for camera detection." << std::endl;
		std::thread thread_object(runPoseRetriever, cameraDevice);
		thread_object.detach();
	}
}
