/*********************************************************************
 * \file   MainMenuBar.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   February 2023
 *********************************************************************/
#include "MainMenuBar.h"

#include <imgui/imgui.h>

void MainMenuBar::onGui()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("Save");

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            // ImGui::MenuItem("Save");

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Windows"))
        {
            ImGui::MenuItem("ControlPanel", "ShowControlPanel", &m_showControlPanel);

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
