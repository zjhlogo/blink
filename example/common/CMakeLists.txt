cmake_minimum_required(VERSION 3.8)
include(${PROJECT_ROOT}/cmake/utils.cmake)

project(libcommon)

set(SOURCE_FILES
guis/IGuiWindow.h
common_components/LockTargetCameraData.h
logical_systems/LockTargetCameraControllerSystem.cpp
logical_systems/LockTargetCameraControllerSystem.h
render_systems/ImguiRenderSystem.cpp
render_systems/ImguiRenderSystem.h
utils/ImguiExtension.cpp
utils/ImguiExtension.h
utils/SceneEntityUtil.cpp
utils/SceneEntityUtil.h
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} libblink)

target_include_directories(${PROJECT_NAME} PRIVATE
	$<BUILD_INTERFACE:${PROJECT_ROOT}/external>
	$<BUILD_INTERFACE:${PROJECT_ROOT}/source>
)

group_sources("${SOURCE_FILES}")
configure_library(${PROJECT_NAME} "example")

add_custom_command(TARGET ${PROJECT_NAME}
    PRE_BUILD
    COMMAND compile.bat
    WORKING_DIRECTORY ${ASSETS_ROOT}/shaders
    USES_TERMINAL
    COMMENT "start compile shaders")
