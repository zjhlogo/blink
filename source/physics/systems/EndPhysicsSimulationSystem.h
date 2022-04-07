
/*********************************************************************
 * \file   EndPhysicsSimulationSystem.h
 * \brief  
 * 
 * \author zjhlogo
 * \date   2/19 2022
 *********************************************************************/
#pragma once

#include <core/base/ILogicalSystem.h>

namespace blink
{
	class EndPhysicsSimulationSystem : public ILogicalSystem
	{
	public:
		virtual bool initialize() override;
		virtual void terminate() override;

	};
}
