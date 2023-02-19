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

bool ImGui::ReadOnlyText(const char* label, const std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* userData)
{
    flags |= ImGuiInputTextFlags_ReadOnly;

    ReadOnlyTextCallback_UserData cb_user_data;
    cb_user_data.str = str;
    cb_user_data.chainCallback = callback;
    cb_user_data.chainCallbackUserData = userData;
    return InputText(label, const_cast<char*>(str->c_str()), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
}

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
