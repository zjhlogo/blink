#include "Geometry.h"

namespace blink
{
    void Geometry::release()
    {
        if (decReference() <= 0)
        {
            delete this;
        }
    }
}
