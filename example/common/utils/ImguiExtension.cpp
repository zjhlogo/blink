/*********************************************************************
 * \file   ImguiExtension.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   August 2022
 *********************************************************************/
#include "ImguiExtension.h"

#include <core/modules/IResModule.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <render_vulkan/VulkanImage.h>
#include <render_vulkan/resources/VulkanTexture.h>
#include <tinygltf/tiny_gltf.h>

namespace ImGui
{
    static const std::map<int, const char*> g_componentTypeMap = {{TINYGLTF_COMPONENT_TYPE_BYTE, "BYTE"},
                                                                  {TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE, "UNSIGNED_BYTE"},
                                                                  {TINYGLTF_COMPONENT_TYPE_SHORT, "SHORT"},
                                                                  {TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT, "UNSIGNED_SHORT"},
                                                                  {TINYGLTF_COMPONENT_TYPE_INT, "INT"},
                                                                  {TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT, "UNSIGNED_INT"},
                                                                  {TINYGLTF_COMPONENT_TYPE_FLOAT, "FLOAT"},
                                                                  {TINYGLTF_COMPONENT_TYPE_DOUBLE, "DOUBLE"}};

    static const std::map<int, const char*> g_primitiveModeMap = {{TINYGLTF_MODE_POINTS, "POINTS"},
                                                                  {TINYGLTF_MODE_LINE, "LINE"},
                                                                  {TINYGLTF_MODE_LINE_LOOP, "LINE_LOOP"},
                                                                  {TINYGLTF_MODE_LINE_STRIP, "LINE_STRIP"},
                                                                  {TINYGLTF_MODE_TRIANGLES, "TRIANGLES"},
                                                                  {TINYGLTF_MODE_TRIANGLE_STRIP, "TRIANGLE_STRIP"},
                                                                  {TINYGLTF_MODE_TRIANGLE_FAN, "TRIANGLE_FAN"}};

    struct ReadOnlyTextCallback_UserData
    {
        const std::string* str;
        ImGuiInputTextCallback chainCallback;
        void* chainCallbackUserData;
    };

    static int InputTextCallback(ImGuiInputTextCallbackData* data)
    {
        ReadOnlyTextCallback_UserData* user_data = (ReadOnlyTextCallback_UserData*)data->UserData;
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
        {
            IM_ASSERT(false);
        }
        else if (user_data->chainCallback)
        {
            // Forward to user callback, if any
            data->UserData = user_data->chainCallbackUserData;
            return user_data->chainCallback(data);
        }
        return 0;
    }

    bool ReadOnlyText(const char* label, const std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* userData)
    {
        flags |= ImGuiInputTextFlags_ReadOnly;

        ReadOnlyTextCallback_UserData cb_user_data;
        cb_user_data.str = str;
        cb_user_data.chainCallback = callback;
        cb_user_data.chainCallbackUserData = userData;
        return InputText(label, const_cast<char*>(str->c_str()), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
    }

    // Helper to display a little (?) mark which shows a tooltip when hovered.
    // In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)
    void HelpMarker(const char* desc)
    {
        ImGui::SameLine();
        ImGui::TextDisabled("(?)");
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
        {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(desc);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }

    static void DrawCombo(const char* label, int key, const std::map<int, const char*>& textMap)
    {
        auto it = textMap.find(key);

        if (ImGui::BeginCombo(label, it == textMap.end() ? "UNKNOWN" : it->second))
        {
            for (auto kvp : textMap)
            {
                bool isSelected = (kvp.first == key);
                ImGui::Selectable(kvp.second, isSelected);
                if (isSelected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    void DrawComponentType(const char* label, int componentType)
    {
        DrawCombo(label, componentType, g_componentTypeMap);
    }

    void DrawPrimitiveMode(const char* label, int primitiveMode)
    {
        DrawCombo(label, primitiveMode, g_primitiveModeMap);
    }
} // namespace ImGui

class ImGuiTextureInfoImpl : public ImGuiTextureInfo
{
public:
    ImGuiTextureInfoImpl(const blink::tstring& path)
    {
        // load texture
        auto resModule = blink::getResModule();
        m_texture = resModule->createTexture2d(path);
        if (!m_texture) return;

        auto vulkanTexture = dynamic_cast<blink::VulkanTexture*>(m_texture);
        m_ds = ImGui_ImplVulkan_AddTexture(vulkanTexture->getTextureSampler(),
                                           vulkanTexture->getTextureImage()->getImageView(),
                                           VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        m_path = path;
    }

    ~ImGuiTextureInfoImpl()
    {
        ImGui_ImplVulkan_RemoveTexture(m_ds);
        SAFE_RELEASE(m_texture);
    }
};

float ImGuiExt::lastFrameMouseWheel{};
float ImGuiExt::lastFrameMouseWheelH{};

std::map<blink::tstring, std::shared_ptr<ImGuiTextureInfo>> ImGuiExt::m_cachedTexInfo;

std::shared_ptr<ImGuiTextureInfo> ImGuiExt::createTexture(const blink::tstring& path)
{
    auto it = m_cachedTexInfo.find(path);
    if (it != m_cachedTexInfo.end()) return it->second;

    std::shared_ptr<ImGuiTextureInfo> texInfo = std::make_shared<ImGuiTextureInfoImpl>(path);
    if (!texInfo->getTexture()) return nullptr;

    m_cachedTexInfo.emplace(path, texInfo);
    return texInfo;
}

void ImGuiExt::recycleUnusedTexture()
{
    auto it = m_cachedTexInfo.begin();
    while (it != m_cachedTexInfo.end())
    {
        if (it->second.use_count() <= 1)
        {
            LOGI("recycling texture {}", it->first);
            m_cachedTexInfo.erase(it++);
        }
        else
        {
            ++it;
        }
    }
}
