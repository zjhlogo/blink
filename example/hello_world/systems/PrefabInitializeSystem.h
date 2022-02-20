
/*********************************************************************
 * \file   PrefabInitializeSystem.h
 * \brief
 *
 * \author zjhlogo
 * \date   2/19 2022
 *********************************************************************/

#include <core/base/ILogicalSystem.h>

class PrefabInitializeSystem : public blink::ILogicalSystem
{
public:
    PrefabInitializeSystem();

    virtual bool initialize(flecs::world& world) override;
    virtual void terminate(flecs::world& world) override;

public:
    flecs::entity prefabRigidBody;

};
