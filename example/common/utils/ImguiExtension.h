/*********************************************************************
 * \file   ImguiExtension.h
 * \brief
 *
 * \author zjhlogo
 * \date   August 2022
 *********************************************************************/

#pragma once

#include <imgui/imgui.h>

#include <string>

namespace ImGui
{
    bool ReadOnlyText(const char* label,
                      const std::string* str,
                      ImGuiInputTextFlags flags = 0,
                      ImGuiInputTextCallback callback = nullptr,
                      void* userData = nullptr);
} // namespace ImGui
