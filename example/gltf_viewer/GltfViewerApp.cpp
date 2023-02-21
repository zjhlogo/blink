/*********************************************************************
 * \file   GltfViewerApp.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   04/24/2022
 *********************************************************************/
#include "GltfViewerApp.h"

#include <blink/blink.h>
#include <blink/components/Components.h>
#include <core/components/Components.h>
#include <core/modules/IResourceModule.h>
#include <fmt/format.h>
#include <render_systems/ImguiRenderSystem.h>
#include <utils/ImguiExtension.h>
#include <utils/SceneEntityUtil.h>

bool GltfViewerApp::initializeLogicalSystems()
{
    SceneEntityUtil::initializeCommonLogicalSystems(this);
    return true;
}

bool GltfViewerApp::initializeRenderSystems()
{
    auto guiRenderSystem = addRenderSystem(new ImguiRenderSystem());
    guiRenderSystem->addWindow(this);
    return true;
}

void GltfViewerApp::onGui()
{
    // ImGui::ShowDemoWindow();
    DrawHierarchyWindow();
    DrawPropertyWindow();
}

bool GltfViewerApp::initialize()
{
    if (!IApp::initialize())
    {
        return false;
    }

    if (!SceneEntityUtil::initializeCommonSceneEntities(getEcsWorld()))
    {
        return false;
    }

    if (!loadModel("/models/damaged_helmet/DamagedHelmet.gltf"))
    {
        return false;
    }

    auto& world = getEcsWorld().getWorld();
    auto resModule = blink::getResourceModule();

    // model
    auto material = resModule->createMaterial("/models/damaged_helmet/DamagedHelmet.mtl");
    auto entityModel = world.entity("model");
    entityModel.set<blink::Position>({glm::zero<glm::vec3>()});
    entityModel.set<blink::Rotation>({glm::identity<glm::quat>()});
    entityModel.set<blink::StaticModel>({m_modelGeometry, material});
    entityModel.set<blink::Renderable>({blink::RenderLayers::NORMAL});

    return true;
}

void GltfViewerApp::terminate()
{
    unloadModel();
    IApp::terminate();
}

bool GltfViewerApp::loadModel(const blink::tstring& modelFilePath)
{
    if (m_modelPath.isValid())
    {
        return false;
    }
    m_modelPath.parse(modelFilePath);

    if (m_modelGeometry != nullptr)
    {
        return false;
    }

    m_modelGeometry = m_meshBuilder.loadModel(modelFilePath).build();
    if (!m_modelGeometry)
    {
        return false;
    }

    return true;
}

void GltfViewerApp::unloadModel()
{
    SAFE_RELEASE(m_modelGeometry);
    m_meshBuilder.reset();

    ImGuiExt::recycleUnusedTexture();

    m_selCategory = Category::Unknown;
    m_selIndex = -1;
    m_modelPath.reset();
}

