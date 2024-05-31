/*
 * Contains the main entry point for the "PoseDetectionRelease" configuration.
 * DO NOT put code here that belongs in another configuration.
 *
 * Kort in het Nederlands:
 * Als de gij niet bezig bent met pose estimation taken in PoseDetectionRelease modus moe de gij hier niet zijn.
 */

// Following includes and configurations are just to make it work and are not actually used in the pose detection configuration.
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")
#include <GLFW/glfw3.h>

#include "easylogging++.h"
#include "log.h"
#include "poseEstimation.h"
#include "poseRetriever.h"

GLFWwindow* window;
// END

#ifdef _DEBUG
#pragma comment (lib, "opencv_world490d.lib")
#else
#pragma comment (lib, "opencv_world490.lib")
#endif

INITIALIZE_EASYLOGGINGPP

int main(int argc, char** argv)
{
	setupLogger();
	//runPoseRetriever();
}