//
// Created by zjhlogo on 2023/3/4.
//

#include "BuiltinResource.h"
#include "../geometries_builder/SphereUvBuilder.h"
#include "core/modules/IResourceModule.h"

namespace blink
{
    IGeometry* BuiltinResource::m_sphereGeometry{};
    IMaterial* BuiltinResource::m_unlitMaterial{};

    bool BuiltinResource::initialize()
    {
        auto resourceModule = getResourceModule();

        {
            SphereUvBuilder builder;
            m_sphereGeometry = builder.build();
        }

        m_unlitMaterial = resourceModule->createMaterial("/materials/unlit_white.mtl");

        return true;
    }

    void BuiltinResource::terminate()
    {
        SAFE_RELEASE(m_sphereGeometry);
    }
} // namespace blink
