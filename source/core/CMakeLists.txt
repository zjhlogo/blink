cmake_minimum_required(VERSION 3.8)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(libcore)

set(SOURCE_FILES
base/Color.cpp
base/Color.h
base/ILogicalSystem.h
components/Components.h
systems/AngularVelocitySystem.cpp
systems/AngularVelocitySystem.h
systems/LinearVelocitySystem.cpp
systems/LinearVelocitySystem.h
EcsWorld.cpp
EcsWorld.h
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} libfoundation libflecs libphysics)

target_include_directories(${PROJECT_NAME} PRIVATE
	$<BUILD_INTERFACE:${PROJECT_ROOT}/external>
	$<BUILD_INTERFACE:${PROJECT_ROOT}/source>
)

group_sources("${SOURCE_FILES}")
configure_library(${PROJECT_NAME} "blink")
