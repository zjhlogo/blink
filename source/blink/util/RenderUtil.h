/*!
 * \file RenderUtil.h
 *
 * \author zjhlogo
 * \date 2021/10/25
 *
 *
 */
#pragma once
#include <foundation/BaseTypesGlm.h>

NS_BEGIN

class VulkanCommandBuffer;
class Mesh;
class Material;

class RenderUtil
{
public:
    static void drawMesh(VulkanCommandBuffer& commandBuffer, Mesh* mesh, Material* material, const glm::vec3& pos, const glm::quat& rot);
};

NS_END
