#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include "easylogging++.h"
#include "GameObject.h"
#include "log.h"
#include "PlayerComponent.h"
#include "CubeComponent.h"
#include "ModelComponent.h"
#include "SpinComponent.h"
using tigl::Vertex;

#pragma comment (lib, "opencv_world490d.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

INITIALIZE_EASYLOGGINGPP

void init();
void initWindow();
void initPlayer();
void initInputCallback();
void update();
void draw();
void updateImGuiWindow();
void drawImGuiWindow();
void onDestroy();


int width = 1600;
int height = 900;
float rotation = 0;
float posX, posY, posZ = 0;
float axisX, axisY, axisZ = 0;
bool isRotated = false;

GLFWwindow* window;

double lastFrameTime = 0;
std::list<std::shared_ptr<GameObject>> objects;
std::shared_ptr<GameObject> player;

int main(void)
{
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

void onDestroy()
{
	LOG(INFO) << "DeInitializing.";
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}

void initWindow()
{
	LOG(INFO) << "Initialized window.";

	if (!glfwInit()) throw "Could not initialize glwf";
	window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);

	if (!window)
	{

		glfwTerminate();
		throw "Could not initialize glwf";
	}

	glfwMakeContextCurrent(window);
}

void initPlayer()
{
	LOG(INFO) << "Initialized player.";
	player = std::make_shared<GameObject>();
	player->position = glm::vec3(0, 0, 0);

	std::shared_ptr<ModelComponent> model_component = std::make_shared<ModelComponent>("models/car_kit/hatchback-sports.obj");
	player->addComponent(model_component);
	objects.push_back(player);
}

void initInputCallback()
{
	LOG(INFO) << "Initialized input callback.";
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE)
				glfwSetWindowShouldClose(window, true);
		});
}

void initImGui()
{
	LOG(INFO) << "Initialized ImGui.";

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
}

void init()
{
	setupLogger(); // MUST go first before any log entries are submitted.

	initWindow();
	tigl::init();

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

	initPlayer();
	initInputCallback();
	initImGui();
}

void update()
{
	updateImGuiWindow();
	// Calculate timings.
	double currentFrameTime = glfwGetTime();
	double deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;

	// Updated gameObjects.
	for (std::shared_ptr<GameObject>& gameObject : objects)
	{
		gameObject->update(static_cast<float>(deltaTime));
	}
}

void draw()
{
	// Set GL.
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	const glm::mat4 projection = glm::perspective(glm::radians(75.0f), static_cast<float>(viewport[2]) / static_cast<float>(viewport[3]), 0.01f, 1000.0f);

	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setViewMatrix(glm::lookAt(glm::vec3(0, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
	tigl::shader->setModelMatrix(glm::mat4(1.0f));

	tigl::shader->enableColor(true);

	// Temporary floor drawing.
	tigl::begin(GL_QUADS);
	tigl::addVertex(Vertex::PCN(glm::vec3(-50, 0, -50), glm::vec4(1, 0, 0, 1), glm::vec3(0, 1, 0)));
	tigl::addVertex(Vertex::PCN(glm::vec3(-50, 0, 50), glm::vec4(0, 1, 0, 1), glm::vec3(0, 1, 0)));
	tigl::addVertex(Vertex::PCN(glm::vec3(50, 0, 50), glm::vec4(0, 0, 1, 1), glm::vec3(0, 1, 0)));
	tigl::addVertex(Vertex::PCN(glm::vec3(50, 0, -50), glm::vec4(0, 0, 1, 1), glm::vec3(0, 1, 0)));
	tigl::end();

	// Draw Gameobjects.
	for (std::shared_ptr<GameObject>& gameObject : objects)
	{
		gameObject->draw();
	}

	drawImGuiWindow(); //Must be done after drawing objects.
}

void updateImGuiWindow()
{
	ImGui::SetNextWindowSize(ImVec2(200, 200));
	ImGui::ShowDemoWindow(0);
	if (ImGui::Begin("Hello Imgui")) {
		float scale = player->scale.y;

		ImGui::Text("Hello Computer Graphics!");
		ImGui::SliderAngle("Rotation", &player->rotation.y);
		ImGui::SliderFloat("Rotation", &player->rotation.y, 0, 10);
		ImGui::SliderFloat("Scale", &scale, 1.0f, 100.0f);

		if (ImGui::Button("Hi")) {
			player->rotation.y += 0.1f;
		}

		static float translation[] = { 0, 0, 0 };
		ImGui::SliderFloat3("position", translation, -2.0, 2.0);

		// Player position set to slider pos.
		player->position = glm::vec3(translation[0], translation[1], translation[2]);

		player->scale = glm::vec3(scale);


		static bool rotateCheck = false;
		if (ImGui::Checkbox("Rotate?", &rotateCheck))
		{
			if (rotateCheck)
			{
				player->addComponent(std::make_shared<SpinComponent>());
			}
			else
			{
				player->removeComponent(player->getComponent<SpinComponent>());
			}
		}

		ImGui::End();
	}

}

void drawImGuiWindow() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
