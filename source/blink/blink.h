/*!
 * \file blink.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 *
 */
#pragma once
#include <foundation/BaseTypes.h>
#include <entityx/entityx.h>

NS_BEGIN

class RenderModule;

class App
{
public:
    virtual ~App();

    virtual bool initialize() = 0;
    virtual void terminate() = 0;

    virtual void step(float dt);

public:
    entityx::EntityX m_ex;
};

int run(const tstring& renderEngine);

NS_END
