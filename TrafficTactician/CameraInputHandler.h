#pragma once
#include "poseChecker.h"
namespace cameraInputHandler
{
	void setInputPose(const Pose& poseToSetTo);
	Pose& getInputPose();
	void initCameraInput();
}
