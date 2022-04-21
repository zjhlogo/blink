
/*********************************************************************
 * \file   IMaterial.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#pragma once

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
        friend class IResourceModule;

    public:
        virtual void release() override;

        virtual bool recreate() = 0;
    };
} // namespace blink
