/**

    @file      SphereUv.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      5.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/

#include "SphereUv.h"
#include "builder/SphereUvBuilder.h"

namespace blink
{
    SphereUv::SphereUv(VulkanLogicalDevice& logicalDevice, VulkanCommandPool& commandPool)
        : IGeometry(logicalDevice, commandPool)
    {
    }

    SphereUv::~SphereUv() {}

    bool SphereUv::create(const glm::vec3& center, float radius, uint16 rings, uint16 section)
    {
        std::vector<glm::vec3> vertsPos;
        std::vector<glm::vec3> vertsNormal;
        std::vector<glm::vec2> vertsUv0;
        std::vector<uint16> indices;

        SphereUvBuilder builder;
        builder
            .center(center.x, center.y, center.z)
            .radius(radius)
            .ringAndSection(rings, section)
            .build(vertsPos, indices, &vertsNormal, &vertsUv0);

        return uploadData(indices, vertsPos, vertsNormal, vertsUv0);
    }
} // namespace blink
