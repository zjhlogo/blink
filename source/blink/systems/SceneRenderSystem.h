/*********************************************************************
 * \file   SceneRenderSystem.h
 * \brief
 *
 * \author zjhlogo
 * \date   02/07/2022
 *********************************************************************/
#pragma once

#include "../app.h"

#include <core/IRenderSystem.h>

class SceneRenderSystem final : public blink::IRenderSystem
{
public:
    explicit SceneRenderSystem(blink::IApp* app);
    ~SceneRenderSystem() override = default;

    SceneRenderSystem(const SceneRenderSystem&) = delete;
    SceneRenderSystem(SceneRenderSystem&&) = delete;
    SceneRenderSystem& operator=(const SceneRenderSystem&) = delete;
    SceneRenderSystem& operator=(SceneRenderSystem&&) = delete;

    bool initialize() override;
    void terminate() override;
    void render() override;

private:
    blink::IApp* m_app{};
};
