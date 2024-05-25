// Following includes and configurations are just to make it work and are not actually used in the pose detection configuration.
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")
#include <GLFW/glfw3.h>

#include "poseEstimation.h"

GLFWwindow* window;
// END

#ifdef _DEBUG
#pragma comment (lib, "opencv_world490d.lib")
#else
#pragma comment (lib, "opencv_world490.lib")
#endif

#include<opencv2/dnn.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include "easylogging++.h"

#include<iostream>
#include<chrono>
#include<random>
#include<set>
#include<cmath>

#include "keyPoint.h"
#include "poseChecker.h"


INITIALIZE_EASYLOGGINGPP

constexpr bool useRealTimePriority = false;

void doExternalOptimizations()
{
	SetThreadPriority(GetCurrentThread(), useRealTimePriority ? REALTIME_PRIORITY_CLASS : HIGH_PRIORITY_CLASS);
	SetPriorityClass(GetCurrentProcess(), useRealTimePriority ? REALTIME_PRIORITY_CLASS : HIGH_PRIORITY_CLASS);
}

auto camera = cv::VideoCapture(0);

PoseDirection leftArmDirection = DIRECTION_UNCLEAR;
PoseDirection rightArmDirection = DIRECTION_UNCLEAR;

void updateFromCamera(cv::Mat& input)
{
	camera.read(input);
}

std::map<std::string, std::vector<KeyPoint>> keyPointsToUseInCalculation;

int main(int argc, char** argv)
{
	doExternalOptimizations();

	std::string inputFile = "./group.jpg";
	// std::string inputFile = "./tpose.jpg";
	std::string device = "gpu";
	std::cout << "USAGE : ./multi-person-openpose <inputFile> <device>" << std::endl;
	if (argc == 2)
	{
		if (static_cast<std::string>(argv[1]) == "gpu")
			device = "gpu";
		else
			inputFile = argv[1];
	}
	else if (argc == 3)
	{
		inputFile = argv[1];
		if (static_cast<std::string>(argv[2]) == "gpu")
			device = "gpu";
	}

	cv::Mat input;
	// cv::Mat input = cv::imread(inputFile, cv::IMREAD_COLOR);
	// cv::resize(input, input, cv::Size(), 0.1, 0.1, cv::INTER_LINEAR);



	updateFromCamera(input);

	cv::dnn::Net inputNet = cv::dnn::readNetFromCaffe("./pose/coco/pose_deploy_linevec.prototxt",
		"./pose/coco/pose_iter_440000.caffemodel");

	if (device == "cpu")
	{
		std::cout << "Using CPU device" << std::endl;
		inputNet.setPreferableBackend(cv::dnn::DNN_TARGET_CPU);
	}
	else if (device == "gpu")
	{
		std::cout << "Using GPU device" << std::endl;
		inputNet.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
		inputNet.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
	}

	const bool isOptimized = cv::useOptimized();
	LOG(INFO) << "Is optimized: " << isOptimized;

	constexpr double upscaleFactor = 4;
	constexpr double downscaleFactor = 0.4;

	

	while (true)
	{
		updateFromCamera(input);
		cv::resize(input, input, cv::Size(), downscaleFactor, downscaleFactor, cv::INTER_AREA);
		LOG(INFO) << "Width: " << input.rows << " | Height: " << input.cols << std::endl;
		cv::Mat outputFrame;

		auto timeStart = cv::getTickCount();
		getCalculatedPose(keyPointsToUseInCalculation, input, outputFrame, inputNet);

		leftArmDirection = getDirectionForArmLeft(keyPointsToUseInCalculation);
		rightArmDirection = getDirectionForArmRight(keyPointsToUseInCalculation);

		auto timeEnd = cv::getTickCount();

		auto time = (timeEnd - timeStart) / cv::getTickFrequency();

		LOG(INFO) << "Time it took for: " << time << std::endl;


		cv::resize(outputFrame, outputFrame, cv::Size(), upscaleFactor, upscaleFactor, cv::INTER_LINEAR);
		cv::flip(outputFrame, outputFrame, 1);

		std::string a = getDirectionString(leftArmDirection);
		std::string b = getDirectionString(rightArmDirection);

		std::string string1 = "Left arm Direction is ";
		std::string string2 = "Right arm Direction is ";

		string1.append(a);
		string2.append(b);

		const std::string leftArmDirectionString = string1;
		const std::string rightArmDirectionString = string2;
		cv::putText(outputFrame, leftArmDirectionString, { 40, outputFrame.rows - 40 }, cv::FONT_HERSHEY_COMPLEX, 1.0, { 0, 0, 0, 0 });
		cv::putText(outputFrame, rightArmDirectionString, { 40, 40 }, cv::FONT_HERSHEY_COMPLEX, 1.0, { 0, 0, 0, 0 });

		cv::imshow("Detected Pose", outputFrame);
		cv::waitKey(1);

		keyPointsToUseInCalculation.clear();

		LOG(INFO) << "Was running at " << GetPriorityClass(GetCurrentProcess()) << std::endl;
	}

	return 0;
}
