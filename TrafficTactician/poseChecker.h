
#pragma once
#include <string>
#include <vector>
#include <map>
#include "keyPoint.h"

enum PoseDirection { DIRECTION_UNCLEAR, DIRECTION_UP, DIRECTION_RIGHT, DIRECTION_DOWN, DIRECTION_LEFT };


void checkPoseForAll(std::map<std::string, std::vector<KeyPoint>>& map);
PoseDirection getDirectionForArmLeft(std::map<std::string, std::vector<KeyPoint>>& map);
PoseDirection getDirectionForArmRight(std::map<std::string, std::vector<KeyPoint>>& map);