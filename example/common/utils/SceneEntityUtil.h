/*********************************************************************
 * \file   SceneEntityUtil.h
 * \brief  
 * 
 * \author zjhlogo
 * \date   February 2023
 *********************************************************************/
#pragma once
#include <core/EcsWorld.h>
#include <blink/app.h>

class SceneEntityUtil
{
public:
    static bool initializeCommonLogicalSystems(blink::IApp* app);
    static bool initializeCommonSceneEntities(blink::EcsWorld& ecsWorld);

};
