/**

    @file      MeshBuilder.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      5.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/

#include "MeshBuilder.h"

#include <core/modules/IResModule.h>
#include <foundation/File.h>
#include <foundation/Log.h>

namespace blink
{
    void MeshBuilder::reset()
    {
        m_model = {};
        m_filePath.clear();
    }

    tstring MeshBuilder::getUniqueId() const
    {
        //
        return fmt::format("file_{0}", m_filePath);
    }

    MeshBuilder& MeshBuilder::loadModel(const tstring& filePath)
    {
        m_filePath = filePath;

        tstring fileContent;
        if (!File::readFileIntoString(fileContent, m_filePath))
        {
            LOGE("Open file failed {0}", m_filePath);
            return *this;
        }

        if (!GltfUtil::loadFromFile(m_model, m_filePath))
        {
            return *this;
        }

        return *this;
    }

    IGeometry* MeshBuilder::build(bool buildNormal, bool buildUv, glm::mat3* inertiaTensorOut) const
    {
        if (m_model.meshes.empty())
        {
            LOGE("Empty mesh {0}", m_filePath);
            return nullptr;
        }
        const auto& mesh = m_model.meshes[0];

        if (mesh.primitives.empty())
        {
            LOGE("Empty primitive {0}", m_filePath);
            return nullptr;
        }
        const auto& primitive = mesh.primitives[0];

        if (primitive.attributes.size() != 3)
        {
            LOGE("Unsupported primitive attributes, size must be 3. {0}", m_filePath);
            return nullptr;
        }
        auto itPos = primitive.attributes.find("POSITION");
        auto itNormal = primitive.attributes.find("NORMAL");
        auto itUv0 = primitive.attributes.find("TEXCOORD_0");
        if (itPos == primitive.attributes.end() || itNormal == primitive.attributes.end() || itUv0 == primitive.attributes.end())
        {
            LOGE("Unsupported primitive attributes, must be POSITION, NORMAL and TEXCOORD_0. {0}", m_filePath);
            return nullptr;
        }

        const auto& accessorPos = m_model.accessors[itPos->second];
        const auto& accessorNormal = m_model.accessors[itNormal->second];
        const auto& accessorUv0 = m_model.accessors[itUv0->second];
        const auto& accessorIndices = m_model.accessors[primitive.indices];

        const auto& buffViewPos = m_model.bufferViews[accessorPos.bufferView];
        const auto& buffViewNormal = m_model.bufferViews[accessorNormal.bufferView];
        const auto& buffViewUv0 = m_model.bufferViews[accessorUv0.bufferView];
        const auto& buffViewIndices = m_model.bufferViews[accessorIndices.bufferView];

        if (buffViewPos.buffer != buffViewNormal.buffer || buffViewPos.buffer != buffViewUv0.buffer || buffViewPos.buffer != buffViewIndices.buffer)
        {
            LOGE("Only single buffer currently supported. {0}", m_filePath);
            return nullptr;
        }

        const auto& buffer = m_model.buffers[buffViewPos.buffer];

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
