/*********************************************************************
 * \file   PrefabInitializeSystem.h
 * \brief
 *
 * \author zjhlogo
 * \date   2/19 2022
 *********************************************************************/
#pragma once

#include <core/systems/ILogicalSystem.h>

class PrefabInitializeSystem final : public blink::ILogicalSystem
{
public:
    bool initialize() override;
    void terminate() override;

public:
    flecs::entity prefabRigidBody;
};
