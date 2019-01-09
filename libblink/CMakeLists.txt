cmake_minimum_required(VERSION 3.8)
include (${CMAKE_CURRENT_SOURCE_DIR}/../cmake/utils.cmake)

project(libblink)

find_package(glfw3 PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
find_package(glm PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
find_package(fmt PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
find_package(tinyxml2 PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)

set(SOURCE_FILES
cameras/Camera.cpp
cameras/Camera.h
cameras/FixCamera.cpp
cameras/FixCamera.h
cameras/TargetCamera.cpp
cameras/TargetCamera.h
core/Component.cpp
core/Component.h
core/Entity.cpp
core/Entity.h
device/Device.h
geometries/BoxGeometry.cpp
geometries/BoxGeometry.h
geometries/BufferAttributes.cpp
geometries/BufferAttributes.h
geometries/BufferGeometry.cpp
geometries/BufferGeometry.h
geometries/Geometry.cpp
geometries/Geometry.h
geometries/GeometryUtils.cpp
geometries/GeometryUtils.h
geometries/PlaneGeometry.cpp
geometries/PlaneGeometry.h
geometries/SphereGeometry.cpp
geometries/SphereGeometry.h
input/MouseComponent.cpp
input/MouseComponent.h
lights/AmbientLight.cpp
lights/AmbientLight.h
lights/Light.cpp
lights/Light.h
lights/PointLight.cpp
lights/PointLight.h
materials/Material.cpp
materials/Material.h
materials/PhongMaterial.cpp
materials/PhongMaterial.h
materials/WireframeMaterial.cpp
materials/WireframeMaterial.h
objects/Mesh.cpp
objects/Mesh.h
objects/Object.cpp
objects/Object.h
objects/Scene.cpp
objects/Scene.h
render/opengl3/OpenGL3RenderModule.cpp
render/opengl3/OpenGL3RenderModule.h
render/GlConfig.h
render/RenderModule.cpp
render/RenderModule.h
shaders/Shader.cpp
shaders/Shader.h
shaders/ShaderLib.h
textures/Texture.cpp
textures/Texture.h
textures/Texture2D.cpp
textures/Texture2D.h
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
configure_library(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
