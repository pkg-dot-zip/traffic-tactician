#include <iostream>
#include <memory>
#include <filesystem>

#include "easylogging++.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>

#include "CameraInputHandler.h"
#include "SoundHandler.h"
#include "stb_image.h"
#include "backends/imgui_impl_opengl3.h"

namespace mainMenu
{
	GLFWwindow* window;

	bool menu_should_run = true;
	bool isQuitting = false;

	bool shouldRunMenu()
	{
		return menu_should_run;
	}

	void setWindowIcon()
	{
		LOG(INFO) << "Setting window icon for main menu." << std::endl;
		GLFWimage image;
		image.pixels = stbi_load("./images/windowIcons/traffic_tactician_round_icon.png", &image.width, &image.height, 0, 4); //rgba channels
		glfwSetWindowIcon(window, 1, &image);
		stbi_image_free(image.pixels);
	}

	// Slightly altered from: https://github.com/ocornut/imgui/issues/707#issuecomment-252413954
	void applyMainMenuStyling()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 5.3f;
		style.FrameRounding = 2.3f;
		style.ScrollbarRounding = 0;

		style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.87f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.69f, 1.00f, 0.53f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
	}

	void init()
	{
		LOG(INFO) << "Initializing main menu." << std::endl;

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(600, 48 * 3 + 24 * 2, "Main Menu",
			nullptr, nullptr);

		if (window == nullptr) {
			LOG(INFO) << "Could not create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(window);
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");

		applyMainMenuStyling();
		setWindowIcon();
	}

	void onShutdown()
	{
		LOG(INFO) << "Shutting down main menu..." << std::endl;
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(window);
		glfwTerminate();

		// Force close the entire process.
		if (isQuitting)
		{
			LOG(INFO) << "Shutting down entire process." << std::endl;
			exit(0);
		}
	}

	// Intermediary ImGui 'InputInt' function which corrects negative input to the last valid value.
	bool InputIntNonNegative(const char* label, int* v, int step, int step_fast, ImGuiInputTextFlags flags) {
		int originalValue = *v;

		// Call the original ImGui function
		bool valueChanged = ImGui::InputInt(label, v, step, step_fast, flags);

		// Check if the input value is negative and correct it to the original value
		if (*v < 0) {
			*v = originalValue;
			valueChanged = false;
		}

		return valueChanged;
	}

	int cameraDevice = 0;

	void applySettings()
	{
		LOG(INFO) << "Applying settings from main menu." << std::endl;
		cameraInputHandler::setCameraToUse(cameraDevice);
	}

	void runMenu() {
		init();

		LOG(INFO) << "Running main menu." << std::endl;
		
		while (shouldRunMenu())
		{
			glfwPollEvents();

			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// Make gui fullscreen.
			ImGui::SetNextWindowPos({ 0, 0 });
			ImGui::SetNextWindowSize({ ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y });

			ImGui::Begin("MainMenu", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
			{
				const ImVec2 buttonSize = { ImGui::GetIO().DisplaySize.x - 16, 48 };
				if (ImGui::Button("Start", buttonSize))
				{
					LOG(INFO) << "Pressed Start button." << std::endl;
					menu_should_run = false;
					SoundHandler::getInstance().playSoundSnippet("sounds/menu_click.wav");
				}

				InputIntNonNegative("Camera device", &cameraDevice, 1, 1, 0);

				if (ImGui::Button("Open instruction video", buttonSize))
				{
					LOG(INFO) << "Pressed Open Instruction Video button." << std::endl;

					std::filesystem::path videoFile = L"./videos/instructionVideo.mp4";

					std::filesystem::path a = "." / videoFile;

					LOG(INFO) << "Trying to open video at " << a.string() << "." << std::endl;
					ShellExecute(0, 0, a.c_str(), 0, 0, SW_SHOW);
					SoundHandler::getInstance().playSoundSnippet("sounds/menu_click.wav");
				}

				if (ImGui::Button("Quit", buttonSize))
				{
					LOG(INFO) << "Pressed Quit button." << std::endl;
					menu_should_run = false;
					isQuitting = true;
					SoundHandler::getInstance().playSoundSnippet("sounds/menu_click.wav");
				}
			}
			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
		}

		applySettings();
		onShutdown();
	}
}
