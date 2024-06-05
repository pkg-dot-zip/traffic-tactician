#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <memory>

#include "InputHandler.h"
#include "KeyBoardInputHandler.h"
using tigl::Vertex;

#include "easylogging++.h"
#include "log.h"

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "Simulation.h"
#include "Scene.h"
#include "GameObject.h"
#include "PlayerComponent.h"
#include <glm/gtx/string_cast.hpp>
#include "CubeComponent.h"
#include "ModelComponent.h"
#include "SpinComponent.h"
#include "utest.h"
#include "WorldComponent.h"
using tigl::Vertex;

#include "stb_image.h"
#include <random>
#include <CarComponent.h>
#include <RouteComponent.h>

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

void init();
void draw();
void update();
void onDestroy();

void initImGui();
void updateImGui();

void loadTextures();
GLuint loadTexture(const std::string& imageName);
void restartTimer();

Simulation* sim;
GLuint* currentSignTexture = nullptr; // Change this to a pointer to the current sign texture e.g. currentSignTexture = &textures["stopSign"];
std::map<std::string, GLuint> textures; // scoreLogo, stopSign, forwardSign, leftSign, rightSign

int width = 1600, height = 900, score = 0;
double lastFrameTime = 0, timerIsDone = 0;
float progress = 0.0f, timerSeconds = 10.0f, remainingTime = -1.0f; // float progress start at 0% | 1.0f = 100% 


void resize(GLFWwindow*, int w, int h) {
	width = w;
	height = h;
}

#ifndef _TESTING_CONFIG
int main(void) {

	setupLogger(); // MUST go first before any log entries are submitted.

	if (!glfwInit())
		throw "Could not initialize glwf";
	window = glfwCreateWindow(width, height, "TrafficTactician", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		throw "Could not initialize glwf";
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, resize);

	tigl::init();
	sim = new Simulation(window);
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

	// Load a font for ImGui 
	//io.Fonts->AddFontFromFileTTF("font.ttf", 24.0f);

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

	// Load textures and set the current sign texture to the stop sign
	loadTextures();
	currentSignTexture = &textures["stopSign"];

	initImGui();
	LOG(INFO) << "Initialized ImGui window." << std::endl;

	sim->init();
	LOG(INFO) << "Initialized simulation." << std::endl;


	initKeyCallback(window);
	initCameraInput();
}

void updateImGui() {

	ImGui::SetNextWindowPos(ImVec2(0, 0)); // Set the window position to the top left corner
	ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, 100)); // Set the window width to the display width and height to 100
	std::shared_ptr<GameObject> car = sim->scene->car;
	ImGui::Text("CarPosition: %f, %f, %f", car->position.x, car->position.y, car->position.z);
	ImGui::SliderAngle("CarRotation:", &car->rotation.y);
	bool continueRoute = false;
	if (ImGui::Checkbox("Continue route", &continueRoute)) {
		sim->scene->car->getComponent<RouteComponent>()->state = RouteComponent::RouteState::MovingSecond;
	}


		const std::string poseString = "Pose: " + getPoseString(getInputPose());
		ImGui::Text(poseString.c_str());
		//float scale = world->scale.y;

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.25f)); // Set the window background color to semi-transparent black

	if (ImGui::Begin("Status", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar)) { // Add ImGuiWindowFlags_NoTitleBar to hide the title bar

		//// Select a random texture
		//std::random_device rd;
		//std::mt19937 gen(rd());
		//std::uniform_int_distribution<> dis(1, textures.size());
		//auto it = textures.begin();
		//std::advance(it, dis(gen) - 1);
		//GLuint randomTexture = it->second;
		//ImGui::Image((void*)(intptr_t)randomTexture, ImVec2(31, 31)); // Display the random icon

		ImGui::Image((void*)(intptr_t)*currentSignTexture, ImVec2(31, 31)); // Display the icon

		ImGui::SameLine(); // Keep the following items on the same line with an offset

		float windowWidth = ImGui::GetWindowWidth();
		float imageWidth = 31.0f; // Width of the image
		float textWidth = ImGui::CalcTextSize(std::to_string(score).c_str()).x; // Width of the text

		ImGui::SetCursorPosX(windowWidth - imageWidth - textWidth - 20.0f); // Set the cursor position to align the image and score to the right, with a small padding of 20.0f

		ImGui::Image((void*)(intptr_t)textures["scoreLogo"], ImVec2(31, 31)); // Display the icon
		//ImGui::SameLine(0.0f, ImGui::GetTextLineHeight() / 2); // Keep the following items on the same line with an offset
		ImGui::SameLine(); // Keep the following items on the same line with an offset

		ImGui::Text("%d", score); // Display the text and integer

		//ImGui::Text("MousePosition3D: %f, %f, %f", sim->mousePosition3D.x, sim->mousePosition3D.y, sim->mousePosition3D.z);

		char overlay[32];
		sprintf_s(overlay, "%.2f s", remainingTime);
		ImGui::ProgressBar(progress, ImVec2(-1.0f, 0.0f), overlay); // Full width progress bar 

		ImGui::End();
	}

	ImGui::PopStyleColor(); // Reset the window background color to the default
}

void restartTimer() {
	if (progress >= 1.00f) {
		progress = 0.0f; // Clamp to 0
		timerIsDone = false;
		std::cout << "Timer has been restarted!" << std::endl;
	}
	else {
		std::cout << "Timer is not done yet!" << std::endl;
	}
}

void update() {
	double currentFrameTime = glfwGetTime();
	double deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;

	if (progress >= 1.0f) {
		timerIsDone = true;
		restartTimer();
	}
	else {
		// Increase the progress over time (timerSeconds) 
		progress += 1.0f / timerSeconds * deltaTime; // Increase the timer 1.0f/timerSeconds 
	}

	// Calculate the remaining time
	remainingTime = timerSeconds * (1.0f - progress);

	// Set the remaining time to 0 if it's less than 0 
	if (remainingTime < 0.0f) {
		remainingTime = 0.0f;
	}

	sim->update(static_cast<float>(deltaTime));

	updateImGui();
}

void draw() {

	if (width == 0 || height == 0)
		return;
	glViewport(0, 0, width, height);
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	tigl::shader->enableTexture(false); //
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

	// Draw ImGui
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

void loadTextures() {
	textures["scoreLogo"] = loadTexture("score_logo.png");
	textures["stopSign"] = loadTexture("sign_stop.png");
	textures["forwardSign"] = loadTexture("sign_forward.png");
	textures["leftSign"] = loadTexture("sign_left.png");
	textures["rightSign"] = loadTexture("sign_sign.png");
}

GLuint loadTexture(const std::string& imageName) {
	GLuint texture = 0; // Initialize texture to 0

	int width, height, nrChannels;
	const char* imageName2 = imageName.c_str();
	unsigned char* data = stbi_load(imageName2, &width, &height, &nrChannels, STBI_rgb_alpha);
	if (data)
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	return texture;
}
