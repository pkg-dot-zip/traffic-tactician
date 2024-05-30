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

constexpr bool useRealTimePriority = settings.useRealTimePriority;

// This forces higher priority on your windows system. You can check this in task manager -> details.
static void doExternalOptimizations()
{
	SetThreadPriority(GetCurrentThread(), useRealTimePriority ? REALTIME_PRIORITY_CLASS : HIGH_PRIORITY_CLASS);
	SetPriorityClass(GetCurrentProcess(), useRealTimePriority ? REALTIME_PRIORITY_CLASS : HIGH_PRIORITY_CLASS);
}

constexpr int cameraToUse = settings.cameraToUse;
auto camera = cv::VideoCapture(cameraToUse);

static void updateFromCamera(cv::Mat& input)
{
	camera.read(input);
}

PoseDirection leftArmDirection = DIRECTION_UNCLEAR;
PoseDirection rightArmDirection = DIRECTION_UNCLEAR;

constexpr std::string_view preferred_device = settings.preferredDevice;

static void setCpuOrGpu(cv::dnn::Net& inputNet)
{
	if (preferred_device == "cpu")
	{
		LOG(INFO) << "Attempting to use CPU device!" << std::endl;
		inputNet.setPreferableBackend(cv::dnn::DNN_TARGET_CPU);
	}
	else if (preferred_device == "gpu") // NOTE: Required custom OpenCV built with CUDA sdk.
	{
		LOG(INFO) << "Attempting to use GPU device!" << std::endl;
		inputNet.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
		inputNet.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
	}
}

void loadDnnModel(cv::dnn::Net& inputNet)
{
	inputNet = cv::dnn::readNetFromCaffe(static_cast<std::string>(settings.prototxt), static_cast<std::string>(settings.caffemodel));
}

static void displayArmDirections(cv::Mat& outputFrame)
{
	const std::string leftArmDirectionString = getDirectionString(leftArmDirection);
	const std::string rightArmDirectionString = getDirectionString(rightArmDirection);

	std::string leftArmDirectionDisplayString = "Left arm Direction is ";
	std::string rightArmDirectionDisplayString = "Right arm Direction is ";

	constexpr int offset = 40;
	constexpr int fontFace = cv::FONT_HERSHEY_COMPLEX;
	constexpr double fontScale = 1.0;
	const cv::Scalar color = {0, 0, 0, 0};

	cv::putText(outputFrame, leftArmDirectionDisplayString.append(leftArmDirectionString),
	            {offset, outputFrame.rows - offset},
	            fontFace, fontScale, color);
	cv::putText(outputFrame, rightArmDirectionDisplayString.append(rightArmDirectionString), {offset, offset},
	            fontFace, fontScale, color);
}

// Decides whether to stop the thread or continue working.
static bool shouldRun()
{
	return true;
}

void displayCurrentPose(const cv::Mat& outputFrame, std::map<std::string, std::vector<KeyPoint>>& map)
{
	const std::string poseString = getPoseString(getPose(map));

	std::string baseString = "Current orientation is: ";

	constexpr int offset = 40;
	constexpr int fontFace = cv::FONT_HERSHEY_COMPLEX;
	constexpr double fontScale = 1.0;
	const cv::Scalar color = { 0, 0, 0, 0 };

	cv::putText(outputFrame, baseString.append(poseString),
		{ offset, outputFrame.rows - offset },
		fontFace, fontScale, color);
}

void displayCurrentOrientation(const cv::Mat& outputFrame, std::map<std::string, std::vector<KeyPoint>>& map)
{
	std::string baseString = "Current Pose is: ";

	constexpr int offset = 40;
	constexpr int fontFace = cv::FONT_HERSHEY_COMPLEX;
	constexpr double fontScale = 1.0;
	const cv::Scalar color = { 0, 0, 0, 0 };

	cv::putText(outputFrame, baseString.append(std::to_string(isStandingTowardsCamera(map))),
		{ offset, offset },
		fontFace, fontScale, color);
}

// Start the thread for pose estimation. 
int runPoseRetriever()
{
	doExternalOptimizations();

	cv::Mat input;
	cv::dnn::Net inputNet;
	loadDnnModel(inputNet);
	setCpuOrGpu(inputNet);

	while (shouldRun())
	{
		updateFromCamera(input);
		cv::Mat outputFrame;

		std::map<std::string, std::vector<KeyPoint>>& keyPointsToUseInCalculation = getPoseEstimationKeyPointsMap(
			input, outputFrame, inputNet);

		leftArmDirection = getDirectionForArmLeft(keyPointsToUseInCalculation);
		rightArmDirection = getDirectionForArmRight(keyPointsToUseInCalculation);

		// displayArmDirections(outputFrame);
		displayCurrentPose(outputFrame, keyPointsToUseInCalculation);
		displayCurrentOrientation(outputFrame, keyPointsToUseInCalculation);

		cv::imshow(std::string(settings.openCVWindowName), outputFrame);
		cv::waitKey(settings.waitKeyDelayOpenCV);

		clearPoseEstimationKeyPointsMap(); // DON'T FORGET TO CLEAR MAP; THIS LINE IS IMPORTANT!
	}

	return 0;
}
