
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
#include <guis/IGuiWindow.h>

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

public:
    GltfViewerApp() = default;
    virtual ~GltfViewerApp() = default;

    virtual bool initialize() override;
    virtual void terminate() override;

    virtual void renderUi() override;

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

private:
    tinygltf::Model m_model;
    Category m_selCategory{Category::Unknown};
    int m_selIndex{-1};
};
