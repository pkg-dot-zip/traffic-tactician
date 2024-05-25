#pragma once
#include <map>
#include <opencv2/core/mat.hpp>
#include <opencv2/dnn.hpp>

#include "keyPoint.h"

void getCalculatedPose(std::map<std::string, std::vector<KeyPoint>>& keyPointsToUseInCalculation, cv::Mat& input, cv::Mat& outputFrame, cv::dnn::Net& inputNet);
