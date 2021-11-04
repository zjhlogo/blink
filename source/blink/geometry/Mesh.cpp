/**

    @file      Mesh.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/

#include "Mesh.h"

#include <foundation/File.h>
#include <foundation/Log.h>
#include <render_vulkan/VulkanBuffer.h>
#include <render_vulkan/VulkanCommandBuffer.h>

#include <unordered_map>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include <tinygltf/tiny_gltf.h>

namespace blink
{
    Mesh::Mesh(VulkanLogicalDevice& logicalDevice, VulkanCommandPool& commandPool)
        : IGeometry(logicalDevice, commandPool)
    {
    }

    bool Mesh::loadFromFile(const tstring& filePath)
    {
        tstring fileContent;
        if (!File::readFileIntoString(fileContent, filePath))
        {
            LOGE("Open file failed {0}", filePath);
            return false;
        }

        tinygltf::Model model;
        tinygltf::TinyGLTF loader;
        tstring err;
        tstring warn;

        bool ret = loader.LoadASCIIFromString(&model, &err, &warn, fileContent.data(), static_cast<unsigned int>(fileContent.length()), EMPTY_STRING);
        if (!warn.empty())
        {
            LOGW(warn);
        }

        if (!err.empty())
        {
            LOGE(err);
        }

        if (!ret)
        {
            LOGE("Failed to parse glTF file {0}", filePath);
            return false;
        }

        if (model.meshes.empty())
        {
            LOGE("Empty mesh {0}", filePath);
            return false;
        }
        const auto& mesh = model.meshes[0];

        if (mesh.primitives.empty())
        {
            LOGE("Empty primitive {0}", filePath);
            return false;
        }
        const auto& primitive = mesh.primitives[0];

        if (primitive.attributes.size() != 3)
        {
            LOGE("Unsupport primitive attributes, size must be 3. {0}", filePath);
            return false;
        }
        auto itPos = primitive.attributes.find("POSITION");
        auto itNormal = primitive.attributes.find("NORMAL");
        auto itUv0 = primitive.attributes.find("TEXCOORD_0");
        if (itPos == primitive.attributes.end() || itNormal == primitive.attributes.end() || itUv0 == primitive.attributes.end())
        {
            LOGE("Unsupport primitive attributes, must be POSITION, NORMAL and TEXCOORD_0. {0}", filePath);
            return false;
        }

        const auto& accessorPos = model.accessors[itPos->second];
        const auto& accessorNormal = model.accessors[itNormal->second];
        const auto& accessorUv0 = model.accessors[itUv0->second];
        const auto& accessorIndices = model.accessors[primitive.indices];

        const auto& buffViewPos = model.bufferViews[accessorPos.bufferView];
        const auto& buffViewNormal = model.bufferViews[accessorNormal.bufferView];
        const auto& buffViewUv0 = model.bufferViews[accessorUv0.bufferView];
        const auto& buffViewIndices = model.bufferViews[accessorIndices.bufferView];

        if (buffViewPos.buffer != buffViewNormal.buffer || buffViewPos.buffer != buffViewUv0.buffer || buffViewPos.buffer != buffViewIndices.buffer)
        {
            LOGE("Only single buffer currently supported. {0}", filePath);
            return false;
        }

        const auto& buffer = model.buffers[buffViewPos.buffer];

        return false;
        // auto& attrib = reader.GetAttrib();
        // auto& shapes = reader.GetShapes();
        // auto& materials = reader.GetMaterials();

        // std::vector<VertexPosColorUv1> vertices;
        // std::vector<uint16> indices;
        // std::unordered_map<VertexPosColorUv1, uint16> uniqueVertices;

        //// Loop over shapes
        // for (const auto& shape : shapes)
        //{
        //    for (const auto& index : shape.mesh.indices)
        //    {
        //        VertexPosColorUv1 vertex{{
        //                                     attrib.vertices[3 * index.vertex_index + 0],
        //                                     attrib.vertices[3 * index.vertex_index + 1],
        //                                     attrib.vertices[3 * index.vertex_index + 2],
        //                                 },
        //                                 {
        //                                     1.0f,
        //                                     1.0f,
        //                                     1.0f,
        //                                 },
        //                                 {
        //                                     attrib.texcoords[2 * index.texcoord_index + 0],
        //                                     1.0f - attrib.texcoords[2 * index.texcoord_index + 1],
        //                                 }};

        //        if (uniqueVertices.count(vertex) == 0)
        //        {
        //            uniqueVertices[vertex] = static_cast<uint16>(vertices.size());
        //            vertices.push_back(vertex);
        //        }

        //        indices.push_back(uniqueVertices[vertex]);
        //    }
        //}

        // return uploadData(vertices.data(),
        //                  static_cast<uint32>(vertices.size()),
        //                  sizeof(VertexPosColorUv1),
        //                  indices.data(),
        //                  static_cast<uint16>(indices.size()));
    }

} // namespace blink
