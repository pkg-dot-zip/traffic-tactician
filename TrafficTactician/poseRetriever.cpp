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

constexpr bool useRealTimePriority = false;

static void doExternalOptimizations()
{
	SetThreadPriority(GetCurrentThread(), useRealTimePriority ? REALTIME_PRIORITY_CLASS : HIGH_PRIORITY_CLASS);
	SetPriorityClass(GetCurrentProcess(), useRealTimePriority ? REALTIME_PRIORITY_CLASS : HIGH_PRIORITY_CLASS);
}

constexpr int cameraToUse = 0;
auto camera = cv::VideoCapture(cameraToUse);

static void updateFromCamera(cv::Mat& input)
{
	camera.read(input);
}

PoseDirection leftArmDirection = DIRECTION_UNCLEAR;
PoseDirection rightArmDirection = DIRECTION_UNCLEAR;
std::map<std::string, std::vector<KeyPoint>> keyPointsToUseInCalculation;

static void setCpuOrGpu(std::string& device, cv::dnn::Net& inputNet)
{
	if (device == "cpu")
	{
		LOG(INFO) << "Using CPU device" << std::endl;
		inputNet.setPreferableBackend(cv::dnn::DNN_TARGET_CPU);
	}
	else if (device == "gpu")
	{
		LOG(INFO) << "Using GPU device" << std::endl;
		inputNet.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
		inputNet.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
	}
}

// TODO: Look into usage, and probably remove, since we will always run on our CPU due to our OpenCV build.
void processCommandLineArguments(int& argc, char**& argv, std::string& device)
{
	LOG(INFO) << "USAGE : ./multi-person-openpose <inputFile> <device>" << std::endl;
	if (argc == 2)
	{
		if (static_cast<std::string>(argv[1]) == "gpu")
			device = "gpu";
	}
	else if (argc == 3)
	{
		if (static_cast<std::string>(argv[2]) == "gpu")
			device = "gpu";
	}
}

static void loadDnnModel(cv::dnn::Net& inputNet)
{
	const std::string prototxt = "./pose/coco/pose_deploy_linevec.prototxt";
	const std::string caffemodel = "./pose/coco/pose_iter_440000.caffemodel";
	inputNet = cv::dnn::readNetFromCaffe(prototxt, caffemodel);
}

void displayArmDirections(cv::Mat& outputFrame)
{
	std::string leftArmDirectionString = getDirectionString(leftArmDirection);
	std::string rightArmDirectionString = getDirectionString(rightArmDirection);

	std::string leftArmDirectionDisplayString = "Left arm Direction is ";
	std::string rightArmDirectionDisplayString = "Right arm Direction is ";

	leftArmDirectionDisplayString.append(leftArmDirectionString);
	rightArmDirectionDisplayString.append(rightArmDirectionString);

	cv::putText(outputFrame, leftArmDirectionDisplayString, { 40, outputFrame.rows - 40 }, cv::FONT_HERSHEY_COMPLEX,
		1.0, { 0, 0, 0, 0 });
	cv::putText(outputFrame, rightArmDirectionDisplayString, { 40, 40 }, cv::FONT_HERSHEY_COMPLEX, 1.0, { 0, 0, 0, 0 });
}

int run(int argc, char** argv)
{
	doExternalOptimizations();

	std::string device = "gpu";
	processCommandLineArguments(argc, argv, device);

	cv::Mat input;
	updateFromCamera(input);

	cv::dnn::Net inputNet;
	loadDnnModel(inputNet);
	setCpuOrGpu(device, inputNet);

	constexpr double upscaleFactor = 4;
	constexpr double downscaleFactor = 0.4;

	while (true)
	{
		updateFromCamera(input);
		cv::resize(input, input, cv::Size(), downscaleFactor, downscaleFactor, cv::INTER_AREA);
		LOG(INFO) << "Width: " << input.rows << " | Height: " << input.cols << std::endl;
		cv::Mat outputFrame;

		const auto timeStart = cv::getTickCount();
		getCalculatedPose(keyPointsToUseInCalculation, input, outputFrame, inputNet);
		const auto timeEnd = cv::getTickCount();

		auto time = (timeEnd - timeStart) / cv::getTickFrequency();

		LOG(INFO) << "Time it took for: " << time << std::endl;

		leftArmDirection = getDirectionForArmLeft(keyPointsToUseInCalculation);
		rightArmDirection = getDirectionForArmRight(keyPointsToUseInCalculation);

		cv::resize(outputFrame, outputFrame, cv::Size(), upscaleFactor, upscaleFactor, cv::INTER_LINEAR);
		cv::flip(outputFrame, outputFrame, 1);

		displayArmDirections(outputFrame);

		cv::imshow("Detected Pose", outputFrame);
		cv::waitKey(1);

		keyPointsToUseInCalculation.clear(); // DON'T FORGET TO CLEAR MAP; THIS LINE IS IMPORTANT!

		LOG(INFO) << "Was running at " << GetPriorityClass(GetCurrentProcess()) << std::endl;
	}

	return 0;
}
