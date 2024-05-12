#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include "easylogging++.h"
#include "log.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
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


void init()
{
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE)
				glfwSetWindowShouldClose(window, true);
		});

}

double lastTime = 0;
void update()
{
	double time = glfwGetTime();
	double deltaTime = time - lastTime;
	lastTime = time;
	if (isRotated){
		rotation += 3 * deltaTime;
	}
}

void draw()
{
	glViewport(0, 0, width, height);
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	tigl::shader->setProjectionMatrix(glm::perspective(glm::radians(70.0f), (float)width / height, 0.1f, 200.0f));
	tigl::shader->setViewMatrix(glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
	glEnable(GL_DEPTH_TEST);



	auto cubePos = glm::vec3(cubeX, cubeY, cubeZ);
	auto cubeAxis = glm::vec3(axisX, axisY, axisZ);
	drawRotatingCube(cubePos, cubeAxis);


	//auto pos1 = glm::vec3(0.5, 1, 1);
	//auto axis1 = glm::vec3(0, 1, 0);
	//drawRotatingCube(pos1, axis1);

}

void drawTriangle() {
	tigl::begin(GL_TRIANGLES);
	tigl::addVertex(Vertex::P(glm::vec3(-1, 0, 0)));
	tigl::addVertex(Vertex::P(glm::vec3(1, 0, 0)));
	tigl::addVertex(Vertex::P(glm::vec3(0, 1, 0)));
	tigl::end();
}

void drawSquare(glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, glm::vec3& p4, glm::vec4 color) {
	//auto red = glm::vec4(1, 0, 0, 1);
	//auto green = glm::vec4(0, 1, 0, 1);
	//auto blue = glm::vec4(0, 0, 1, 1);

	//tigl::addVertex(Vertex::PC(glm::vec3(-1, -1, 0), red));
	//tigl::addVertex(Vertex::PC(glm::vec3(-1, 1, 0), red));
	//tigl::addVertex(Vertex::PC(glm::vec3(1, 1, 0), red));
	//tigl::addVertex(Vertex::PC(glm::vec3(1, -1, 0), red));

	tigl::begin(GL_QUADS);
	tigl::addVertex(Vertex::PC(p1, color));
	tigl::addVertex(Vertex::PC(p2, color));
	tigl::addVertex(Vertex::PC(p3, color));
	tigl::addVertex(Vertex::PC(p4, color));
	tigl::end();
}

void drawCube() {
	// front
	auto f1 = glm::vec3(0, 0, 0);
	auto f2 = glm::vec3(1, 0, 0);
	auto f3 = glm::vec3(1, 1, 0);
	auto f4 = glm::vec3(0, 1, 0);
	drawSquare(f1, f2, f3, f4, glm::vec4(1, 0, 0, 1));

	// back
	auto back1 = glm::vec3(0, 0, -1);
	auto back2 = glm::vec3(1, 0, -1);
	auto back3 = glm::vec3(1, 1, -1);
	auto back4 = glm::vec3(0, 1, -1);
	drawSquare(back1, back2, back3, back4, glm::vec4(0, 1, 0, 1));

	// right
	auto r1 = glm::vec3(0, 0, 0);
	auto r2 = glm::vec3(0, 0, -1);
	auto r3 = glm::vec3(0, 1, -1);
	auto r4 = glm::vec3(0, 1, 0);
	drawSquare(r1, r2, r3, r4, glm::vec4(0, 0, 1, 1));

	// left
	auto l1 = glm::vec3(1, 0, 0);
	auto l2 = glm::vec3(1, 0, -1);
	auto l3 = glm::vec3(1, 1, -1);
	auto l4 = glm::vec3(1, 1, 0);
	drawSquare(l1, l2, l3, l4, glm::vec4(1, 1, 0, 1));

	// top
	auto t1 = glm::vec3(0, 1, 0);
	auto t2 = glm::vec3(1, 1, 0);
	auto t3 = glm::vec3(1, 1, -1);
	auto t4 = glm::vec3(0, 1, -1);
	drawSquare(t1, t2, t3, t4, glm::vec4(1, 0, 1, 1));

	// bottom
	auto bottom1 = glm::vec3(0, 0, 0);
	auto bottom2 = glm::vec3(1, 0, 0);
	auto bottom3 = glm::vec3(1, 0, -1);
	auto bottom4 = glm::vec3(0, 0, -1);
	drawSquare(bottom1, bottom2, bottom3, bottom4, glm::vec4(1, 0, 1, 1));






}

void drawRotatingCube(glm::vec3& pos, glm::vec3& axis)
{

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, pos);
	modelMatrix = glm::rotate(modelMatrix, rotation, axis);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f, -0.5f, 0.5f));
	tigl::shader->setModelMatrix(modelMatrix);
	drawCube();
}
