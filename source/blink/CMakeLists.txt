cmake_minimum_required(VERSION 3.8)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(libblink)

find_package(glm PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
find_package(fmt PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
find_package(entityx PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)

set(SOURCE_FILES
camera/CameraData.cpp
camera/CameraData.h
camera/CameraSystem.cpp
camera/CameraSystem.h
geometry/BoxGeometry.cpp
geometry/BoxGeometry.h
geometry/GeometryUtils.cpp
geometry/GeometryUtils.h
geometry/PlaneGeometry.cpp
geometry/PlaneGeometry.h
geometry/SphereGeometry.cpp
geometry/SphereGeometry.h
input/InputData.h
input/InputSystem.cpp
input/InputSystem.h
light/AmbientLight.cpp
light/AmbientLight.h
light/Light.cpp
light/Light.h
light/PointLight.cpp
light/PointLight.h
material/Material.cpp
material/Material.h
material/PhongMaterial.cpp
material/PhongMaterial.h
material/WireframeMaterial.cpp
material/WireframeMaterial.h
scene/SceneSystem.cpp
scene/SceneSystem.h
scene/TransformData.h
blink.cpp
blink.h
RenderData.h
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
	$<BUILD_INTERFACE:$<TARGET_PROPERTY:glm,INTERFACE_INCLUDE_DIRECTORIES>>
	$<BUILD_INTERFACE:$<TARGET_PROPERTY:fmt::fmt,INTERFACE_INCLUDE_DIRECTORIES>>
	$<BUILD_INTERFACE:$<TARGET_PROPERTY:entityx,INTERFACE_INCLUDE_DIRECTORIES>>
	$<BUILD_INTERFACE:${PROJECT_ROOT}/external/vulkan/source>
	$<BUILD_INTERFACE:${PROJECT_ROOT}/external/glad/include>
	$<BUILD_INTERFACE:${PROJECT_ROOT}/source>
)

group_sources("${SOURCE_FILES}")
configure_library(${PROJECT_NAME} "blink")
