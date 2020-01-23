/*!
 * \file Texture.h
 *
 * \author zjhlogo
 * \date 2019/07/26
 *
 *
 */
#pragma once
#include <foundation/BaseTypes.h>
#include <foundation/BaseTypesGlm.h>

NS_BEGIN

class Texture
{
public:
    enum class SampleFilter
    {
        Nearest = 0,
        Linear,
        LinearMipmapLinear,
        NearestMipmapNearest,
        LinearMipmapNearest,
        NearestMipmapLinear,
    };

    enum class WrapMethod
    {
        Clamp = 0,
        Repeat,
        MirrorRepeat,
    };

public:
    Texture(const tstring& id);
    virtual ~Texture();

    const tstring& getId() const { return m_id; };

private:
    tstring m_id;
};

NS_END
