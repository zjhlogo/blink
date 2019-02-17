#include "RandomMapApp.h"
#include "SideViewCameraSystem.h"
#include "MapSystem.h"
#include "MapRenderSystem.h"
#include <scene/SceneSystem.h>
#include <render/util/ImageLoader.h>

entityx::Entity createMap(entityx::EntityManager& entities, const blink::tstring& filePath)
{
    entityx::Entity map = entities.create();

    // load map from file
    blink::ImageLoader::ImageInfo imageInfo;
    blink::ImageLoader::loadTextureFromImage(imageInfo, filePath, true);

    map.assign<MapData>(imageInfo.width, imageInfo.height, imageInfo.data);

    return map;
}

bool RandomMapApp::initialize()
{
    m_ex.systems.add<SideViewCameraSystem>();
    m_ex.systems.add<blink::SceneSystem>();
    m_ex.systems.add<MapSystem>();
    m_ex.systems.add<MapRenderSystem>();
    m_ex.systems.configure();

    // add map data
    createMap(m_ex.entities, "resource/random_map_big.png");

    // add a camera
    entityx::Entity camera = m_ex.entities.create();
    camera.assign<blink::CameraData>(glm::vec3(0.0f, 0.0f, 3.0f), blink::VEC3_ZERO, blink::VEC3_PY);

    return true;
}

void RandomMapApp::terminate()
{

}

int main(int argc, char** argv)
{
    RandomMapApp app;
    return blink::run(&app);
}
