cmake_minimum_required(VERSION 3.8)
include (${CMAKE_CURRENT_SOURCE_DIR}/../cmake/utils.cmake)

project(libblink)

find_package(glfw3 PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
find_package(glm PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
find_package(fmt PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
find_package(tinyxml2 PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
find_package(entityx PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)

set(SOURCE_FILES
systems/CameraSystem.cpp
systems/CameraSystem.h
systems/GlConfig.h
systems/InputSystem.cpp
systems/InputSystem.h
systems/OpenGL3RenderSystem.cpp
systems/OpenGL3RenderSystem.h
systems/RenderSystem.cpp
systems/RenderSystem.h
systems/SceneSystem.cpp
systems/SceneSystem.h
utils/ImageLoader.cpp
utils/ImageLoader.h
utils/stb_image.h
blink.cpp
blink.h
Components.cpp
Components.h
GeometryBuilder.cpp
GeometryBuilder.h
GeometryUtils.cpp
GeometryUtils.h
Materials.cpp
Materials.h
Shader.cpp
Shader.h
ShaderLib.h
Texture.cpp
Texture.h
Texture2D.cpp
Texture2D.h
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
