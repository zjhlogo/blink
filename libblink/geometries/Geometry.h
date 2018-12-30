#pragma once
#include <Rtti.h>

namespace blink
{
    class Geometry
    {
    public:
        RTTI_ROOT(Geometry);

        Geometry();

        void release();
        int incReference() { return ++m_reference; };
        int decReference() { return --m_reference; };

    protected:
        virtual ~Geometry();

    private:
        int m_reference{};
    };
}
