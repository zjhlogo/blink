cmake_minimum_required(VERSION 3.8)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(sprite)

set(SOURCE_FILES
Sprite.cpp
Sprite.h
SpriteApp.cpp
SpriteApp.h
SpriteGeometry.cpp
SpriteGeometry.h
SpriteMaterial.cpp
SpriteMaterial.h
SpriteRenderBlock.cpp
SpriteRenderBlock.h
SpriteRenderModule.cpp
SpriteRenderModule.h
)

group_sources("${SOURCE_FILES}")
configure_runtime(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../../bin "example")
