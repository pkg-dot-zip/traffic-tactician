#include <ControllerComponent.h>
#include <RouteComponent.h>
#include <Scene.h>
#include <PoseFrame.h>

#include <opencv2/core.hpp>    // Basis OpenCV types zoals cv::Mat
#include <opencv2/opencv.hpp>

#include "easylogging++.h"
#include "imgui.h"
namespace statusHandler
{
    // ChatGPT method
    GLuint cvMatToTexture(const cv::Mat& mat)
    {
        // Genereer een OpenGL-textuur-ID
        GLuint textureID;
        glGenTextures(1, &textureID);
        // Bind de textureID
        glBindTexture(GL_TEXTURE_2D, textureID);
        // Stel de textureparameters in
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Stel de texture-inhoud in
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, mat.ptr());
        // Ontkoppel de texture
        glBindTexture(GL_TEXTURE_2D, 0);
        return textureID;
    }

    void showStatus(const std::shared_ptr<Simulation>& sim)
    {
        Scene::OverlayData data = sim->scene->data;

        // Eerste venster bovenaan voor score en bord
        if (ImGui::Begin("Status", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoScrollWithMouse))
        {
            // Toon het huidige verkeersbord
            ImGui::Image((void*)(intptr_t)*data.currentSignTexture, ImVec2(65, 65), { 0,1 }, { 1,0 }); // Display the icon
            ImGui::SameLine(); // Blijf op dezelfde regel

            // Haal de vensterbreedte op voor het "Status"-venster
            const float windowWidth = ImGui::GetWindowWidth();
            const int score = data.points;
            const float textWidth = ImGui::CalcTextSize(std::to_string(score).c_str()).x; // Breedte van de tekst

            // Stel de cursorpositie in voor het scorelogo en scoretekst
            ImGui::SetCursorPosX(windowWidth - 31.0f - textWidth - 20.0f); // 31.0f is de breedte van de scoreLogo-afbeelding
            ImGui::Image((void*)(intptr_t)data.textures["scoreLogo"], ImVec2(31, 31), { 0,1 }, { 1,0 }); // Display the icon
            ImGui::SameLine(); // Blijf op dezelfde regel

            // Toon de score
            ImGui::Text("%d", score); // Display the score

            // Toon voortgangsbalk indien de auto stilstaat
            if (!sim->scene->currentCarObject.expired())
            {
                const std::shared_ptr<GameObject> car = sim->scene->currentCarObject.lock();
                if (car->getComponent<RouteComponent>().has_value())
                {
                    if (car->getComponent<RouteComponent>().value()->state == RouteComponent::RouteState::Idle)
                    {
                        // Start de timer
                        car->getComponent<ControllerComponent>().value()->timer->toggleTimer(true);

                        char overlay[32];
                        sprintf_s(overlay, "%.2f s", data.remainingTime);
                        ImGui::ProgressBar(data.progress, ImVec2(-1.0f, 0.0f), overlay); // Voortgangsbalk over de volledige breedte
                    }
                }
                else
                {
                    LOG(ERROR) << "Error: Can not update UI when no ControllerComponent can be found." << std::endl;
                    throw std::exception("Error: Can not update UI when no ControllerComponent can be found.");
                }
            }

            ImGui::End(); // Sluit het eerste venster
        }

        // Tweede venster rechtsonder voor het stopteken
        constexpr float newImageWidth = 280.0f;
        constexpr float newImageHeight = 230.0f; 
        const ImVec2 windowSize(newImageWidth + 20.0f, newImageHeight + 20.0f);

        // Positie voor het venster instellen (rechtsonder)
        ImGui::SetNextWindowPos(ImVec2(1600 - windowSize.x, 900 - windowSize.y));
         
        // Venstergrootte instellen
        ImGui::SetNextWindowSize(windowSize);

        if (ImGui::Begin("userScreen", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoScrollWithMouse))
        {
            // Stel de cursorpositie in voor de afbeelding binnen het venster
            ImGui::SetCursorPos(ImVec2(10.0f, 10.0f)); // Marge binnen het venster

            GLuint textureID = cvMatToTexture(PoseFrame::getInstance().getFrame());
            ImGui::Image((void*)(intptr_t) textureID, ImVec2(newImageWidth, newImageHeight), ImVec2(1, 0), ImVec2(0, 1));

            ImGui::End(); // Sluit het tweede venster
        }
    }
}
