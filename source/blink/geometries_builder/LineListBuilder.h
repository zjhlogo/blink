
/*********************************************************************
 * \file   LineListBuilder.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/15/2022
 *********************************************************************/
#pragma once

#include "IGeometryBuilder.h"

#include <core/base/Color.h>

#include <vector>

namespace blink
{
    class LineListGeometry;

    class LineListBuilder : public IGeometryBuilder
    {
    public:
        LineListBuilder();

        virtual tstring getUniqueId() const override;

        void addLine(const glm::vec3& start, const glm::vec3& end, const Color& color);
        void reset();

        bool build(LineListGeometry* geometry) const;

    private:
        int m_uniqueId{};
        std::vector<uint16> m_indices;
        std::vector<glm::vec3> m_vertsPos;
        std::vector<Color> m_vertsColor;
    };
} // namespace blink
