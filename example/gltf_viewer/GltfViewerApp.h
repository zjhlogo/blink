/*********************************************************************
 * \file   GltfViewerApp.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/24/2022
 *********************************************************************/
#pragma once

#include <blink/app.h>
#include <blink/utils/GltfUtil.h>
#include <foundation/PathParser.h>
#include <guis/IGuiWindow.h>
#include <render_vulkan/VulkanBase.h>

namespace blink
{
    class VulkanTexture;
}

class GltfViewerApp : public blink::IApp, public IGuiWindow
{
public:
    enum class Category
    {
        Unknown,
        Scene,
        Node,
        Mesh,
        Material,
        Texture,
    };

    struct ImGuiTextureInfo
    {
        blink::tstring path;
        blink::VulkanTexture* vulkanTexture;
        VkDescriptorSet ds;
    };

public:
    virtual void onGui() override;

    bool loadModel(const blink::tstring& modelFilePath);
    void unloadModel();

protected:
    bool initializeLogicalSystems() override;
    bool initializeRenderSystems() override;

private:
    void DrawHierarchyWindow();
    void DrawPropertyWindow();
    void DrawSceneHierarchy(const tinygltf::Model& model, int sceneIndex);
    void DrawNode(const tinygltf::Model& model, int nodeIndex);
    void DrawMesh(const tinygltf::Model& model, int meshIndex);
    void DrawMaterial(const tinygltf::Model& model, int materialIndex);
    void DrawTexture(const tinygltf::Model& model, int textureIndex);
    void DrawSceneProperty(const tinygltf::Model& model, int sceneIndex);
    void DrawNodeProperty(const tinygltf::Model& model, int nodeIndex);
    void DrawMeshProperty(const tinygltf::Model& model, int meshIndex);
    void DrawMaterialProperty(const tinygltf::Model& model, int materialIndex);
    void DrawTextureProperty(const tinygltf::Model& model, int textureIndex);

    void DrawComponentType(const char* label, int componentType);

    const ImGuiTextureInfo* GetOrLoadTextureFromFile(const blink::tstring& path);

private:
    tinygltf::Model m_model;
    blink::PathParser m_modelPath;

    Category m_selCategory{Category::Unknown};
    int m_selIndex{-1};

    std::map<blink::tstring, ImGuiTextureInfo*> m_textureMap;
};
