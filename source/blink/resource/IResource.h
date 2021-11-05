/**

    @file      IResource.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      5.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/
#pragma once

#include <foundation/BaseTypes.h>

namespace blink
{
    class IResource
    {
    public:
        void setId(const tstring& id) { m_id = id; };
        const tstring& getId() const { return m_id; };

        int incRef() { return ++m_ref; };
        int decRef() { return --m_ref; };
        int getRef() { return m_ref; };

    private:
        tstring m_id;
        int m_ref{};
    };
} // namespace blink