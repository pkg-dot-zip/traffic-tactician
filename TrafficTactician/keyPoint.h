#pragma once
#include <opencv2/core/types.hpp>

struct KeyPoint
{
	KeyPoint(cv::Point point, float probability)
	{
		this->id = -1;
		this->point = point;
		this->probability = probability;
	}

	int id;
	cv::Point point;
	float probability;
};
