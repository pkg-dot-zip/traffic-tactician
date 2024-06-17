#include <mutex>
#include "easylogging++.h"
#include "poseChecker.h"
#include "poseRetriever.h"
#include "SettingsRetriever.h"

Pose currentPose;
std::mutex mtx;

namespace cameraInputHandler
{
	int cameraToUseForInput = GetDNNSettings().cameraToUse;

	int getCameraToUse()
	{
		return cameraToUseForInput;
	}

	void setCameraToUse(int camera)
	{
		LOG(INFO) << "Setting camera to initialize the input with to camera " << cameraToUseForInput << "!" << std::endl;
		cameraToUseForInput = camera;
	}

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
