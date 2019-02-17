cmake_minimum_required(VERSION 3.8)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(random_map)

set(SOURCE_FILES
Atlas.cpp
Atlas.h
MapData.h
MapSystem.cpp
MapSystem.h
MapTileMaterial.cpp
MapTileMaterial.h
MapRenderBlock.cpp
MapRenderBlock.h
MapRenderSystem.cpp
MapRenderSystem.h
RandomMapApp.cpp
RandomMapApp.h
SideViewCameraSystem.cpp
SideViewCameraSystem.h
TileAtlas.cpp
TileAtlas.h
)

group_sources("${SOURCE_FILES}")
configure_runtime(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../../bin "example")
