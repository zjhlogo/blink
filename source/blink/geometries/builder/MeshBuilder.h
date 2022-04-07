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

        virtual tstring getUniqueId() const;
        bool build(Geometry* geometry, bool calcInertiaTensor = false) const;

    private:
        tstring m_filePath;
    };
} // namespace blink
