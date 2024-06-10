#include <iostream>
#include <memory>

#include "easylogging++.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>

#include "backends/imgui_impl_opengl3.h"

#define GLSL_VERSION "#version 330"

namespace mainMenu
{
	GLFWwindow* window;

	bool menu_should_run = true;

	bool shouldRunMenu()
	{
		return menu_should_run;
	}

	void init()
	{
		LOG(INFO) << "Initializing main menu." << std::endl;

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(600, 600, "Main Menu",
			nullptr, nullptr);

		if (window == nullptr) {
			LOG(INFO) << "Could not create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(window);

		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		// io.IniFilename = "imguiMainMenu.ini";
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(GLSL_VERSION);
	}

	void onShutdown()
	{
		LOG(INFO) << "Shutting down main menu..." << std::endl;
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void runMenu() {
		// TODO: Implement menu skip.

		init();

		LOG(INFO) << "Running main menu" << std::endl;

		while (shouldRunMenu())
		{
			glfwPollEvents();

			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Window");
			{
				ImGui::Text("Hello, Dear ImGUI!");
				if (ImGui::SmallButton("Start"))
				{
					LOG(INFO) << "Pressed Start button." << std::endl;
					menu_should_run = false;
				}
			}
			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
		}

		onShutdown();
	}
}
