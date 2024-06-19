#pragma once
#include <opencv2/core/types.hpp>

struct KeyPoint
{
	KeyPoint(const cv::Point point, const float probability)
	{
		this->id = -1;
		this->point = point;
		this->probability = probability;
	}

	int id;
	cv::Point point;
	float probability;
};
