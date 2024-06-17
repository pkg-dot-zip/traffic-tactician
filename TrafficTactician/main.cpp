#include "log.h"
#include "easylogging++.h"
#include "CameraInputHandler.h"
#include "TrafficTactician.h"
#include "utest.h"
#include "poseRetriever.h"
#include "SettingsRetriever.h"
#include "SoundHandler.h"
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
	setupLogger(); // MUST go first before any log entries are submitted.

#ifdef _POSE_DEBUG
	return runPoseRetriever(cameraInputHandler::getCameraToUse());
#elif _TESTING_CONFIG
	return utest_main(argc, argv);
#else
	SoundHandler::getInstance().playSoundSnippet("sounds/Start_Sounds_013.wav"); // First sound is delayed so we play a sound at application boot.
	mainMenu::runMenu();
	cameraInputHandler::initCameraInput(cameraInputHandler::getCameraToUse());
	return runApp();
#endif
}
