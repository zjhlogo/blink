
/*********************************************************************
 * \file   GltfViewerApp.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   04/24/2022
 *********************************************************************/
#include "GltfViewerApp.h"

#include <blink/blink.h>
#include <flecs/flecs_os_api_stdcpp.h>
#include <fmt/format.h>
#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <render_systems/ImguiRenderSystem.h>
#include <render_systems/SceneRenderSystem.h>

bool GltfViewerApp::initialize()
{
    stdcpp_set_os_api();

    // add render systems
    addRenderSystem(new SceneRenderSystem(this));
    auto guiRenderSystem = new ImguiRenderSystem();
    addRenderSystem(guiRenderSystem);
    if (!initializeRenderSystems()) return false;

    guiRenderSystem->addWindow(this);

    if (!blink::GltfUtil::loadFromFile(m_model, "resource/models/damaged_helmet/DamagedHelmet.gltf")) return false;

    return true;
}

void GltfViewerApp::terminate()
{
    terminateRenderSystems();
    m_ecsWorld.terminate();
}

void GltfViewerApp::renderUi()
{
    DrawHierarchyWindow();
    DrawPropertyWindow();
}

void GltfViewerApp::DrawHierarchyWindow()
{
    // draw hierarchy
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), 0, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::CollapsingHeader("Summory", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::LabelText("version", m_model.asset.version.c_str());
        ImGui::LabelText("generator", m_model.asset.generator.c_str());
        ImGui::LabelText("minVersion", m_model.asset.minVersion.c_str());
        ImGui::LabelText("copyright", m_model.asset.copyright.c_str());
    }

    if (ImGui::CollapsingHeader("Scene List"))
    {
        for (int i = 0; i < m_model.scenes.size(); ++i)
        {
            DrawSceneHierarchy(m_model, i);
        }
    }

    if (ImGui::CollapsingHeader("Mesh List"))
    {
        for (int i = 0; i < m_model.meshes.size(); ++i)
        {
            DrawMesh(m_model, i);
        }
    }

    if (ImGui::CollapsingHeader("Material List"))
    {
        for (int i = 0; i < m_model.materials.size(); ++i)
        {
            DrawMaterial(m_model, i);
        }
    }

    if (ImGui::CollapsingHeader("Texture List"))
    {
        for (int i = 0; i < m_model.textures.size(); ++i)
        {
            DrawTexture(m_model, i);
        }
    }

    ImGui::End();
}

void GltfViewerApp::DrawPropertyWindow()
{
    // draw property
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x, 0.0f), 0, ImVec2(1.0f, 0.0f));
    ImGui::Begin("Property", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::BeginDisabled();

    switch (m_selCategory)
    {
    case GltfViewerApp::Category::Scene:
        DrawSceneProperty(m_model, m_selIndex);
        break;
    case GltfViewerApp::Category::Node:
        DrawNodeProperty(m_model, m_selIndex);
        break;
    case GltfViewerApp::Category::Mesh:
        DrawMeshProperty(m_model, m_selIndex);
        break;
    case GltfViewerApp::Category::Material:
        DrawMaterialProperty(m_model, m_selIndex);
        break;
    case GltfViewerApp::Category::Texture:
        DrawTextureProperty(m_model, m_selIndex);
        break;
    default:
        break;
    }

    ImGui::EndDisabled();
    ImGui::End();
}

void GltfViewerApp::DrawSceneHierarchy(const tinygltf::Model& model, int sceneIndex)
{
    const auto& scene = model.scenes[sceneIndex];

    ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_SpanAvailWidth;
    if (m_selCategory == Category::Scene && m_selIndex == sceneIndex)
    {
        flag |= ImGuiTreeNodeFlags_Selected;
    }

    blink::tstring sceneName;
    if (!scene.name.empty())
    {
        sceneName = fmt::format("{}##scene{}", scene.name, sceneIndex);
    }
    else
    {
        sceneName = fmt::format("scene {}##scene{}", sceneIndex, sceneIndex);
    }

    bool nodeOpened = ImGui::TreeNodeEx(sceneName.c_str(), flag);
    if (ImGui::IsItemClicked())
    {
        m_selCategory = Category::Scene;
        m_selIndex = sceneIndex;
    }

    if (nodeOpened)
    {
        for (auto nodeIndex : scene.nodes)
        {
            DrawNode(model, nodeIndex);
        }
        ImGui::TreePop();
    }
}

