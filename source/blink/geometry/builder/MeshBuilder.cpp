/**

    @file      MeshBuilder.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      5.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/

#include "MeshBuilder.h"
#include "../../resource/ResourceMgr.h"

#include <foundation/File.h>
#include <foundation/Log.h>

#define TINYGLTF_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include <tinygltf/tiny_gltf.h>

namespace blink
{
    MeshBuilder& MeshBuilder::filePath(const tstring& filePath)
    {
        m_filePath = filePath;
        return *this;
    }

    tstring MeshBuilder::getUniqueId() const
    {
        return fmt::format("file_{0}", m_filePath);
    }

    bool MeshBuilder::build(Geometry* geometry) const
    {
        tstring fileContent;
        if (!File::readFileIntoString(fileContent, m_filePath))
        {
            LOGE("Open file failed {0}", m_filePath);
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
            return false;
        }

        if (!ret)
        {
            LOGE("Failed to parse glTF file {0}", m_filePath);
            return false;
        }

        if (model.meshes.empty())
        {
            LOGE("Empty mesh {0}", m_filePath);
            return false;
        }
        const auto& mesh = model.meshes[0];

        if (mesh.primitives.empty())
        {
            LOGE("Empty primitive {0}", m_filePath);
            return false;
        }
        const auto& primitive = mesh.primitives[0];

        if (primitive.attributes.size() != 3)
        {
            LOGE("Unsupport primitive attributes, size must be 3. {0}", m_filePath);
            return false;
        }
        auto itPos = primitive.attributes.find("POSITION");
        auto itNormal = primitive.attributes.find("NORMAL");
        auto itUv0 = primitive.attributes.find("TEXCOORD_0");
        if (itPos == primitive.attributes.end() || itNormal == primitive.attributes.end() || itUv0 == primitive.attributes.end())
        {
            LOGE("Unsupport primitive attributes, must be POSITION, NORMAL and TEXCOORD_0. {0}", m_filePath);
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
            LOGE("Only single buffer currently supported. {0}", m_filePath);
            return false;
        }

        const auto& buffer = model.buffers[buffViewPos.buffer];

        if (!geometry->uploadData(buffer.data.data(),
                                  buffer.data.size(),
                                  static_cast<uint32>(accessorPos.count),
                                  static_cast<uint32>(accessorIndices.count),
                                  buffViewPos.byteOffset,
                                  buffViewNormal.byteOffset,
                                  buffViewUv0.byteOffset,
                                  buffViewIndices.byteOffset))
        {
            return false;
        }

        return true;
    }
} // namespace blink
