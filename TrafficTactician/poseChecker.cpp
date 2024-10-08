/**
 * This file contains all methods to interface with poseEstimation.cpp, whose contents should not be interacted with directly.
 * Instead, use the methods in this file.
 */

#include "poseChecker.h"

#include <map>
#include <string>
#include <vector>
#include <math.h>

#include "easylogging++.h"
#include "keyPoint.h"
#include "KeypointLocationStrings.h"
#include "SettingsRetriever.h"
#include "utest.h"


float calculateDegreesOfElbowToWristLeft(std::map<std::string_view, std::vector<KeyPoint>>& map);
float calculateDegreesOfElbowToWristRight(std::map<std::string_view, std::vector<KeyPoint>>& map);
float calculateDegreesBetweenTwoPoints(std::map<std::string_view, std::vector<KeyPoint>>& map, const std::string_view& point1,
                                       const std::string_view& point2);

PoseDirection getDirectionForArm(float angleInDegrees);
PoseDirection getDirectionForArmLeft(std::map<std::string_view, std::vector<KeyPoint>>& map);
PoseDirection getDirectionForArmRight(std::map<std::string_view, std::vector<KeyPoint>>& map);

std::string getPoseString(Pose pose);

std::string getPoseString(const Pose pose)
{
	switch (pose)
	{
	case POSE_MOVE_LEFT:
		return "POSE_MOVE_LEFT";
	case POSE_MOVE_RIGHT:
		return "POSE_MOVE_RIGHT";
	case POSE_MOVE_FORWARD:
		return "POSE_MOVE_FORWARD";
	case POSE_STOP:
		return "POSE_STOP";
	case POSE_OTHER:
		return "POSE_OTHER";
	default:
		throw "Unhandled enum state for Pose -> Can't convert unhandled pose to string";
	}
}

// Checks if the right wrist is near the right shoulder.
bool isRightWristNearShoulder(std::map<std::string_view, std::vector<KeyPoint>>& map)
{
	if (map[pose_keypoint_wrist_right].empty() || map[pose_keypoint_shoulder_right].empty()) return false;

	constexpr float toleranceWristNearShoulderDistanceX = GetDNNSettings().toleranceWristNearShoulderDistanceX;
	constexpr float toleranceWristNearShoulderDistanceY = GetDNNSettings().toleranceWristNearShoulderDistanceY;

	constexpr float toleranceWristNearShoulderDistanceLeftAndDown = GetDNNSettings().toleranceWristNearShoulderDistanceLeftAndDown;

	const float wristX = map[pose_keypoint_wrist_right][0].point.x;
	const float wristY = map[pose_keypoint_wrist_right][0].point.y;

	const float shoulderX = map[pose_keypoint_shoulder_right][0].point.x;
	const float shoulderY = map[pose_keypoint_shoulder_right][0].point.y;

	const bool isWristXNearShoulderX = wristX <= shoulderX + toleranceWristNearShoulderDistanceLeftAndDown && wristX >= shoulderX -
		toleranceWristNearShoulderDistanceX;
	const bool isWristYNearShoulderY = wristY <= shoulderY + toleranceWristNearShoulderDistanceLeftAndDown && wristY >= shoulderY -
		toleranceWristNearShoulderDistanceY;

	return isWristXNearShoulderX && isWristYNearShoulderY;
}

// Checks if person is standing oriented towards the camera or sideways. Returns true if standing oriented towards the camera.
bool isPersonFacingFront(std::map<std::string_view, std::vector<KeyPoint>>& map)
{
	if (map[pose_keypoint_shoulder_left].empty() || map[pose_keypoint_shoulder_right].empty()) return false;

	constexpr float personOrientationShoulderTolerance = GetDNNSettings().personOrientationShoulderTolerance;

	const float LshoulderX = map[pose_keypoint_shoulder_left][0].point.x;
	const float RshoulderX = map[pose_keypoint_shoulder_right][0].point.x;

	return !(RshoulderX < LshoulderX + personOrientationShoulderTolerance && RshoulderX > LshoulderX -
		personOrientationShoulderTolerance);
}

