/*
 * Contains unit tests for retrieving poses from images.
 */
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

#include "poseChecker.h"
#include "poseRetriever.h"
#ifdef _TESTING_CONFIG

#include "poseEstimation.h"
#include "utest.h"
#include <opencv2/dnn.hpp>

#pragma region helpers
extern std::map<std::string_view, std::vector<KeyPoint>> poseEstimationKeyPoints;

std::map<std::string_view, std::vector<KeyPoint>>& getPoints(std::string_view filePath)
{
	poseEstimationKeyPoints.clear();

	cv::Mat input1 = cv::imread(cv::String(filePath));
	cv::dnn::Net inputNet;
	cv::Mat outputFrame;

	if (inputNet.empty()) loadDnnModel(inputNet);

	poseEstimationKeyPoints = getPoseEstimationKeyPointsMap(
		input1, outputFrame, inputNet);

	return poseEstimationKeyPoints;
}
#pragma endregion helpers

#pragma region returnsMoveLeft
UTEST(getPose, returnsMoveLeft1)
{
	ASSERT_EQ(getPose(getPoints("./images/unittesting/move_left1.jpg")), POSE_MOVE_LEFT);
}

UTEST(getPose, returnsMoveLeft2)
{
	ASSERT_EQ(getPose(getPoints("./images/unittesting/move_left2.jpg")), POSE_MOVE_LEFT);
}

UTEST(getPose, returnsMoveLeft3)
{
	ASSERT_EQ(getPose(getPoints("./images/unittesting/move_left3.jpg")), POSE_MOVE_LEFT);
}
#pragma endregion returnsMoveLeft

#pragma region NOTreturnsMoveLeft
UTEST(getPose, NOTreturnsMoveLeft1)
{
	ASSERT_NE(getPose(getPoints("./images/unittesting/not_move_left1.jpg")), POSE_MOVE_LEFT);
}

UTEST(getPose, NOTreturnsMoveLeft2)
{
	ASSERT_NE(getPose(getPoints("./images/unittesting/not_move_left2.jpg")), POSE_MOVE_LEFT);
}

UTEST(getPose, NOTreturnsMoveLeft3)
{
	ASSERT_NE(getPose(getPoints("./images/unittesting/not_move_left3.jpg")), POSE_MOVE_LEFT);
}
#pragma endregion NOTreturnsMoveLeft

#pragma region returnsMoveRight
UTEST(getPose, returnsMoveRight1)
{
	ASSERT_EQ(getPose(getPoints("./images/unittesting/move_right1.jpg")), POSE_MOVE_RIGHT);
}

UTEST(getPose, returnsMoveRight2)
{
	ASSERT_EQ(getPose(getPoints("./images/unittesting/move_right2.jpg")), POSE_MOVE_RIGHT);
}

UTEST(getPose, returnsMoveRight3)
{
	ASSERT_EQ(getPose(getPoints("./images/unittesting/move_right3.jpg")), POSE_MOVE_RIGHT);
}
#pragma endregion returnsMoveRight

#pragma region NOTreturnsMoveRight
UTEST(getPose, NOTreturnsMoveRight1)
{
	ASSERT_NE(getPose(getPoints("./images/unittesting/not_move_right1.jpg")), POSE_MOVE_RIGHT);
}

UTEST(getPose, NOTreturnsMoveRight2)
{
	ASSERT_NE(getPose(getPoints("./images/unittesting/not_move_right2.jpg")), POSE_MOVE_RIGHT);
}
#pragma endregion NOTreturnsMoveRight

#endif
