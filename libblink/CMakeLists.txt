cmake_minimum_required(VERSION 3.8)
include (${CMAKE_CURRENT_SOURCE_DIR}/../cmake/utils.cmake)

project(libblink)

find_package(glfw3 PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
find_package(glm PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
find_package(fmt PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
find_package(tinyxml2 PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)

set(SOURCE_FILES
core/Component.cpp
core/Component.h
core/Entity.cpp
core/Entity.h
device/Device.h
render/geometries/BoxGeometry.cpp
render/geometries/BoxGeometry.h
render/geometries/BufferAttributes.cpp
render/geometries/BufferAttributes.h
render/geometries/BufferGeometry.cpp
render/geometries/BufferGeometry.h
render/geometries/Geometry.cpp
render/geometries/Geometry.h
render/materials/Material.cpp
render/materials/Material.h
render/textures/Texture.cpp
render/textures/Texture.h
render/textures/Texture2D.cpp
render/textures/Texture2D.h
render/objects/Mesh.cpp
render/objects/Mesh.h
render/objects/Object.cpp
render/objects/Object.h
render/shaders/ShaderLib.h
render/GlConfig.h
render/RenderModule.cpp
render/RenderModule.h
render/RenderState.cpp
render/RenderState.h
render/Scene.cpp
render/Scene.h
render/Shader.cpp
render/Shader.h
utils/ImageLoader.cpp
utils/ImageLoader.h
utils/stb_image.h
Framework.cpp
Framework.h
IApp.cpp
IApp.h
)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(SOURCE_FILES ${SOURCE_FILES} device/platform/windows/DeviceWindows.cpp)
endif ()

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
	$<BUILD_INTERFACE:$<TARGET_PROPERTY:glfw,INTERFACE_INCLUDE_DIRECTORIES>>
	$<BUILD_INTERFACE:$<TARGET_PROPERTY:glm,INTERFACE_INCLUDE_DIRECTORIES>>
	$<BUILD_INTERFACE:$<TARGET_PROPERTY:fmt::fmt,INTERFACE_INCLUDE_DIRECTORIES>>
	$<BUILD_INTERFACE:$<TARGET_PROPERTY:tinyxml2::tinyxml2,INTERFACE_INCLUDE_DIRECTORIES>>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../external/glad/include>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../libfoundation>
)

group_sources("${SOURCE_FILES}")
configure_project(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
