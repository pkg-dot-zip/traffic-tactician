#pragma once
#include <map>
#include <opencv2/dnn.hpp>

#include "keyPoint.h"

std::map<std::string, std::vector<KeyPoint>>& getPoseEstimationKeyPointsMap(cv::Mat& input,
	cv::Mat& outputFrame,
	cv::dnn::Net& inputNet);

void clearPoseEstimationKeyPointsMap();
