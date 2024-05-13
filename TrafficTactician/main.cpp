#include "imgui.h"
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
#include "SpinComponent.h"
using tigl::Vertex;

#pragma comment (lib, "opencv_world490d.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

void init();
void update();
void draw();

int width = 1400;
int height = 800;
float rotation = 0;
float posX, posY, posZ = 0;
float axisX, axisY, axisZ = 0;
bool isRotated = false;

GLFWwindow* window;

double lastFrameTime = 0;
std::list<std::shared_ptr<GameObject>> objects;
std::shared_ptr<GameObject> player;

INITIALIZE_EASYLOGGINGPP

int main(void)
{
	if (!glfwInit())
		throw "Could not initialize glwf";
	window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		throw "Could not initialize glwf";
	}
	glfwMakeContextCurrent(window);

	tigl::init();
	init();



	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	// Setup Platform/Renderer settings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();


	while (!glfwWindowShouldClose(window))
	{
		update();
		draw();

		// feed imputs to dear imgui, start new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// render your GUI
		ImGui::Begin("Hello Imgui");

#pragma region ImGui-Content
		ImGui::Text("Hello Computer Graphics!");

		static float translation[] = { 0, 0, 0 };
		ImGui::SliderFloat3("position", translation, -2.0, 2.0);
		//player position set to slider pos
		player->position = glm::vec3(translation[0], translation[1], translation[2]);


		static bool rotateCheck = false;
		if (ImGui::Checkbox("Rotate?", &rotateCheck)) {
			if (rotateCheck) {
				player->addComponent(std::make_shared<SpinComponent>(1));
			}
			else {
				player->removeComponent(player->getComponent<SpinComponent>());

			}
		}

#pragma endregion ImGui-Content

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

	return 0;
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

	// Create Player object.
	player = std::make_shared<GameObject>();
	player->position = glm::vec3(0, 1, 2);
	
	player->addComponent(std::make_shared<CubeComponent>(1.0f));
	objects.push_back(player);


	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE)
				glfwSetWindowShouldClose(window, true);
		});


}

double lastTime = 0;
void update()
{
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
	glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 1000.0f);

	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setViewMatrix(glm::lookAt(glm::vec3(0, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
	tigl::shader->setModelMatrix(glm::mat4(1.0f));

	tigl::shader->enableColor(true);
	//temporary draw floor
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
}
