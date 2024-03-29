
/*********************************************************************
 * \file   LineListBuilder.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/15/2022
 *********************************************************************/
#pragma once

#include "IGeometryBuilder.h"

#include <vector>

namespace blink
{
    class LineListGeometry;

    class LineListBuilder : public IGeometryBuilder
    {
    public:
        LineListBuilder();

        virtual tstring getUniqueId() const override;

        bool build(LineListGeometry* geometry) const;

    private:
        int m_uniqueId{};
        std::vector<uint16> m_indices;
        std::vector<glm::vec3> m_verts;
    };
} // namespace blink
