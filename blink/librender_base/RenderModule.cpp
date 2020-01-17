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

RenderModule::FuncCreateBufferObject RenderModule::createBufferObject{};
RenderModule::FuncDestroyBufferObject RenderModule::destroyBufferObject{};
RenderModule::FuncCreateVertexBuffer RenderModule::createVertexBuffer{};
RenderModule::FuncDestroyVertexBuffer RenderModule::destroyVertexBuffer{};
RenderModule::FuncCreateShaderFromStock RenderModule::createShaderFromStock{};
RenderModule::FuncCreateShaderFromBuffer RenderModule::createShaderFromBuffer{};
RenderModule::FuncDestroyShader RenderModule::destroyShader{};

RenderModule::RenderModule(const tstring& name)
    : m_name(name)
{
}

RenderModule::~RenderModule()
{
}

const tstring& RenderModule::getName()
{
    return m_name;
}

NS_END
