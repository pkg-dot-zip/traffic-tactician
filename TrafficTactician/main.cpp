#include "log.h"
#include "easylogging++.h"
#include "TrafficTactician.h"
#include "utest.h"
#include "poseRetriever.h"
#include "startMenu.h"

#ifdef _DEBUG
#pragma comment (lib, "opencv_world490d.lib")
#else
#pragma comment (lib, "opencv_world490.lib")
#endif

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

INITIALIZE_EASYLOGGINGPP

#ifdef _TESTING_CONFIG
UTEST_STATE();
#endif

int main(int argc, const char* const argv[])
{
	srand(5); // Set seed for rand() calls.
	setupLogger(); // MUST go first before any log entries are submitted.

#ifdef _POSE_DEBUG
	return runPoseRetriever();
#elif _TESTING_CONFIG
	return utest_main(argc, argv);
#else
	mainMenu::runMenu();
	return runApp();
#endif
}
