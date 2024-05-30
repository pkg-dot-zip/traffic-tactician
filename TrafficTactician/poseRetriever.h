#pragma once
#include <opencv2/dnn.hpp>

int runPoseRetriever();
void loadDnnModel(cv::dnn::Net& inputNet);