#include "Light.h"

namespace blink
{
    void Light::release()
    {
        if (decReference() <= 0)
        {
            delete this;
        }
    }
}