void GltfViewerApp::DrawNode(const tinygltf::Model& model, int nodeIndex)
{
    const auto& node = model.nodes[nodeIndex];

    ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_SpanAvailWidth;
    if (m_selCategory == Category::Node && m_selIndex == nodeIndex)
    {
        flag |= ImGuiTreeNodeFlags_Selected;
    }

    if (node.children.empty())
    {
        flag |= (ImGuiTreeNodeFlags_Leaf);
    }

    blink::tstring nodeName;
    if (!node.name.empty())
    {
        nodeName = fmt::format("{}##node{}", node.name, nodeIndex);
    }
    else
    {
        nodeName = fmt::format("node {}##node{}", nodeIndex, nodeIndex);
    }

    bool nodeOpened = ImGui::TreeNodeEx(nodeName.c_str(), flag);
    if (ImGui::IsItemClicked())
    {
        m_selCategory = Category::Node;
        m_selIndex = nodeIndex;
    }

    if (nodeOpened)
    {
        for (auto childNodeIndex : node.children)
        {
            DrawNode(model, childNodeIndex);
        }
        ImGui::TreePop();
    }
}

void GltfViewerApp::DrawMesh(const tinygltf::Model& model, int meshIndex)
{
    const auto& mesh = model.meshes[meshIndex];

    blink::tstring meshName;
    if (!mesh.name.empty())
    {
        meshName = fmt::format("{}##mesh{}", mesh.name, meshIndex);
    }
    else
    {
        meshName = fmt::format("mesh {}##mesh{}", meshIndex, meshIndex);
    }

    bool selected = (m_selCategory == Category::Mesh && m_selIndex == meshIndex);
    if (ImGui::Selectable(meshName.c_str(), selected))
    {
        m_selCategory = Category::Mesh;
        m_selIndex = meshIndex;
    }
}

void GltfViewerApp::DrawMaterial(const tinygltf::Model& model, int materialIndex)
{
    const auto& material = model.materials[materialIndex];

    blink::tstring materialName;
    if (!material.name.empty())
    {
        materialName = fmt::format("{}##material{}", material.name, materialIndex);
    }
    else
    {
        materialName = fmt::format("material {}##material{}", materialIndex, materialIndex);
    }

    bool selected = (m_selCategory == Category::Material && m_selIndex == materialIndex);
    if (ImGui::Selectable(materialName.c_str(), selected))
    {
        m_selCategory = Category::Material;
        m_selIndex = materialIndex;
    }
}

void GltfViewerApp::DrawTexture(const tinygltf::Model& model, int textureIndex)
{
    const auto& texture = model.textures[textureIndex];

    blink::tstring textureName;
    if (!texture.name.empty())
    {
        textureName = fmt::format("{}##texture{}", texture.name, textureIndex);
    }
    else
    {
        textureName = fmt::format("texture {}##texture{}", textureIndex, textureIndex);
    }

    bool selected = (m_selCategory == Category::Texture && m_selIndex == textureIndex);
    if (ImGui::Selectable(textureName.c_str(), selected))
    {
        m_selCategory = Category::Texture;
        m_selIndex = textureIndex;
    }
}

void GltfViewerApp::DrawSceneProperty(const tinygltf::Model& model, int sceneIndex)
{
    const auto& scene = model.scenes[sceneIndex];

    auto name = scene.name;
    ImGui::InputText("name", &name);
}

void GltfViewerApp::DrawNodeProperty(const tinygltf::Model& model, int nodeIndex)
{
    const auto& node = model.nodes[nodeIndex];

    auto name = node.name;
    ImGui::InputText("name", &name);

    int camera = node.camera;
    ImGui::InputInt("camera", &camera);

    int mesh = node.mesh;
    ImGui::InputInt("mesh", &mesh);

    int skin = node.skin;
    ImGui::InputInt("skin", &skin);

    if (!node.translation.empty())
    {
        float position[3]{0.0f, 0.0f, 0.0f};
        position[0] = static_cast<float>(node.translation[0]);
        position[1] = static_cast<float>(node.translation[1]);
        position[2] = static_cast<float>(node.translation[2]);
        ImGui::InputFloat3("position", position);
    }

    if (!node.rotation.empty())
    {
        float rotation[4]{0.0f, 0.0f, 0.0f, 1.0f};
        rotation[0] = static_cast<float>(node.rotation[0]);
        rotation[1] = static_cast<float>(node.rotation[1]);
        rotation[2] = static_cast<float>(node.rotation[2]);
        rotation[3] = static_cast<float>(node.rotation[3]);
        ImGui::InputFloat4("rotation", rotation);
    }

    if (!node.scale.empty())
    {
        float scale[3]{1.0f, 1.0f, 1.0f};
        scale[0] = static_cast<float>(node.scale[0]);
        scale[1] = static_cast<float>(node.scale[1]);
        scale[2] = static_cast<float>(node.scale[2]);
        ImGui::InputFloat3("scale", scale);
    }
}

