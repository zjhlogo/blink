/*********************************************************************
 * \file   MainMenuBar.h
 * \brief
 *
 * \author zjhlogo
 * \date   February 2023
 *********************************************************************/
#pragma once
#include <common/guis/IGuiWindow.h>

class MainMenuBar final : public IGuiWindow
{
public:
    void onGui() override;

private:
    bool m_showControlPanel{true};
};
