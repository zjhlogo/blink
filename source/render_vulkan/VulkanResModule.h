
/*********************************************************************
 * \file   VulkanResModule.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#pragma once

#include <core/modules/IResModule.h>

#include <unordered_map>

namespace blink
{
    class VulkanLogicalDevice;
    class VulkanSwapchain;

    class VulkanResModule : public IResModule
    {
    public:
        static const tstring DEFAULT_TEXTURE;

    public:
        virtual bool initialize() override;
        virtual void terminate() override;

        virtual ITexture2d* createTexture2d(const tstring& filePath) override;
        virtual void releaseTexture2d(ITexture2d* texture) override;

        virtual IGeometry* createGeometry(const tstring& uniqueId, PrimitiveTopology topology) override;
        void releaseGeometry(IGeometry* geometry) override;

        virtual IMaterial* createMaterial(const tstring& filePath) override;
        virtual void releaseMaterial(IMaterial* material) override;

        virtual void recreate() override;

    private:
        VulkanLogicalDevice* m_logicalDevice{};
        VulkanSwapchain* m_swapchain{};

        std::unordered_map<tstring, ITexture2d*> m_texture2dMap;
        std::unordered_map<tstring, IGeometry*> m_geometryMap;
        std::unordered_map<tstring, IMaterial*> m_materialMap;
    };

} // namespace blink
