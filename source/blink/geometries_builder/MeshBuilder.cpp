/**

    @file      MeshBuilder.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      5.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/

#include "MeshBuilder.h"
#include "../utils/GltfUtil.h"

#include <core/modules/IResModule.h>
#include <foundation/File.h>
#include <foundation/Log.h>

namespace blink
{
    MeshBuilder& MeshBuilder::filePath(const tstring& filePath)
    {
        m_filePath = filePath;
        return *this;
    }

    tstring MeshBuilder::getUniqueId() const
    {
        //
        return fmt::format("file_{0}", m_filePath);
    }

    IGeometry* MeshBuilder::build(bool buildNormal, bool buildUv, glm::mat3* inertiaTensorOut) const
    {
        tstring fileContent;
        if (!File::readFileIntoString(fileContent, m_filePath))
        {
            LOGE("Open file failed {0}", m_filePath);
            return false;
        }

        tinygltf::Model model;
        if (!GltfUtil::loadFromFile(model, m_filePath))
        {
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

        auto geometry = getResModule()->createGeometry(getUniqueId(), PrimitiveTopology::TriangleList);
        if (!geometry->uploadData(buffer.data.data(),
                                  buffer.data.size(),
                                  static_cast<uint32_t>(accessorPos.count),
                                  static_cast<uint32_t>(accessorIndices.count),
                                  buffViewPos.byteOffset,
                                  buffViewNormal.byteOffset,
                                  buffViewUv0.byteOffset,
                                  buffViewIndices.byteOffset))
        {
            SAFE_RELEASE(geometry);
            return nullptr;
        }

        if (inertiaTensorOut)
        {
            *inertiaTensorOut = CalculateInertiaTensor((glm::vec3*)(buffer.data.data() + buffViewPos.byteOffset), accessorPos.count);
        }

        return geometry;
    }
} // namespace blink
