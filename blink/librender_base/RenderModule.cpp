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

RenderModule* RenderModule::s_renderModule = nullptr;

RenderModule::RenderModule(const tstring& name)
    : m_name(name)
{
    assert(!s_renderModule);
    s_renderModule = this;
}

RenderModule::~RenderModule()
{
    assert(s_renderModule);
    s_renderModule = nullptr;
}

RenderModule& RenderModule::getInstance()
{
    assert(s_renderModule);
    return *s_renderModule;
}

const tstring& RenderModule::getName()
{
    return m_name;
}

NS_END
