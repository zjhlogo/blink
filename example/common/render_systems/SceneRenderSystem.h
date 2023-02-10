/*********************************************************************
 * \file   SceneRenderSystem.h
 * \brief
 *
 * \author zjhlogo
 * \date   02/07/2022
 *********************************************************************/
#pragma once

#include <blink/app.h>
#include <core/IRenderSystem.h>

class SceneRenderSystem final : public blink::IRenderSystem
{
public:
    explicit SceneRenderSystem(blink::IApp* app);
    ~SceneRenderSystem() override = default;

    SceneRenderSystem(const SceneRenderSystem& system) = delete;
    SceneRenderSystem(SceneRenderSystem&& system) = delete;
    SceneRenderSystem& operator=(const SceneRenderSystem& system) = delete;
    SceneRenderSystem& operator=(SceneRenderSystem&& system) = delete;

    bool initialize() override;
    void terminate() override;
    void render(blink::IRenderData& renderData) override;

private:
    blink::IApp* m_app{};
};
