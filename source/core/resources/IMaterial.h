
/*********************************************************************
 * \file   IMaterial.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#pragma once

#include "IResource.h"

#include <core/types/UniformTypes.h>

namespace blink
{
    class IMaterial : public IResource
    {
    public:
        virtual void release() override;
        virtual bool recreate() = 0;
        virtual bool setUniform(const tstring& memberName, UniformType type, const void* data) = 0;

        template <typename T> bool setUniform(const tstring& memberName, const T& value)
        {
            auto type = getUniformType<T>();
            return setUniform(memberName, type, &value);
        }
    };
} // namespace blink
