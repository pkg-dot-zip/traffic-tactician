#pragma once
#include <string>
#include <vector>
#include <map>
#include "keyPoint.h"

enum PoseDirection { DIRECTION_UNCLEAR, DIRECTION_UP, DIRECTION_RIGHT, DIRECTION_DOWN, DIRECTION_LEFT };

enum Pose { POSE_MOVE_RIGHT, POSE_MOVE_LEFT, POSE_MOVE_FORWARD, POSE_STOP, POSE_OTHER };

void checkPoseForAll(std::map<std::string, std::vector<KeyPoint>>& map);
std::string getDirectionString(PoseDirection pose_direction);
PoseDirection getDirectionForArmLeft(std::map<std::string, std::vector<KeyPoint>>& map);
PoseDirection getDirectionForArmRight(std::map<std::string, std::vector<KeyPoint>>& map);

float calculateDifferenceInHeightBetweenShoulderAndWristLeft(std::map<std::string, std::vector<KeyPoint>>& map);
float calculateDifferenceInHeightBetweenShoulderAndWristRight(std::map<std::string, std::vector<KeyPoint>>& map);
float calculateDifferenceInHeightBetweenTwoPoints(std::map<std::string, std::vector<KeyPoint>>& map, const std::string& point1,
	const std::string& point2);

std::string getPoseString(Pose pose);
Pose getPose(std::map<std::string, std::vector<KeyPoint>>& map);

bool isPersonFacingFront(std::map<std::string, std::vector<KeyPoint>>& map);