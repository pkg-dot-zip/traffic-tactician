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
#include "settingsFromJson.h"


float calculateDegreesOfElbowToWristLeft(std::map<std::string, std::vector<KeyPoint>>& map);
float calculateDegreesOfElbowToWristRight(std::map<std::string, std::vector<KeyPoint>>& map);
float calculateDegreesBetweenTwoPoints(std::map<std::string, std::vector<KeyPoint>>& map, const std::string& point1,
                                       const std::string& point2);

PoseDirection getDirectionForArm(float angleInDegrees);
PoseDirection getDirectionForArmLeft(std::map<std::string, std::vector<KeyPoint>>& map);
PoseDirection getDirectionForArmRight(std::map<std::string, std::vector<KeyPoint>>& map);


void printDirectionOfArms(std::map<std::string, std::vector<KeyPoint>>& map);

std::string getPoseString(const Pose pose);

// Method used for debugging prints.
void checkPoseForAll(std::map<std::string, std::vector<KeyPoint>>& map)
{
	calculateDegreesOfElbowToWristLeft(map);
	calculateDegreesOfElbowToWristRight(map);

	printDirectionOfArms(map);

	const Pose currentPose = getPose(map);
	LOG(INFO) << "Current pose: " << getPoseString(currentPose) << std::endl;
}

std::string getPoseString(const Pose pose)
{
	if (pose == POSE_MOVE_LEFT)
	{
		return "POSE_MOVE_LEFT";
	}
	else if (pose == POSE_MOVE_RIGHT)
	{
		return "POSE_MOVE_RIGHT";
	}
	else if (pose == POSE_MOVE_FORWARD)
	{
		return "POSE_MOVE_FORWARD";
	}
	else if (pose == POSE_STOP)
	{
		return "POSE_STOP";
	}
	else if (pose == POSE_OTHER)
	{
		return "POSE_OTHER";
	}

	throw "Can't convert invalid pose enum value to string!";
}

Pose getPose(std::map<std::string, std::vector<KeyPoint>>& map)
{
	const PoseDirection leftArmDirection = getDirectionForArmLeft(map);
	const PoseDirection rightArmDirection = getDirectionForArmRight(map);

	const float heightDifferenceLeft = calculateDifferenceInHeightBetweenElbowAndWristLeft(map);
	const float heightDifferenceRight = calculateDifferenceInHeightBetweenElbowAndWristRight(map);

	constexpr float heightTolerance = 40.0F;

	LOG(INFO) << "Height dif left " << heightDifferenceLeft << std::endl;
	LOG(INFO) << "Height dif right " << heightDifferenceRight << std::endl;

	if (leftArmDirection == DIRECTION_LEFT && (rightArmDirection == DIRECTION_DOWN || rightArmDirection ==
		DIRECTION_UNCLEAR) && heightDifferenceLeft >= 0 - heightTolerance && heightDifferenceRight <= heightTolerance)
	{
		return POSE_MOVE_LEFT;
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
void printDirectionOfArms(std::map<std::string, std::vector<KeyPoint>>& map)
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

float calculateDifferenceInHeightBetweenElbowAndWristLeft(std::map<std::string, std::vector<KeyPoint>>& map)
{
	return calculateDifferenceInHeightBetweenTwoPoints(map, "L-Sho", "L-Wr");
}

float calculateDifferenceInHeightBetweenElbowAndWristRight(std::map<std::string, std::vector<KeyPoint>>& map)
{
	return calculateDifferenceInHeightBetweenTwoPoints(map, "R-Sho", "R-Wr");
}

// Returns the difference in height between two points. Returns NAN if the two points couldn't be found.
float calculateDifferenceInHeightBetweenTwoPoints(std::map<std::string, std::vector<KeyPoint>>& map,
                                                  const std::string& point1,
                                                  const std::string& point2)
{
	if (map[point1].empty() || map[point2].empty()) return NAN;
	return map[point1][0].point.y - map[point2][0].point.y;
}

// Returns the angle between two points in degrees (NOT RADIANS!). Returns NAN if the two points couldn't be found.
float calculateDegreesBetweenTwoPoints(std::map<std::string, std::vector<KeyPoint>>& map, const std::string& point1,
                                       const std::string& point2)
{
	if (map[point1].empty() || map[point2].empty()) return NAN;

	const int point1X = map[point1][0].point.x;
	const int point1Y = map[point1][0].point.y;

	const int point2X = map[point2][0].point.x;
	const int point2Y = map[point2][0].point.y;

	LOG(INFO) << point1 << " " << point1X << "/" << point1Y << " ||| " << point2 << " " << point2X << "/" << point2Y <<
		std::endl;

	const float angle = atan2(point1Y - point2Y, point1X - point2X);

	constexpr float PI = 3.14159265358979323846;
	const float degrees = angle * 180 / PI;

	LOG(INFO) << "Degrees between " << point1 << " & " << point2 << ":\t" << degrees << std::endl;

	return degrees;
}

PoseDirection getDirectionForArmLeft(std::map<std::string, std::vector<KeyPoint>>& map)
{
	return getDirectionForArm(calculateDegreesOfElbowToWristLeft(map));
}

PoseDirection getDirectionForArmRight(std::map<std::string, std::vector<KeyPoint>>& map)
{
	return getDirectionForArm(calculateDegreesOfElbowToWristRight(map));
}

PoseDirection getDirectionForArm(const float angleInDegrees)
{
	constexpr float tolerance = settings.tolerance; // Tolerance in degrees.

	if (angleInDegrees < (90.0F + tolerance) && angleInDegrees > (90.0F - tolerance))
	{
		return DIRECTION_UP;
	}
	else if (angleInDegrees < (0.0F + tolerance) && angleInDegrees > (0.0F - tolerance))
	{
		return DIRECTION_RIGHT;
	}
	else if (angleInDegrees < (-90.0F + tolerance) && angleInDegrees > (-90.0F - tolerance))
	{
		return DIRECTION_DOWN;
	}
	else if (angleInDegrees > (180.0F - tolerance) && angleInDegrees > (-180.0F - tolerance))
	{
		return DIRECTION_LEFT;
	}

	return DIRECTION_UNCLEAR;
}

float calculateDegreesOfElbowToWristLeft(std::map<std::string, std::vector<KeyPoint>>& map)
{
	return calculateDegreesBetweenTwoPoints(map, "L-Elb", "L-Wr");
}

float calculateDegreesOfElbowToWristRight(std::map<std::string, std::vector<KeyPoint>>& map)
{
	return calculateDegreesBetweenTwoPoints(map, "R-Elb", "R-Wr");
}
