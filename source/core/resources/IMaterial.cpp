
/*********************************************************************
 * \file   IMaterial.cpp
 * \brief  
 * 
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#include "IMaterial.h"
#include "../modules/IResourceModule.h"

namespace blink
{
	void IMaterial::release()
	{
		//
		getResourceModule()->releaseMaterial(this);
	}

}
