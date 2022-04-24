
/*********************************************************************
 * \file   PendulumComponents.h
 * \brief  
 * 
 * \author zjhlogo
 * \date   4/15 2022
 *********************************************************************/
#pragma once

#include <foundation/BaseTypesGlm.h>

struct SinglePendulum
{
	glm::vec3 anchorPoint;
	glm::vec3 startPos;
	glm::vec3 startVelocity;
	float length;
	glm::vec3 gravity;
};

