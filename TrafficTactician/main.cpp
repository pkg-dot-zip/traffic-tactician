#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <memory>

#include "InputHandler.h"
#include "KeyBoardInputHandler.h"
#include "TextureCache.h"
#include "SettingsRetriever.h"
using tigl::Vertex;

#include "easylogging++.h"
#include "log.h"

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "Simulation.h"
#include "Scene.h"
#include "GameObject.h"
#include <glm/gtx/string_cast.hpp>
#include "utest.h"

#include <random>
#include <RouteComponent.h>
#include <ControllerComponent.h>

#ifdef _DEBUG
#pragma comment (lib, "opencv_world490d.lib")
#else
#pragma comment (lib, "opencv_world490.lib")
#endif

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

#ifdef _TESTING_CONFIG
UTEST_MAIN();
#endif

INITIALIZE_EASYLOGGINGPP

GLFWwindow* window;

void initFog();
void init();
void draw();
void update();
void onDestroy();

void initImGui();
void updateImGui();

std::shared_ptr<Simulation> sim;
void loadTextures();

int width = 1600, height = 900;
double lastFrameTime = 0;

std::array<float, 4> clearColor = { 0.3f, 0.4f, 0.6f, 1.0f };

void resize(GLFWwindow*, int w, int h) {
	width = w;
	height = h;
}

#ifndef _TESTING_CONFIG
int main(void) {

	setupLogger(); // MUST go first before any log entries are submitted.

	if (GetGraphicSettings().mxaaEnabled) glfwWindowHint(GLFW_SAMPLES, 4); // Multisample anti-aliasing.

	if (!glfwInit()) throw std::exception("Could not initialize glwf");
	window = glfwCreateWindow(width, height, "TrafficTactician", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		throw std::exception("Could not initialize glwf");
	}

	glfwMakeContextCurrent(window);
	if (GetGraphicSettings().mxaaEnabled) glEnable(GL_MULTISAMPLE);
	glfwSetWindowSizeCallback(window, resize);

	tigl::init();
	sim = std::make_shared<Simulation>(window);
	init();

	while (!glfwWindowShouldClose(window))
	{
		// Start new ImGui frame.
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		update();
		draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	onDestroy();

	return 0;
}
#endif

void initImGui() {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	// Setup Platform/Renderer settings.
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	// Setup Dear ImGui style.
	ImGui::StyleColorsDark();

	LOG(INFO) << "Initialized ImGui." << std::endl;
}

void init() {
	glEnable(GL_DEPTH_TEST);

	tigl::shader->enableColor(true);
	tigl::shader->enableLighting(true);
	tigl::shader->setLightCount(1);
	tigl::shader->setLightDirectional(0, true);
	tigl::shader->setLightPosition(0, glm::normalize(glm::vec3(1, 1, 1)));
	tigl::shader->setLightAmbient(0, glm::vec3(0.5f, 0.5f, 0.5f));
	tigl::shader->setLightDiffuse(0, glm::vec3(0.5f, 0.5f, 0.5f));
	tigl::shader->setLightSpecular(0, glm::vec3(1, 1, 1));
	tigl::shader->setShinyness(0);
	initFog();

	loadTextures();

	initImGui();
	LOG(INFO) << "Initialized ImGui window." << std::endl;
	
	sim->init(sim);
	LOG(INFO) << "Initialized simulation." << std::endl;


	initKeyCallback(window);
	initCameraInput();
}

void initFog()
{
	if (!GetGraphicSettings().useFog) return;

	tigl::shader->enableFog(true);
	tigl::shader->setFogColor({ clearColor[0], clearColor[1], clearColor[2] }); // Must match the clear color.


	if (GetGraphicSettings().fogType == "exp2")
	{
		tigl::shader->setFogExp2(GetGraphicSettings().fogExponentialDensity);
	}
	else if (GetGraphicSettings().fogType == "exp")
	{
		tigl::shader->setFogExp(GetGraphicSettings().fogExponentialDensity);
	}
	else if (GetGraphicSettings().fogType == "linear")
	{
		tigl::shader->setFogLinear(GetGraphicSettings().fogLinearNear, GetGraphicSettings().fogLinearFar);
	}
	else
	{
		tigl::shader->enableFog(false);
		LOG(WARNING) << "Invalid fog type found in settings. Disabling fog." << std::endl;
		return;
	}

	LOG(INFO) << "Initialized fog." << std::endl;
}

void updateImGui() {

	ImGui::SetNextWindowPos(ImVec2(0, 0)); // Set the window position to the top left corner
	ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, 100)); // Set the window width to the display width and height to 100
	std::shared_ptr<GameObject> car = sim->scene->currentCarObject;
	ImGui::Text("CarPosition: %f, %f, %f", car->position.x, car->position.y, car->position.z);
	ImGui::SliderAngle("CarRotation:", &car->rotation.y);
	bool continueRoute = false;
	if (ImGui::Checkbox("Continue route", &continueRoute)) {

		if (sim->scene->currentCarObject->getComponent<RouteComponent>().has_value())
		{
			sim->scene->currentCarObject->getComponent<RouteComponent>().value()->state = RouteComponent::RouteState::Moving;
			sim->scene->currentCarObject->getComponent<RouteComponent>().value()->crossed = true;
		} else
		{
			LOG(ERROR) << "Error: Can not update UI when no RouteComponent can be found." << std::endl;
			throw std::exception("Error: Can not update UI when no RouteComponent can be found.");
		}
		
	}
	
	if (sim->scene->currentCarObject->getComponent<ControllerComponent>().has_value()) {
		const auto correctPose = sim->scene->currentCarObject->getComponent<ControllerComponent>().value()->correctPose;
		ImGui::Text("Correct Pose: %s", getPoseString(correctPose).c_str());
	} else
	{
		LOG(ERROR) << "Error: Can not update UI when no ControllerComponent can be found." << std::endl;
		throw std::exception("Error: Can not update UI when no ControllerComponent can be found.");
	}


	const std::string poseString = "Pose: " + getPoseString(getInputPose());
	ImGui::Text(poseString.c_str());

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.25f)); // Set the window background color to semi-transparent black

	// TODO uit de main halen smh
	if (ImGui::Begin("Status", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar)) 
	{ 

		Scene::OverlayData data = sim->scene->data;
		ImGui::Image((void*)(intptr_t)*data.currentSignTexture, ImVec2(31, 31), { 0,1 }, { 1,0 }); // Display the icon

		ImGui::SameLine(); // Keep the following items on the same line with an offset

		const int score = data.points;

		const float windowWidth = ImGui::GetWindowWidth();
		constexpr float imageWidth = 31.0f; // Width of the image
		const float textWidth = ImGui::CalcTextSize(std::to_string(score).c_str()).x; // Width of the text

		ImGui::SetCursorPosX(windowWidth - imageWidth - textWidth - 20.0f); // Set the cursor position to align the image and score to the right, with a small padding of 20.0f

		ImGui::Image((void*)(intptr_t)data.textures["scoreLogo"], ImVec2(31, 31), {0,1}, {1,0}); // Display the icon
		ImGui::SameLine(); // Keep the following items on the same line with an offset

		ImGui::Text("%d", score); // Display the text and integer

		if (sim->scene->currentCarObject->getComponent<RouteComponent>().has_value())
		{
			if (sim->scene->currentCarObject->getComponent<RouteComponent>().value()->state == RouteComponent::RouteState::Idle)
			{
				// Start timer
				sim->scene->currentCarObject->getComponent<ControllerComponent>().value()->timer->toggleTimer(true);

				char overlay[32];
				sprintf_s(overlay, "%.2f s", data.remainingTime);
				ImGui::ProgressBar(data.progress, ImVec2(-1.0f, 0.0f), overlay); // Full width progress bar .
			}
		}
		else
		{
			LOG(ERROR) << "Error: Can not update UI when no ControllerComponent can be found." << std::endl;
			throw std::exception("Error: Can not update UI when no ControllerComponent can be found.");
		}

		ImGui::End();
	}

	ImGui::PopStyleColor(); // Reset the window background color to the default.
}

