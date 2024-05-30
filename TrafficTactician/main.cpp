#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
using tigl::Vertex;

#include "easylogging++.h"
#include "log.h"


#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "Simulation.h"
#include "GameObject.h"
#include "PlayerComponent.h"
#include <glm/gtx/string_cast.hpp>
#include "CubeComponent.h"
#include "ModelComponent.h"
#include "SpinComponent.h"
#include "utest.h"
#include "WorldComponent.h"
using tigl::Vertex;



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

Simulation* sim;

int width = 1600, height = 900;
double lastFrameTime = 0;

void resize(GLFWwindow*, int w, int h)
{
	width = w;
	height = h;
}

#ifndef _TESTING_CONFIG
int main(void)
{
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

void initImGui()
{
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


void init()
{
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

	initImGui();
	LOG(INFO) << "Initialized ImGui window." << std::endl;


	sim->init();
	LOG(INFO) << "Initialized simulation." << std::endl;


	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE)
				glfwSetWindowShouldClose(window, true);
		});
	LOG(INFO) << "Initialized input callback." << std::endl;

}

void updateImGui()
{
	ImGui::SetNextWindowSize(ImVec2(200, 200));
	ImGui::ShowDemoWindow(0);
	if (ImGui::Begin("Hello Imgui")) {

		ImGui::Text("MousePosition3D: %f, %f, %f", sim->mousePosition3D.x, sim->mousePosition3D.y, sim->mousePosition3D.z);
		//float scale = world->scale.y;

		//ImGui::Text("Hello Computer Graphics!");
		//ImGui::SliderAngle("World rotation", &world->rotation.y);
		//ImGui::SliderFloat("Car Scale", &player->scale.y, 0, 1);
		//ImGui::SliderFloat("World Scale", &scale, 0.5f, 50.0f);

		//if (ImGui::Button("Hi")) {
		//	world->rotation.y += 0.1f;
		//}

		//static float translation[] = { 0, 0, 0 };
		//ImGui::SliderFloat3("position", translation, -2.0, 2.0);

		//// Player position set to slider pos.
		//world->position = glm::vec3(translation[0], translation[1], translation[2]);

		//world->scale = glm::vec3(scale);

		//std::string str;
		//ImGui::Text("MousePosition3D: (%f, %f, %f)", mousePosition3D.x, mousePosition3D.y, mousePosition3D.z);
		//ImGui::Text("Mouse2D: (%f, %f)", mouse2D.x, mouse2D.y);
		//ImGui::Text("Car Pos: (%f, %f, %f)", player->position.x, player->position.y, player->position.z);
		/*for (glm::vec3 point : player->getComponent<CarComponent>()->points) {
			ImGui::Text("(%f, %f, %f)",
				point.x, point.y, point.z);
		}*/

		ImGui::End();
	}
}

void update()
{
	double currentFrameTime = glfwGetTime();
	double deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;

	sim->update(static_cast<float>(deltaTime));

	updateImGui();
}



void draw()
{
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


