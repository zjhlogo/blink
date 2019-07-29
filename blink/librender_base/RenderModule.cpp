/*!
 * \file RenderModule.cpp
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#include "RenderModule.h"

namespace blink
{
    RenderModule::CreateBufferObjectCb RenderModule::createBuffer{};
    RenderModule::DestroyBufferObjectCb RenderModule::destroyBuffer{};
    RenderModule::CreateVertexBufferCb RenderModule::createVertexBuffer{};

    RenderModule::RenderModule()
    {

    }

    RenderModule::~RenderModule()
    {

    }

}
