/*********************************************************************
 * \file   PhysicsApp.h
 * \brief  
 * 
 * \author zjhlogo
 * \date   February 2023
 *********************************************************************/
#pragma once

#include <blink/app.h>

class PhysicsApp final : public blink::IApp
{
protected:
    bool initializeLogicalSystems() override;
    bool initializeRenderSystems() override;
};
