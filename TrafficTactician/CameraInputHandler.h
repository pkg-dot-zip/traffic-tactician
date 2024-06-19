#pragma once
#include "poseChecker.h"
namespace cameraInputHandler
{
	int getCameraToUse();
	void setCameraToUse(int camera);
	void setInputPose(const Pose& poseToSetTo);
	Pose& getInputPose();
	void initCameraInput(int cameraDevice);
}
