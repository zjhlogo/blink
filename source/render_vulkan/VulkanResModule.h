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
    class VulkanRenderPass;

    class VulkanResModule final : public IResModule
    {
    public:
        static const tstring DEFAULT_TEXTURE;

    public:
        VulkanResModule() = default;

        bool initialize() override;
        void terminate() override;

        ITexture2d* createTexture2d(const tstring& filePath) override;
        void releaseTexture2d(ITexture2d* texture) override;

        IGeometry* createGeometry(const tstring& uniqueId, PrimitiveTopology topology) override;
        void releaseGeometry(IGeometry* geometry) override;

        IMaterial* createMaterial(const tstring& filePath) override;
        void releaseMaterial(IMaterial* material) override;

    private:
        VulkanLogicalDevice* m_logicalDevice{};
        VulkanRenderPass* m_renderPass{};

        std::unordered_map<tstring, ITexture2d*> m_texture2dMap;
        std::unordered_map<tstring, IGeometry*> m_geometryMap;
        std::unordered_map<tstring, IMaterial*> m_materialMap;
    };
} // namespace blink
