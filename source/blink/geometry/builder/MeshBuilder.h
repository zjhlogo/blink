/**

    @file      MeshBuilder.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      5.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#pragma once

#include "IGeometryBuilder.h"

namespace blink
{
    class Geometry;

    class MeshBuilder : public IGeometryBuilder
    {
    public:
        MeshBuilder& filePath(const tstring& filePath);

        virtual Geometry* createGeometry(VulkanLogicalDevice& logicalDevice, VulkanCommandPool& commandPool) override;

        virtual bool build(std::vector<glm::vec3>& positionsOut,
                           std::vector<uint16>& indicesOut,
                           std::vector<glm::vec3>* normalsOut = nullptr,
                           std::vector<glm::vec2>* uvsOut = nullptr) override;

    private:
        tstring m_filePath;
    };
} // namespace blink
