/**

    @file      Texture2d.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      5.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/
#pragma once
#include "../resources/IResource.h"

namespace blink
{
    class VulkanLogicalDevice;
    class VulkanTexture;

    class ResourceMgr;

    class Texture2d : public IResource
    {
        friend ResourceMgr;

    public:
        virtual void release() override;

        VulkanTexture* getVulkanTexture() const { return m_texture; };

    private:
        Texture2d();
        ~Texture2d();

        bool create(VulkanLogicalDevice& logicalDevice, const tstring& filePath);

    private:
        VulkanTexture* m_texture{};
    };
} // namespace blink
