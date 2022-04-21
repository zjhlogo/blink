
/*********************************************************************
 * \file   ITexture2d.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#pragma once

#include "ITexture.h"
namespace blink
{
    class ITexture2d : public ITexture
    {
        friend class IResourceModule;

    public:
        virtual void release() override;

    };
} // namespace blink
