#include "CameraData.h"
#include "../render/shader/Shader.h"

namespace blink
{
    void CameraData::setupShaderUniforms(const glm::mat4 & localToWorld, Shader * shader)
    {
        shader->setUniform("u_worldToClip", worldToClip);
        shader->setUniform("u_localToWorld", localToWorld);
        shader->setUniform("u_localToWorldTranInv", glm::transpose(glm::inverse(glm::mat3(localToWorld))));
        shader->setUniform("u_localToClip", worldToClip * localToWorld);
        shader->setUniform("u_viewPos", cameraPos);
    }
}