void GltfViewerApp::DrawMeshProperty(const tinygltf::Model& model, int meshIndex)
{
    const auto& mesh = model.meshes[meshIndex];

    auto name = mesh.name;
    ImGui::InputText("name", &name);

    for (int i = 0; i < mesh.primitives.size(); ++i)
    {
        const auto& primitive = mesh.primitives[i];
        if (ImGui::CollapsingHeader(fmt::format("primitive {}", i).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
        {
            for (auto kvp : primitive.attributes)
            {
                auto attribute = kvp.first;
                ImGui::InputText("attribute", &attribute);
            }
        }
    }
}

void GltfViewerApp::DrawMaterialProperty(const tinygltf::Model& model, int materialIndex)
{
    const auto& material = model.materials[materialIndex];

    auto name = material.name;
    ImGui::InputText("name", &name);

    auto alphaMode = material.alphaMode;
    ImGui::InputText("alphaMode", &alphaMode);

    double alphaCutoff = material.alphaCutoff;
    ImGui::InputDouble("alphaCutoff", &alphaCutoff);

    bool doubleSided = material.doubleSided;
    ImGui::Checkbox("doubleSided", &doubleSided);

    if (!material.emissiveFactor.empty())
    {
        float emissiveFactor[3]{0.0f, 0.0f, 0.0f};
        emissiveFactor[0] = static_cast<float>(material.emissiveFactor[0]);
        emissiveFactor[1] = static_cast<float>(material.emissiveFactor[1]);
        emissiveFactor[2] = static_cast<float>(material.emissiveFactor[2]);
        ImGui::InputFloat3("emissiveFactor", emissiveFactor);
    }

    if (ImGui::CollapsingHeader("pbrMetallicRoughness", ImGuiTreeNodeFlags_DefaultOpen))
    {
        float baseColorFactor[4]{1.0f, 1.0f, 1.0f, 1.0f};
        baseColorFactor[0] = static_cast<float>(material.pbrMetallicRoughness.baseColorFactor[0]);
        baseColorFactor[1] = static_cast<float>(material.pbrMetallicRoughness.baseColorFactor[1]);
        baseColorFactor[2] = static_cast<float>(material.pbrMetallicRoughness.baseColorFactor[2]);
        baseColorFactor[3] = static_cast<float>(material.pbrMetallicRoughness.baseColorFactor[3]);
        ImGui::InputFloat4("baseColorFactor", baseColorFactor);

        double metallicFactor = material.pbrMetallicRoughness.metallicFactor;
        ImGui::InputDouble("metallicFactor", &metallicFactor);

        double roughnessFactor = material.pbrMetallicRoughness.roughnessFactor;
        ImGui::InputDouble("roughnessFactor", &roughnessFactor);
    }
}

void GltfViewerApp::DrawTextureProperty(const tinygltf::Model& model, int textureIndex)
{
    const auto& texture = model.textures[textureIndex];

    auto name = texture.name;
    ImGui::InputText("name", &name);

    int samplerIndex = texture.sampler;
    if (samplerIndex == -1)
    {
        ImGui::InputInt("sampler", &samplerIndex);
    }
    else
    {
        if (ImGui::CollapsingHeader(fmt::format("sampler {}", samplerIndex).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
        {
            const auto& sampler = model.samplers[samplerIndex];

            int minFilter = sampler.minFilter;
            ImGui::InputInt("minFilter", &minFilter);

            int magFilter = sampler.magFilter;
            ImGui::InputInt("magFilter", &magFilter);

            int wrapS = sampler.wrapS;
            ImGui::InputInt("wrapS", &wrapS);

            int wrapT = sampler.wrapT;
            ImGui::InputInt("wrapT", &wrapT);
        }
    }

    int source = texture.source;
    ImGui::InputInt("source", &source);
}

int main(int argc, char** argv)
{
    GltfViewerApp app;
    return blink::run(app);
}
