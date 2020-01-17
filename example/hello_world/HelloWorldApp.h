/*!
 * \file HelloWorldApp.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 *
 */
#pragma once
#include <blink.h>

class HelloWorldApp : public NS::App
{
public:
    HelloWorldApp();
    virtual ~HelloWorldApp();

    virtual bool initialize() override;
    virtual void terminate() override;
};
