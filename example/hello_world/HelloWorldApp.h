/*********************************************************************
 * \file   HelloWorldApp.h
 * \brief  
 * 
 * \author zjhlogo
 * \date   February 2023
 *********************************************************************/
#pragma once

#include <blink/app.h>

class HelloWorldApp final : public blink::IApp
{
protected:
    bool initializeLogicalSystems() override;
    bool initializeRenderSystems() override;
};
