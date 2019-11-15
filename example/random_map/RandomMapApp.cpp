#include "RandomMapApp.h"
#include "SideViewCameraSystem.h"
#include "PlayerCameraSystem.h"
#include "MapSystem.h"
#include "MapRenderSystem.h"
#include "PlayerRenderSystem.h"
#include "PlayerControlSystem.h"
#include <scene/SceneSystem.h>
#include <render/util/ImageLoader.h>

entityx::Entity createMap(entityx::EntityManager& entities, const NS::tstring& filePath)
{
    entityx::Entity map = entities.create();

    // load map from file
    NS::ImageLoader::ImageInfo imageInfo;
    NS::ImageLoader::loadTextureFromImage(imageInfo, filePath, true);

    auto mapData = map.assign<MapData>(imageInfo.width, imageInfo.height, imageInfo.data);
    mapData->origin.x = imageInfo.width / 2;
    mapData->origin.y = imageInfo.height / 4 * 3;

    return map;
}

bool RandomMapApp::initialize()
{
    m_ex.systems.add<PlayerControlSystem>();
    //m_ex.systems.add<SideViewCameraSystem>();
    m_ex.systems.add<PlayerCameraSystem>();
    m_ex.systems.add<NS::SceneSystem>();
    m_ex.systems.add<MapSystem>();
    m_ex.systems.add<MapRenderSystem>();
    m_ex.systems.add<PlayerRenderSystem>();
    m_ex.systems.configure();

    // add map data
    createMap(m_ex.entities, "resource/random_map_big.png");

    // add player
    entityx::Entity player = m_ex.entities.create();
    player.assign<PlayerData>();

    // add a camera
    entityx::Entity camera = m_ex.entities.create();
    camera.assign<NS::CameraData>(glm::vec3(0.0f, 0.0f, 30.0f), NS::VEC3_ZERO, NS::VEC3_PY);

    return true;
}

void RandomMapApp::terminate()
{

}

int main(int argc, char** argv)
{
    RandomMapApp app;
    return NS::run(&app);
}
