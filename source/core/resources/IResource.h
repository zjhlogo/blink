
/*********************************************************************
 * \file   IResource.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#pragma once

#include <foundation/BaseTypes.h>

namespace blink
{
    class IResource
    {
    public:
        [[nodiscard]] const tstring& getId() const { return m_id; };
        void setId(const tstring& id) { m_id = id; };

        [[nodiscard]] int getRef() const { return m_ref; };
        int incRef() { return ++m_ref; };
        int decRef() { return --m_ref; };

        virtual void release() = 0;

    protected:
        IResource() = default;
        virtual ~IResource() = default;

    private:
        tstring m_id;
        int m_ref{};
    };
} // namespace blink