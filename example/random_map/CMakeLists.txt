cmake_minimum_required(VERSION 3.8)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(random_map)

set(SOURCE_FILES
ArmorAtlas.cpp
ArmorAtlas.h
Atlas.cpp
Atlas.h
DiffuseMaterial.cpp
DiffuseMaterial.h
MapData.h
MapRenderBlock.cpp
MapRenderBlock.h
MapRenderSystem.cpp
MapRenderSystem.h
MapSystem.cpp
MapSystem.h
MapUtilities.cpp
MapUtilities.h
PlayerCameraSystem.cpp
PlayerCameraSystem.h
PlayerControlSystem.cpp
PlayerControlSystem.h
PlayerData.h
PlayerMovementSystem.cpp
PlayerMovementSystem.h
PlayerRenderSystem.cpp
PlayerRenderSystem.h
RandomMapApp.cpp
RandomMapApp.h
SideViewCameraSystem.cpp
SideViewCameraSystem.h
TileAtlas.cpp
TileAtlas.h
)

group_sources("${SOURCE_FILES}")
configure_runtime(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../../bin "example")
