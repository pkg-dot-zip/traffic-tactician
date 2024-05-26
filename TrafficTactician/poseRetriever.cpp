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
	else if (preferred_device == "gpu")
	{
		LOG(INFO) << "Attempting to use GPU device!" << std::endl;
		inputNet.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
		inputNet.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
	}
}

static void loadDnnModel(cv::dnn::Net& inputNet)
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

constexpr double upscaleFactor = settings.upscaleFactor;
constexpr double downscaleFactor = settings.downscaleFactor;

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
		cv::resize(input, input, cv::Size(), downscaleFactor, downscaleFactor, cv::INTER_AREA);
		// INTER_AREA is better than the default (INTER_LINEAR) for camera views, according to a Stackoverflow user. TODO: CHECK IF THIS IS TRUE.
		LOG(INFO) << "AFTER DOWNSCALING - Width: " << input.rows << " | Height: " << input.cols << std::endl;
		cv::Mat outputFrame;

		std::map<std::string, std::vector<KeyPoint>>& keyPointsToUseInCalculation = getPoseEstimationKeyPointsMap(
			input, outputFrame, inputNet);


		leftArmDirection = getDirectionForArmLeft(keyPointsToUseInCalculation);
		rightArmDirection = getDirectionForArmRight(keyPointsToUseInCalculation);

		cv::resize(outputFrame, outputFrame, cv::Size(), upscaleFactor, upscaleFactor);
		cv::flip(outputFrame, outputFrame, 1);
		// We flip the mat here so that our cam view looks more natural; it confuses the user to see his left arm on the right side of his screen.

		displayArmDirections(outputFrame);

		cv::imshow("Detected Pose", outputFrame);
		cv::waitKey(1);

		clearPoseEstimationKeyPointsMap(); // DON'T FORGET TO CLEAR MAP; THIS LINE IS IMPORTANT!
	}

	return 0;
}
