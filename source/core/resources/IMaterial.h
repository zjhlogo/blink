
/*********************************************************************
 * \file   IMaterial.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#pragma once

#include "IGeometry.h"
#include "IResource.h"

#include <foundation/BaseTypesGlm.h>

namespace blink
{
    struct PerMaterialUniforms
    {
        alignas(4) float roughness;
        alignas(4) float metallic;
        alignas(16) glm::vec3 color;
    };

    class IMaterial : public IResource
    {
    public:
        virtual void release() override;

        virtual float getRoughness() const = 0;
        virtual void setRoughness(float roughness) = 0;

        virtual float getMetallic() const = 0;
        virtual void setMetallic(float metallic) = 0;

        virtual const glm::vec3& getColor() const = 0;
        virtual void setColor(const glm::vec3& color) = 0;

        virtual bool recreate() = 0;
    };
} // namespace blink
