
/*********************************************************************
 * \file   LineArtApp.h
 * \brief  
 * 
 * \author zjhlogo
 * \date   04/24/2022
 *********************************************************************/
#pragma once

#include <blink/app.h>

class LineArtApp : public blink::IApp
{
public:
    LineArtApp() = default;
    virtual ~LineArtApp() = default;

    virtual bool initialize() override;
    virtual void terminate() override;

};