void update() {
	const double currentFrameTime = glfwGetTime();
	const double deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;

	sim->update(static_cast<float>(deltaTime));

	updateImGui();
}

void draw() {
	if (width == 0 || height == 0) return;
	glViewport(0, 0, width, height);
	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	tigl::shader->enableTexture(false);
	tigl::shader->enableColor(true);
	tigl::shader->enableLighting(true);
	tigl::shader->setLightCount(1);
	tigl::shader->setLightDirectional(0, true);
	tigl::shader->setLightPosition(0, glm::normalize(glm::vec3(1, 1, 1)));
	tigl::shader->setLightAmbient(0, glm::vec3(0.5f, 0.5f, 0.5f));
	tigl::shader->setLightDiffuse(0, glm::vec3(0.5f, 0.5f, 0.5f));
	tigl::shader->setLightSpecular(0, glm::vec3(1, 1, 1));
	tigl::shader->setShinyness(0);

	sim->draw();

	// Draw ImGui.
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void onDestroy() {
	LOG(INFO) << "Deinitializing..." << std::endl;
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}

// preload textures
void loadTextures()
{
	TextureCache::loadTexture("score_logo.png");
	TextureCache::loadTexture("sign_stop.png");
	TextureCache::loadTexture("sign_forward.png");
	TextureCache::loadTexture("sign_left.png");
	TextureCache::loadTexture("sign_right.png");
}
