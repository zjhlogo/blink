/*********************************************************************
 * \file   ImguiExtension.h
 * \brief
 *
 * \author zjhlogo
 * \date   August 2022
 *********************************************************************/

#pragma once

#include <core/resources/ITexture.h>
#include <foundation/BaseTypes.h>
#include <imgui/imgui.h>
#include <vulkan/vulkan.h>

#include <map>
#include <memory>

namespace ImGui
{
    bool ReadOnlyText(const char* label,
                      const std::string* str,
                      ImGuiInputTextFlags flags = 0,
                      ImGuiInputTextCallback callback = nullptr,
                      void* userData = nullptr);

    // Helper to display a little (?) mark which shows a tooltip when hovered.
    // In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)
    void HelpMarker(const char* desc);

    void DrawComponentType(const char* label, int componentType);
    void DrawPrimitiveMode(const char* label, int primitiveMode);

} // namespace ImGui

class ImGuiTextureInfo
{
public:
    ImGuiTextureInfo(const ImGuiTextureInfo&) = delete;
    ImGuiTextureInfo(ImGuiTextureInfo&&) = delete;
    ImGuiTextureInfo& operator=(const ImGuiTextureInfo&) = delete;
    ImGuiTextureInfo& operator=(ImGuiTextureInfo&&) = delete;

    const blink::tstring& getPath() const { return m_path; }
    blink::ITexture* getTexture() const { return m_texture; }
    VkDescriptorSet getDs() const { return m_ds; }

protected:
    ImGuiTextureInfo(){};
    ~ImGuiTextureInfo(){};

    blink::tstring m_path;
    blink::ITexture* m_texture{};
    VkDescriptorSet m_ds{};
};

class ImGuiExt
{
public:
    static std::shared_ptr<ImGuiTextureInfo> createTexture(const blink::tstring& m_path);
    static void recycleUnusedTexture();

public:
    static float lastFrameMouseWheel;
    static float lastFrameMouseWheelH;

private:
    static std::map<blink::tstring, std::shared_ptr<ImGuiTextureInfo>> m_cachedTexInfo;
};
