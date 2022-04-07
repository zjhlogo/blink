
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

    virtual bool initialize() override;
    virtual void terminate() override;

public:
    flecs::entity prefabRigidBody;

};
