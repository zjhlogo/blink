cmake_minimum_required(VERSION 3.8)
include (${CMAKE_CURRENT_SOURCE_DIR}/../cmake/utils.cmake)

project(libblink)

find_package(glfw3 PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
find_package(glm PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
find_package(fmt PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
find_package(tinyxml2 PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
find_package(entityx PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)

set(SOURCE_FILES
camera/CameraData.cpp
camera/CameraData.h
camera/CameraSystem.cpp
camera/CameraSystem.h
input/InputData.h
input/InputSystem.cpp
input/InputSystem.h
render/geometry/BoxGeometry.cpp
render/geometry/BoxGeometry.h
render/geometry/BufferAttributes.cpp
render/geometry/BufferAttributes.h
render/geometry/BufferGeometry.cpp
render/geometry/BufferGeometry.h
render/geometry/Geometry.cpp
render/geometry/Geometry.h
render/geometry/GeometryUtils.cpp
render/geometry/GeometryUtils.h
render/geometry/PlaneGeometry.cpp
render/geometry/PlaneGeometry.h
render/geometry/SphereGeometry.cpp
render/geometry/SphereGeometry.h
render/light/AmbientLight.cpp
render/light/AmbientLight.h
render/light/Light.cpp
render/light/Light.h
render/light/PointLight.cpp
render/light/PointLight.h
render/material/Material.cpp
render/material/Material.h
render/material/PhongMaterial.cpp
render/material/PhongMaterial.h
render/material/WireframeMaterial.cpp
render/material/WireframeMaterial.h
render/shader/shader_lib/ShaderLib.h
render/shader/Shader.cpp
render/shader/Shader.h
render/texture/Texture.cpp
render/texture/Texture.h
render/texture/Texture2D.cpp
render/texture/Texture2D.h
render/util/ImageLoader.cpp
render/util/ImageLoader.h
render/util/stb_image.h
render/GlConfig.h
render/OpenGL3RenderSystem.cpp
render/OpenGL3RenderSystem.h
render/RenderData.h
render/RenderSystem.cpp
render/RenderSystem.h
scene/SceneSystem.cpp
scene/SceneSystem.h
scene/TransformData.h
blink.cpp
blink.h
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
	$<BUILD_INTERFACE:$<TARGET_PROPERTY:glfw,INTERFACE_INCLUDE_DIRECTORIES>>
	$<BUILD_INTERFACE:$<TARGET_PROPERTY:glm,INTERFACE_INCLUDE_DIRECTORIES>>
	$<BUILD_INTERFACE:$<TARGET_PROPERTY:fmt::fmt,INTERFACE_INCLUDE_DIRECTORIES>>
	$<BUILD_INTERFACE:$<TARGET_PROPERTY:tinyxml2::tinyxml2,INTERFACE_INCLUDE_DIRECTORIES>>
	$<BUILD_INTERFACE:$<TARGET_PROPERTY:entityx,INTERFACE_INCLUDE_DIRECTORIES>>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../external/glad/include>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../external/imgui>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../libfoundation>
)

group_sources("${SOURCE_FILES}")
configure_library(${PROJECT_NAME} "")
