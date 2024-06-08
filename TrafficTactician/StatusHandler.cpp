#include <ControllerComponent.h>

#include "easylogging++.h"
#include "imgui.h"
#include "RouteComponent.h"
#include "Scene.h"

void showStatus(const std::shared_ptr<Simulation>& sim)
{
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

		ImGui::Image((void*)(intptr_t)data.textures["scoreLogo"], ImVec2(31, 31), { 0,1 }, { 1,0 }); // Display the icon
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
}
