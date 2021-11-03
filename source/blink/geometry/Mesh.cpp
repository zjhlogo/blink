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
#include <render_vulkan/Types.h>
#include <render_vulkan/VulkanBuffer.h>
#include <render_vulkan/VulkanCommandBuffer.h>

#include <unordered_map>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
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
        return false;
        //tstring fileContent;
        //if (!File::readFileIntoString(fileContent, filePath))
        //{
        //    LOGE("Open file failed {0}", filePath);
        //    return false;
        //}

        //tinygltf::Model model;
        //tinygltf::TinyGLTF loader;
        //tstring err;
        //tstring warn;

        //bool ret = loader.LoadASCIIFromString(&model, &err, &warn, fileContent.data(), fileContent.length(), EMPTY_STRING);
        //if (!warn.empty())
        //{
        //    LOGW(warn);
        //}

        //if (!err.empty())
        //{
        //    LOGE(err);
        //}

        //if (!ret)
        //{
        //    LOGE("Failed to parse glTF file {0}", filePath);
        //    return false;
        //}

        //const tinygltf::Scene& scene = model.scenes[model.defaultScene];
        //for (size_t i = 0; i < scene.nodes.size(); ++i)
        //{
        //    int nodeIndex = scene.nodes[i];
        //    const auto& node = model.nodes[nodeIndex];

        //    if (node.mesh >= 0 && node.mesh < model.meshes.size())
        //    {
        //        const auto& mesh = model.meshes[node.mesh];

        //        for (size_t j = 0; j < mesh.primitives.size(); ++j)
        //        {
        //            mesh.primitives[j];
        //        }
        //    }
        //}

        //auto& attrib = reader.GetAttrib();
        //auto& shapes = reader.GetShapes();
        //auto& materials = reader.GetMaterials();

        //std::vector<VertexPosColorUv1> vertices;
        //std::vector<uint16> indices;
        //std::unordered_map<VertexPosColorUv1, uint16> uniqueVertices;

        //// Loop over shapes
        //for (const auto& shape : shapes)
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

        //return uploadData(vertices.data(),
        //                  static_cast<uint32>(vertices.size()),
        //                  sizeof(VertexPosColorUv1),
        //                  indices.data(),
        //                  static_cast<uint16>(indices.size()));
    }

} // namespace blink
