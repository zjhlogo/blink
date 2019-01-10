cmake_minimum_required(VERSION 3.8)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(sprite)

set(SOURCE_FILES
data/Item.h
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

find_package(nlohmann_json PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE
	$<BUILD_INTERFACE:$<TARGET_PROPERTY:nlohmann_json::nlohmann_json,INTERFACE_INCLUDE_DIRECTORIES>>
)
