/*********************************************************************
 * \file   GltfViewerApp.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/24/2022
 *********************************************************************/
#pragma once

#include <blink/app.h>
#include <blink/geometries_builder/MeshBuilder.h>
#include <foundation/PathParser.h>
#include <common/guis/IGuiWindow.h>

namespace blink
{
    class IGeometry;
} // namespace blink

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
    virtual void onGui() override;

    bool loadModel(const blink::tstring& modelFilePath);
    void unloadModel();

protected:
    bool startup() override;
    void shutdown() override;

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

private:
    blink::PathParser m_modelPath;
    blink::MeshBuilder m_meshBuilder;
    blink::IGeometry* m_modelGeometry{};

    Category m_selCategory{Category::Unknown};
    int m_selIndex{-1};
};
