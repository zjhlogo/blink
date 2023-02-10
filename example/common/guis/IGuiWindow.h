/*********************************************************************
 * \file   IGuiWindow.h
 * \brief  
 * 
 * \author zjhlogo
 * \date   02/07/2022
 *********************************************************************/
#pragma once

class IGuiWindow
{
public:
    virtual ~IGuiWindow() = default;
    virtual void renderUi() = 0;
};
