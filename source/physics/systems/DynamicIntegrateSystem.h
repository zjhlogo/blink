
/*********************************************************************
 * \file   DynamicIntegrateSystem.h
 * \brief  
 * 
 * \author zjhlogo
 * \date   2/13 2022
 *********************************************************************/
#pragma once

#include <core/ILogicalSystem.h>

namespace blink
{
	class DynamicIntegrateSystem : public ILogicalSystem
	{
	public:
		virtual bool initialize() override;
		virtual void terminate() override;

	};
}
