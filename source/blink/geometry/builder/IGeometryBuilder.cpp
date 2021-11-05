/**

    @file      IGeometryBuilder.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      5.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/

#include "IGeometryBuilder.h"

namespace blink
{
    Geometry* blink::IGeometryBuilder::createGeometry(VulkanLogicalDevice& logicalDevice, VulkanCommandPool& commandPool)
    {
        std::vector<glm::vec3> vertsPos;
        std::vector<glm::vec3> vertsNormal;
        std::vector<glm::vec2> vertsUv0;
        std::vector<uint16> indices;

        if (!build(vertsPos, indices, &vertsNormal, &vertsUv0)) return nullptr;

        Geometry* geometry = new Geometry(logicalDevice, commandPool);
        if (!geometry->uploadData(indices, vertsPos, vertsNormal, vertsUv0))
        {
            SAFE_DELETE(geometry);
            return nullptr;
        }

        return geometry;
    }
} // namespace blink
