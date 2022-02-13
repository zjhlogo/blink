
/*********************************************************************
 * \file   DynamicIntegrateSystem.h
 * \brief  
 * 
 * \author zjhlogo
 * \date   2/13 2022
 *********************************************************************/
#pragma once

#include <core/base/ILogicalSystem.h>

namespace blink
{
	class DynamicIntegrateSystem : ILogicalSystem
	{
	public:
		virtual bool initialize(flecs::world& world) override;
		virtual void terminate(flecs::world& world) override;

	};
}