void GltfViewerApp::DrawHierarchyWindow()
{
    const auto& model = m_meshBuilder.getModel();

    // draw hierarchy
    ImGui::Begin("Hierarchy");

    if (ImGui::CollapsingHeader("Summory", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::ReadOnlyText("version", &model.asset.version);
        ImGui::ReadOnlyText("generator", &model.asset.generator);
        ImGui::ReadOnlyText("minVersion", &model.asset.minVersion);
        ImGui::ReadOnlyText("copyright", &model.asset.copyright);
    }

    if (ImGui::CollapsingHeader("Scene List"))
    {
        for (int i = 0; i < model.scenes.size(); ++i)
        {
            DrawSceneHierarchy(model, i);
        }
    }

    if (ImGui::CollapsingHeader("Mesh List"))
    {
        for (int i = 0; i < model.meshes.size(); ++i)
        {
            DrawMesh(model, i);
        }
    }

    if (ImGui::CollapsingHeader("Material List"))
    {
        for (int i = 0; i < model.materials.size(); ++i)
        {
            DrawMaterial(model, i);
        }
    }

    if (ImGui::CollapsingHeader("Texture List"))
    {
        for (int i = 0; i < model.textures.size(); ++i)
        {
            DrawTexture(model, i);
        }
    }

    ImGui::End();
}

void GltfViewerApp::DrawPropertyWindow()
{
    const auto& model = m_meshBuilder.getModel();

    // draw property
    ImGui::Begin("Property");

    switch (m_selCategory)
    {
    case GltfViewerApp::Category::Scene:
        DrawSceneProperty(model, m_selIndex);
        break;
    case GltfViewerApp::Category::Node:
        DrawNodeProperty(model, m_selIndex);
        break;
    case GltfViewerApp::Category::Mesh:
        DrawMeshProperty(model, m_selIndex);
        break;
    case GltfViewerApp::Category::Material:
        DrawMaterialProperty(model, m_selIndex);
        break;
    case GltfViewerApp::Category::Texture:
        DrawTextureProperty(model, m_selIndex);
        break;
    default:
        break;
    }

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
    const auto& m_texture = model.textures[textureIndex];

    blink::tstring textureName;
    if (!m_texture.name.empty())
    {
        textureName = fmt::format("{}##texture{}", m_texture.name, textureIndex);
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
    ImGui::ReadOnlyText("name##scene", &scene.name);

    int numNodes = static_cast<int>(scene.nodes.size());
    ImGui::InputInt("num_nodes", &numNodes);
}

void GltfViewerApp::DrawNodeProperty(const tinygltf::Model& model, int nodeIndex)
{
    const auto& node = model.nodes[nodeIndex];

    ImGui::ReadOnlyText("name##node", &node.name, ImGuiInputTextFlags_ReadOnly);

    int camera = node.camera;
    ImGui::InputInt("camera", &camera, ImGuiInputTextFlags_ReadOnly);

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

    ImGui::ReadOnlyText("name##mesh", &mesh.name, ImGuiInputTextFlags_ReadOnly);

    for (int i = 0; i < mesh.primitives.size(); ++i)
    {
        const auto& primitive = mesh.primitives[i];
        if (ImGui::CollapsingHeader(fmt::format("primitive {}", i).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
        {
            for (auto kvp : primitive.attributes)
            {
                ImGui::ReadOnlyText(fmt::format("attribute {}", kvp.second).c_str(), &kvp.first, ImGuiInputTextFlags_ReadOnly);
            }
        }

        int materialIndex = primitive.material;
        ImGui::InputInt("material", &materialIndex);

        int indices = primitive.indices;
        ImGui::InputInt("indices", &indices);

        ImGui::DrawPrimitiveMode("mode", primitive.mode);

        int numTargets = static_cast<int>(primitive.targets.size());
        ImGui::InputInt("targets", &numTargets);
    }
}

void GltfViewerApp::DrawMaterialProperty(const tinygltf::Model& model, int materialIndex)
{
    const auto& material = model.materials[materialIndex];

    ImGui::ReadOnlyText("name##material", &material.name, ImGuiInputTextFlags_ReadOnly);
    ImGui::ReadOnlyText("alphaMode", &material.alphaMode, ImGuiInputTextFlags_ReadOnly);

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

    if (ImGui::CollapsingHeader("normalTexture"))
    {
        const auto& image = model.images[material.normalTexture.index];
        auto texturePath = m_modelPath.replaceFileName(image.uri);
        auto texInfo = ImGuiExt::createTexture(texturePath);
        if (texInfo)
        {
            ImGui::ImageButton("image##normal", texInfo->getDs(), {128, 128});
        }

        int texCoord = material.normalTexture.texCoord;
        ImGui::InputInt("texCoord", &texCoord);

        double scale = material.normalTexture.scale;
        ImGui::InputDouble("scale", &scale);
        ImGui::HelpMarker("scaledNormal = normalize((<sampled normal texture value> * 2.0 - 1.0) * vec3(<normal scale>, <normal scale>, 1.0))");
    }

    if (ImGui::CollapsingHeader("occlusionTexture"))
    {
        const auto& image = model.images[material.occlusionTexture.index];
        auto texturePath = m_modelPath.replaceFileName(image.uri);
        auto texInfo = ImGuiExt::createTexture(texturePath);
        if (texInfo)
        {
            ImGui::ImageButton("image##occlusion", texInfo->getDs(), {128, 128});
        }

        int texCoord = material.occlusionTexture.texCoord;
        ImGui::InputInt("texCoord", &texCoord);

        double strength = material.occlusionTexture.strength;
        ImGui::InputDouble("strength", &strength);
        ImGui::HelpMarker("occludedColor = lerp(color, color * <sampled occlusion texture value>, <occlusion strength>)");
    }

    if (ImGui::CollapsingHeader("emissiveTexture"))
    {
        const auto& image = model.images[material.emissiveTexture.index];
        auto texturePath = m_modelPath.replaceFileName(image.uri);
        auto texInfo = ImGuiExt::createTexture(texturePath);
        if (texInfo)
        {
            ImGui::ImageButton("image##emissive", texInfo->getDs(), {128, 128});
        }

        int texCoord = material.emissiveTexture.texCoord;
        ImGui::InputInt("texCoord", &texCoord);
    }
}

void GltfViewerApp::DrawTextureProperty(const tinygltf::Model& model, int textureIndex)
{
    const auto& m_texture = model.textures[textureIndex];

    ImGui::ReadOnlyText("name##texture", &m_texture.name, ImGuiInputTextFlags_ReadOnly);

    int samplerIndex = m_texture.sampler;
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

    if (m_texture.source != -1)
    {
        const auto image = model.images[m_texture.source];

        if (ImGui::CollapsingHeader(fmt::format("image {}", m_texture.source).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::ReadOnlyText("name##image", &image.name, ImGuiInputTextFlags_ReadOnly);

            auto width = image.width;
            ImGui::InputInt("width", &width);

            auto height = image.height;
            ImGui::InputInt("height", &height);

            auto component = image.component;
            ImGui::InputInt("component", &component);

            auto bits = image.bits;
            ImGui::InputInt("bits", &bits);

            ImGui::DrawComponentType("pixel_type", image.pixel_type);

            ImGui::ReadOnlyText("mineType", &image.mimeType, ImGuiInputTextFlags_ReadOnly);
            ImGui::ReadOnlyText("uri", &image.uri, ImGuiInputTextFlags_ReadOnly);

            auto texturePath = m_modelPath.replaceFileName(image.uri);
            auto texInfo = ImGuiExt::createTexture(texturePath);
            if (texInfo)
            {
                ImGui::ImageButton("image", texInfo->getDs(), {128, 128});
            }
        }
    }
}

int main(int argc, char** argv)
{
    GltfViewerApp app;
    return blink::run(argc, argv, app);
}
