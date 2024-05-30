/**
 * This code will launch, and contain the code for the tasks, of the pose estimation thread.
 */

#include<opencv2/dnn.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include "easylogging++.h"

#include<chrono>
#include<random>
#include<set>
#include<cmath>

#include "keyPoint.h"
#include "poseChecker.h"
#include "poseEstimation.h"
#include "settingsFromJson.h"


bool isPoseEstimationEnabled = true;

// This forces higher priority on your windows system. You can check this in task manager -> details.
static void doExternalOptimizations()
{
	SetThreadPriority(GetCurrentThread(), settings.useRealTimePriority ? REALTIME_PRIORITY_CLASS : HIGH_PRIORITY_CLASS);
	SetPriorityClass(GetCurrentProcess(), settings.useRealTimePriority ? REALTIME_PRIORITY_CLASS : HIGH_PRIORITY_CLASS);
}

// Attempts to initialize the camera, then returns true if cameraCapture was successfully opened.
static bool initCamera(cv::VideoCapture& camera)
{
	camera = cv::VideoCapture(settings.cameraToUse);
	return camera.isOpened();
}

static void setCpuOrGpu(cv::dnn::Net& inputNet)
{
	if (settings.preferredDevice == "cpu")
	{
		LOG(INFO) << "Attempting to use CPU device!" << std::endl;
		inputNet.setPreferableBackend(cv::dnn::DNN_TARGET_CPU);
	}
	else if (settings.preferredDevice == "gpu") // NOTE: Requires custom OpenCV built with CUDA sdk.
	{
		LOG(INFO) << "Attempting to use GPU device!" << std::endl;
		inputNet.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
		inputNet.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
	}
}

// Attempts to load the DNN model, then returns true if it was successful.
bool loadDnnModel(cv::dnn::Net& inputNet)
{
	try
	{
		inputNet = cv::dnn::readNetFromCaffe(static_cast<std::string>(settings.prototxt),
		                                     static_cast<std::string>(settings.caffemodel));
	}
	catch (cv::Exception e)
	{
		return false;
	}

	return true;
}

void displayCurrentPose(const cv::Mat& outputFrame, std::map<std::string, std::vector<KeyPoint>>& map)
{
	const std::string poseString = getPoseString(getPose(map));

	std::string baseString = "Current orientation is: ";

	constexpr int offset = 40;
	constexpr int fontFace = cv::FONT_HERSHEY_COMPLEX;
	constexpr double fontScale = 1.0;
	const cv::Scalar color = {0, 0, 0, 0};

	cv::putText(outputFrame, baseString.append(poseString),
	            {offset, outputFrame.rows - offset},
	            fontFace, fontScale, color);
}

void displayCurrentOrientation(const cv::Mat& outputFrame, std::map<std::string, std::vector<KeyPoint>>& map)
{
	std::string baseString = "Current Pose is: ";

	constexpr int offset = 40;
	constexpr int fontFace = cv::FONT_HERSHEY_COMPLEX;
	constexpr double fontScale = 1.0;
	const cv::Scalar color = {0, 0, 0, 0};

	cv::putText(outputFrame, baseString.append(std::to_string(isStandingTowardsCamera(map))),
	            {offset, offset},
	            fontFace, fontScale, color);
}

// Start the thread for pose estimation. 
int runPoseRetriever()
{
	doExternalOptimizations();

	// Try to initialize the camera. If it fails, we return and never touch anything related to the camera and/or pose estimation for the remainder of this process.
	cv::VideoCapture camera;
	if (!initCamera(camera))
	{
		LOG(INFO) << "Could not initialize camera. Will continue without pose estimation." << std::endl;
		isPoseEstimationEnabled = false;
		return 0;
	}

	// Then we try to load the model. If it fails, we return and never touch anything related to the camera and/or pose estimation for the remainder of this process.
	cv::dnn::Net inputNet;

	if (!loadDnnModel(inputNet))
	{
		LOG(INFO) << "Could not initialize dnn model. Will continue without pose estimation." << std::endl;
		LOG(INFO) << "Did you check whether the models are in right directory? (./pose/coco/...)." << std::endl;
		isPoseEstimationEnabled = false;
		return 0;
	}

	setCpuOrGpu(inputNet);

	// Main loop.
	while (true)
	{
		cv::Mat inputFrame;
		cv::Mat outputFrame;
		camera.read(inputFrame); // Get camera frame and put it into valid matrix.

		std::map<std::string, std::vector<KeyPoint>>& keyPointsToUseInCalculation = getPoseEstimationKeyPointsMap(
			inputFrame, outputFrame, inputNet);

		displayCurrentPose(outputFrame, keyPointsToUseInCalculation);
		displayCurrentOrientation(outputFrame, keyPointsToUseInCalculation);

		cv::imshow(std::string(settings.openCVWindowName), outputFrame);
		cv::waitKey(settings.waitKeyDelayOpenCV);

		clearPoseEstimationKeyPointsMap(); // DON'T FORGET TO CLEAR MAP; THIS LINE IS IMPORTANT!
	}

	return 0;
}
