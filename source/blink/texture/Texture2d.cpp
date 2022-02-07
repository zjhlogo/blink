/**

    @file      Texture2d.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      5.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/

#include "Texture2d.h"

#include <blink/resource/ResourceMgr.h>
#include <render_vulkan/VulkanTexture.h>

namespace blink
{
    Texture2d::Texture2d()
    {
        //
    }

    Texture2d::~Texture2d()
    {
        //
        SAFE_DELETE(m_texture);
    }

    void Texture2d::release()
    {
        //
        ResourceMgr::getInstance().releaseTexture2d(this);
    }

    bool Texture2d::create(VulkanLogicalDevice& logicalDevice, const tstring& filePath)
    {
        SAFE_DELETE(m_texture);

        m_texture = new VulkanTexture(logicalDevice);
        if (!m_texture->createTexture2D(filePath)) return false;

        return true;
    }

} // namespace blink
