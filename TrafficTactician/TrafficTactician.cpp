#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <memory>

#include "CameraInputHandler.h"
#include "KeyBoardInputHandler.h"
#include "TextureCache.h"
#include "Texture.h"
#include "SettingsRetriever.h"
#include "StatusHandler.h"
#include "SoundHandler.h"
using tigl::Vertex;

#include "easylogging++.h"

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

GLFWwindow* window;

void initFog();
void init();
void draw();
void update();
void onDestroy();

void initImGui();
void updateImGui();

int gameLoop();
void simulationLoop();
void endScreenLoop();
void restart();

std::shared_ptr<Simulation> sim;

int width = GetGraphicSettings().screenWidth, height = GetGraphicSettings().screenHeight;
double lastFrameTime = 0;

bool drawStatus = true;
bool drawDebugMenu = false;
bool drawEndScreen = true;

enum EndOption {
	Quit, Restart, InfinityMode
};
EndOption endOption = Quit;

std::array clearColor = { 0.3f, 0.4f, 0.6f, 1.0f };

int runApp() {
	// TODO: create a hash function for the random number generator to make a reproducible test
	srand((unsigned int)time(NULL)); // Set seed to for rand() calls. 

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
	glfwSetWindowSizeCallback(window, [](GLFWwindow*, int w, int h)
	{
		width = w;
		height = h;
	});

	tigl::init();
	sim = std::make_shared<Simulation>(window);
	init();

	return gameLoop();
}

int gameLoop() {
	simulationLoop();
	endScreenLoop();

	if (endOption == Restart) {
		restart();
	}
	else if (endOption == InfinityMode) {
		LOG(INFO) << "Entering infinity mode." << std::endl;
		restart();
	}
	else {
		// Quit
		onDestroy();
		return 0;
	}
}

void restart() {
	LOG(INFO) << "Restarting game." << std::endl;
	
	// Reset state
	sim->scene->data.points = 0;
	glfwSetWindowShouldClose(window, false);
	drawEndScreen = true;

	gameLoop();
}

void simulationLoop() {
	while (!glfwWindowShouldClose(window))
	{
		// Get score and check if the game should end.
		if (endOption != InfinityMode && sim->scene->data.points >= 10) glfwSetWindowShouldClose(window, true);

		// Start new ImGui frame.
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		update();
		draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void endScreenLoop() {
	// Show end screen
	LOG(INFO) << "Entering end screen." << std::endl;
	while (drawEndScreen)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowPos({ 0, 0 });
		ImGui::SetNextWindowSize({ ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y });

		// Build screen
		ImGui::Begin("End Screen", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoScrollWithMouse);

		Scene::OverlayData data = sim->scene->data;
		const std::string scoreString = "Your score:";
		const int score = data.points;

		const float windowWidth = ImGui::GetWindowWidth();
		const float windowHeight = ImGui::GetWindowHeight();
		constexpr float imageWidth = 200.0f;
		const float textWidth = ImGui::CalcTextSize(scoreString.c_str()).x;
		const float scoreWidth = ImGui::CalcTextSize(std::to_string(score).c_str()).x;

		ImGui::SetCursorPos({ (windowWidth - imageWidth) / 2, (windowHeight - imageWidth) / 2 - imageWidth }); // Center image
		//ImGui::Image((void*)(intptr_t)data.textures["scoreLogo"], ImVec2(imageWidth, imageWidth), { 0,1 }, { 1,0 }); // Score icon
		ImGui::Image((void*)(intptr_t)TextureCache::loadTexture("images/windowIcons/traffic_tactician_round_icon.png")->id, ImVec2(imageWidth, imageWidth), { 0,1 }, { 1,0 }); // Score icon
		ImGui::SetWindowFontScale(2.0f);
		ImGui::SetCursorPosX((windowWidth - textWidth) / 2); // Center text
		ImGui::Text(("\n" + scoreString).c_str());
		ImGui::SetCursorPosX((windowWidth - scoreWidth) / 2); // Center score
		ImGui::Text("%d", score);

		const ImVec2 buttonSize = { ImGui::GetIO().DisplaySize.x - 16, 48 };
		if (ImGui::Button("Restart", buttonSize))
		{
			LOG(INFO) << "Pressed Restart button." << std::endl;
			SoundHandler::getInstance().playSoundSnippet("sounds/menu_click.wav");
			endOption = Restart;
			drawEndScreen = false;
		}
		if (ImGui::Button("Infinity Mode", buttonSize))
		{
			LOG(INFO) << "Pressed Infinity Mode button." << std::endl;
			SoundHandler::getInstance().playSoundSnippet("sounds/menu_click.wav");
			endOption = InfinityMode;
			drawEndScreen = false;
		}
		if (ImGui::Button("Quit", buttonSize))
		{
			LOG(INFO) << "Pressed Quit button." << std::endl;
			SoundHandler::getInstance().playSoundSnippet("sounds/menu_click.wav");
			endOption = Quit;
			drawEndScreen = false;
		}
		ImGui::End();

		// Render
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

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

	TextureCache::preloadTextures();
	initImGui();
	sim->init(sim);
	initKeyCallback(window);
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

	if (drawDebugMenu)
	{
		ImGui::SetNextWindowPos(ImVec2(0, 0)); // Set the window position to the top left corner
		ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, 100)); // Set the window width to the display width and height to 100

		if (sim->scene->currentCarObject.expired())
		{
			LOG(WARNING) << "No car found to update UI for. Returning." << std::endl;
			return;
		}

		const std::shared_ptr<GameObject> car = sim->scene->currentCarObject.lock();
		ImGui::Text("CarPosition: %f, %f, %f", car->position.x, car->position.y, car->position.z);
		ImGui::SliderAngle("CarRotation:", &car->rotation.y);

		if (car->getComponent<ControllerComponent>().has_value()) {
			const auto correctPose = car->getComponent<ControllerComponent>().value()->correctPose;
			ImGui::Text("Correct Pose: %s", getPoseString(correctPose).c_str());
		}
		else
		{
			LOG(ERROR) << "Error: Can not update UI when no ControllerComponent can be found." << std::endl;
			throw std::exception("Error: Can not update UI when no ControllerComponent can be found.");
		}

		const std::string poseString = "Pose: " + getPoseString(cameraInputHandler::getInputPose());
		ImGui::Text(poseString.c_str());
	}

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.25f)); // Set the window background color to semi-transparent black

	if (drawStatus)
	{
		statusHandler::showStatus(sim);
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
