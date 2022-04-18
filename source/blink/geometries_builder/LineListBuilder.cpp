
/*********************************************************************
 * \file   LineListBuilder.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   04/15/2022
 *********************************************************************/
#include "LineListBuilder.h"
#include "../geometries/LineListGeometry.h"

#include <foundation/BuiltinFormatter.h>

namespace blink
{
    static int g_globalIndex = 1;

    LineListBuilder::LineListBuilder()
    {
        //
        m_uniqueId = g_globalIndex++;
    }

    tstring LineListBuilder::getUniqueId() const
    {
        //
        return fmt::format("line_{0}", m_uniqueId);
    }

    void LineListBuilder::addLine(const glm::vec3& start, const glm::vec3& end, const Color& color)
    {
        uint16 baseIndex = (uint16)m_vertsPos.size();

        m_indices.push_back(baseIndex);
        m_indices.push_back(baseIndex + 1);

        m_vertsPos.push_back(start);
        m_vertsPos.push_back(end);

        m_vertsColor.push_back(color.value);
        m_vertsColor.push_back(color.value);
    }

    void LineListBuilder::reset()
    {
        m_indices.clear();
        m_vertsPos.clear();
        m_vertsColor.clear();
    }

    bool LineListBuilder::build(LineListGeometry* geometry) const
    {
        if (!geometry->uploadData(m_indices, m_vertsPos, m_vertsColor))
        {
            return false;
        }

        return true;
    }
} // namespace blink
