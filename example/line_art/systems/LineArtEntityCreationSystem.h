
/*********************************************************************
 * \file   LineArtEntityCreationSystem.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/24/2022
 *********************************************************************/
#pragma once

#include <blink/components/Components.h>
#include <core/ILogicalSystem.h>

class LineArtEntityCreationSystem : public blink::ILogicalSystem
{
public:
    virtual bool initialize() override;
    virtual void terminate() override;
    virtual void framePostUpdate() override;

private:
    blink::IMaterial* m_material{};
};
