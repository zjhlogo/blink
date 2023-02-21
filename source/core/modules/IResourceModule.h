/*********************************************************************
 * \file   IResourceModule.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#pragma once

#include "../resources/IGeometry.h"
#include "../resources/IMaterial.h"
#include "../resources/ITexture2d.h"

namespace blink
{
    class IResourceModule
    {
    public:
        IResourceModule() = default;
        virtual ~IResourceModule() = default;

        IResourceModule(const IResourceModule&) = delete;
        IResourceModule(IResourceModule&&) = delete;
        IResourceModule& operator=(const IResourceModule&) = delete;
        IResourceModule& operator=(IResourceModule&&) = delete;

        virtual bool initialize() = 0;
        virtual void terminate() = 0;

        virtual ITexture2d* createTexture2d(const tstring& filePath) = 0;
        virtual void releaseTexture2d(ITexture2d* texture) = 0;

        virtual IGeometry* createGeometry(const tstring& uniqueId, PrimitiveTopology topology) = 0;
        virtual void releaseGeometry(IGeometry* geometry) = 0;

        virtual IMaterial* createMaterial(const tstring& filePath) = 0;
        virtual void releaseMaterial(IMaterial* material) = 0;
    };

    IResourceModule* getResourceModule();
} // namespace blink