Pose getPose(std::map<std::string_view, std::vector<KeyPoint>>& map)
{
	const PoseDirection leftArmDirection = getDirectionForArmLeft(map);
	const PoseDirection rightArmDirection = getDirectionForArmRight(map);

	const float heightDifferenceElbowLeft = calculateDifferenceInHeightBetweenShoulderAndWristLeft(map);
	const float heightDifferenceElbowRight = calculateDifferenceInHeightBetweenShoulderAndWristRight(map);

	constexpr float heightDifferenceBetweenShoulderAndWristTolerance = GetDNNSettings().heightDifferenceBetweenShoulderAndWristTolerance;

	if (leftArmDirection == DIRECTION_LEFT && (rightArmDirection == DIRECTION_DOWN || rightArmDirection ==
			DIRECTION_UNCLEAR) && heightDifferenceElbowLeft >= 0 - heightDifferenceBetweenShoulderAndWristTolerance && heightDifferenceElbowLeft <=
		heightDifferenceBetweenShoulderAndWristTolerance)
	{
		return POSE_MOVE_LEFT;
	}

	if (rightArmDirection == DIRECTION_RIGHT && (leftArmDirection == DIRECTION_DOWN || leftArmDirection ==
			DIRECTION_UNCLEAR) && heightDifferenceElbowRight >= 0 - heightDifferenceBetweenShoulderAndWristTolerance && heightDifferenceElbowRight
		<=
		heightDifferenceBetweenShoulderAndWristTolerance)
	{
		return POSE_MOVE_RIGHT;
	}

	if (isRightWristNearShoulder(map) && (leftArmDirection == DIRECTION_DOWN || leftArmDirection ==
		DIRECTION_UNCLEAR))
	{
		return POSE_STOP;
	}

	if (!isPersonFacingFront(map) && (leftArmDirection == DIRECTION_DOWN || leftArmDirection ==
		DIRECTION_UNCLEAR) && (rightArmDirection == DIRECTION_DOWN || rightArmDirection ==
		DIRECTION_UNCLEAR))
	{
		return POSE_MOVE_FORWARD;
	}

	return POSE_OTHER;
}

std::string getDirectionString(const PoseDirection pose_direction)
{
	switch (pose_direction)
	{
	case DIRECTION_LEFT:
		return "LEFT";
	case DIRECTION_UP:
		return "UP";
	case DIRECTION_RIGHT:
		return "RIGHT";
	case DIRECTION_DOWN:
		return "DOWN";
	default:
		return "UNCLEAR";
	}
}

// Method used for debugging prints for direction of arms.
void printDirectionOfArms(std::map<std::string_view, std::vector<KeyPoint>>& map)
{
	const int leftArmDirection = getDirectionForArmLeft(map);
	const int rightArmDirection = getDirectionForArmRight(map);

	switch (leftArmDirection)
	{
	case DIRECTION_LEFT:
		LOG(INFO) << "Left arm direction is LEFT!" << std::endl;
		break;
	case DIRECTION_UP:
		LOG(INFO) << "Left arm direction is UP!" << std::endl;
		break;
	case DIRECTION_RIGHT:
		LOG(INFO) << "Left arm direction is RIGHT!" << std::endl;
		break;
	case DIRECTION_DOWN:
		LOG(INFO) << "Left arm direction is DOWN!" << std::endl;
		break;
	default:
		LOG(INFO) << "Left arm direction is UNCLEAR!" << std::endl;
		break;
	}

	switch (rightArmDirection)
	{
	case DIRECTION_LEFT:
		LOG(INFO) << "Right arm direction is LEFT!" << std::endl;
		break;
	case DIRECTION_UP:
		LOG(INFO) << "Right arm direction is UP!" << std::endl;
		break;
	case DIRECTION_RIGHT:
		LOG(INFO) << "Right arm direction is RIGHT!" << std::endl;
		break;
	case DIRECTION_DOWN:
		LOG(INFO) << "Right arm direction is DOWN!" << std::endl;
		break;
	default:
		LOG(INFO) << "Right arm direction is UNCLEAR!" << std::endl;
		break;
	}
}

