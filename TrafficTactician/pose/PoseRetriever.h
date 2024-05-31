#pragma once
#include <opencv2/dnn.hpp>

int runPoseRetriever();
bool loadDnnModel(cv::dnn::Net& inputNet);