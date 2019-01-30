cmake_minimum_required(VERSION 3.8)
include (${CMAKE_CURRENT_SOURCE_DIR}/../cmake/utils.cmake)

project(libblink)

find_package(glfw3 PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
find_package(glm PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
find_package(fmt PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
find_package(tinyxml2 PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
find_package(entityx PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)

set(SOURCE_FILES
camera/CameraData.h
camera/CameraSystem.cpp
camera/CameraSystem.h
input/InputData.h
input/InputSystem.cpp
input/InputSystem.h
render/shader_lib/ShaderLib.h
render/util/ImageLoader.cpp
render/util/ImageLoader.h
render/util/stb_image.h
render/util/GeometryUtils.cpp
render/util/GeometryUtils.h
render/GeometryBuilder.cpp
render/GeometryBuilder.h
render/GlConfig.h
render/OpenGL3RenderSystem.cpp
render/OpenGL3RenderSystem.h
render/RenderData.h
render/RenderSystem.cpp
render/RenderSystem.h
render/Shader.cpp
render/Shader.h
render/Texture.cpp
render/Texture.h
render/Texture2D.cpp
render/Texture2D.h
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
