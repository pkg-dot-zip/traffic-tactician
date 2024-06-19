#pragma once
#include <opencv2/dnn.hpp>

int runPoseRetriever(int cameraDevice);
bool loadDnnModel(cv::dnn::Net& inputNet);
