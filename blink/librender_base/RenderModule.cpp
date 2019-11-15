/*!
 * \file RenderModule.cpp
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#include "RenderModule.h"

NS_BEGIN

RenderModule::CreateBufferObjectCb RenderModule::createBufferObject{};
RenderModule::DestroyBufferObjectCb RenderModule::destroyBufferObject{};
RenderModule::CreateVertexBufferCb RenderModule::createVertexBuffer{};
RenderModule::DestroyVertexBufferCb RenderModule::destroyVertexBuffer{};
RenderModule::CreateShaderFromStockCb RenderModule::createShaderFromStock{};
RenderModule::CreateShaderFromBufferCb RenderModule::createShaderFromBuffer{};
RenderModule::DestroyShaderCb RenderModule::destroyShader{};

RenderModule::RenderModule()
{

}

RenderModule::~RenderModule()
{

}

NS_END