float calculateDifferenceInHeightBetweenShoulderAndWristLeft(std::map<std::string_view, std::vector<KeyPoint>>& map)
{
	return calculateDifferenceInHeightBetweenTwoPoints(map, pose_keypoint_shoulder_left, pose_keypoint_wrist_left);
}

float calculateDifferenceInHeightBetweenShoulderAndWristRight(std::map<std::string_view, std::vector<KeyPoint>>& map)
{
	return calculateDifferenceInHeightBetweenTwoPoints(map, pose_keypoint_shoulder_right, pose_keypoint_wrist_right);
}

// Returns the difference in height between two points. Returns NAN if any of the two points couldn't be found.
float calculateDifferenceInHeightBetweenTwoPoints(std::map<std::string_view, std::vector<KeyPoint>>& map,
                                                  const std::string_view& point1,
                                                  const std::string_view& point2)
{
	if (map[point1].empty() || map[point2].empty()) return NAN;
	return map[point1][0].point.y - map[point2][0].point.y;
}

// Returns the angle between two points in degrees (NOT RADIANS!). Returns NAN if any of the two points couldn't be found.
float calculateDegreesBetweenTwoPoints(std::map<std::string_view, std::vector<KeyPoint>>& map, const std::string_view& point1,
                                       const std::string_view& point2)
{
	if (map[point1].empty() || map[point2].empty()) return NAN;

	const int point1X = map[point1][0].point.x;
	const int point1Y = map[point1][0].point.y;

	const int point2X = map[point2][0].point.x;
	const int point2Y = map[point2][0].point.y;

	const float angle = atan2(point1Y - point2Y, point1X - point2X);

	constexpr float PI = 3.14159265358979323846;
	const float degrees = angle * 180 / PI;

#ifdef _POSE_DEBUG
	LOG(INFO) << "Degrees between " << point1 << " & " << point2 << ":\t" << degrees << std::endl;
#endif

	return degrees;
}

PoseDirection getDirectionForArmLeft(std::map<std::string_view, std::vector<KeyPoint>>& map)
{
	return getDirectionForArm(calculateDegreesOfElbowToWristLeft(map));
}

PoseDirection getDirectionForArmRight(std::map<std::string_view, std::vector<KeyPoint>>& map)
{
	return getDirectionForArm(calculateDegreesOfElbowToWristRight(map));
}

PoseDirection getDirectionForArm(const float angleInDegrees)
{
	if (angleInDegrees == NAN) return DIRECTION_UNCLEAR;

	constexpr float anglesForDirectionCalculationTolerance = GetDNNSettings().anglesForDirectionCalculationTolerance; // Tolerance in degrees.

	if (angleInDegrees < (90.0F + anglesForDirectionCalculationTolerance) && angleInDegrees > (90.0F - anglesForDirectionCalculationTolerance))
	{
		return DIRECTION_UP;
	}
	if (angleInDegrees < (0.0F + anglesForDirectionCalculationTolerance) && angleInDegrees > (0.0F - anglesForDirectionCalculationTolerance))
	{
		return DIRECTION_RIGHT;
	}
	if (angleInDegrees < (-90.0F + anglesForDirectionCalculationTolerance) && angleInDegrees > (-90.0F - anglesForDirectionCalculationTolerance))
	{
		return DIRECTION_DOWN;
	}
	if (angleInDegrees > (180.0F - anglesForDirectionCalculationTolerance) && angleInDegrees > (-180.0F - anglesForDirectionCalculationTolerance))
	{
		return DIRECTION_LEFT;
	}

	return DIRECTION_UNCLEAR;
}

float calculateDegreesOfElbowToWristLeft(std::map<std::string_view, std::vector<KeyPoint>>& map)
{
	return calculateDegreesBetweenTwoPoints(map, pose_keypoint_elbow_left, pose_keypoint_wrist_left);
}

float calculateDegreesOfElbowToWristRight(std::map<std::string_view, std::vector<KeyPoint>>& map)
{
	return calculateDegreesBetweenTwoPoints(map, pose_keypoint_elbow_right, pose_keypoint_wrist_right);
}
