/*********************************************************************
 * \file   ControlPanel.h
 * \brief  
 * 
 * \author zjhlogo
 * \date   February 2023
 *********************************************************************/
#pragma once

#include <common/guis/IGuiWindow.h>

class ControlPanel final : public IGuiWindow
{
public:
    void onGui() override;
};
