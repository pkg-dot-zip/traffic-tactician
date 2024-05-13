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
using tigl::Vertex;

#pragma comment (lib, "opencv_world490d.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

void init();
void update();
void draw();
void drawTriangle();
void drawCube();
void drawRotatingCube(glm::vec3& pos, glm::vec3& axis);
void drawSquare(glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, glm::vec3& p4, glm::vec4 color);

int width = 1400;
int height = 800;
float rotation = 0;
float cubeX, cubeY, cubeZ = 0;
float axisX, axisY, axisZ = 0;
bool isRotated = false;

GLFWwindow* window;

INITIALIZE_EASYLOGGINGPP

int main(void)
{
	if (!glfwInit())
		throw "Could not initialize glwf";
	window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		throw "Could not initialize glwf";
	}
	glfwMakeContextCurrent(window);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	// Setup Platform/Renderer settings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	tigl::init();

	init();
	tigl::shader->enableColor(true);


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

		/// IMGUI stuff here
		ImGui::Text("Hello Computer Graphics!");

		static float translation[] = { 0, 0, 0 };
		ImGui::SliderFloat3("position", translation, -2.0, 2.0);
		cubeX = translation[0];
		cubeY = translation[1];
		cubeZ = translation[2];

		static float axis[] = { 0, 0, 0 };
		ImGui::SliderFloat3("axis", axis, -2.0, 2.0);
		axisX = axis[0];
		axisY = axis[1];
		axisZ = axis[2];

		static bool rotateCheck = true;
		if (ImGui::Checkbox("Rotate?", &rotateCheck)) {
			isRotated = rotateCheck;
		}

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

double lastFrameTime = 0;
std::list<std::shared_ptr<GameObject>> objects;
std::shared_ptr<GameObject> player;

void init()
{
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE)
				glfwSetWindowShouldClose(window, true);
		});

    // Create Player object.
    player = std::make_shared<GameObject>();
    player->position = glm::vec3(0, 1, 5);
    // player->addComponent(std::make_shared<CubeComponent>(1.0f));
    player->addComponent(std::make_shared<PlayerComponent>());
    player->addComponent(std::make_shared<CubeComponent>(1.0f));
    objects.push_back(player);
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

    // Draw Gameobjects.
    for (std::shared_ptr<GameObject>& gameObject : objects)
    {
        gameObject->draw();
    }
}
