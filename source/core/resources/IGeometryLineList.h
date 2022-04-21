
/*********************************************************************
 * \file   IGeometryLineList.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#pragma once

#include "../base/Color.h"
#include "IGeometry.h"

#include <foundation/BaseTypesGlm.h>

#include <vector>

namespace blink
{
    class IGeometryLineList : public IGeometry
    {
        friend class IResourceModule;

    public:
        virtual bool uploadData(const std::vector<uint16>& indices,
                                const std::vector<glm::vec3>& positions,
                                const std::vector<Color>& colors) = 0;
    };
} // namespace blink
