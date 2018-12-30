#include "Geometry.h"

namespace blink
{
    Geometry::Geometry()
    {

    }

    Geometry::~Geometry()
    {

    }

    void Geometry::release()
    {
        if (decReference() <= 0)
        {
            delete this;
        }
    }
}
