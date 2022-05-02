cmake_minimum_required(VERSION 3.8)
include(${PROJECT_ROOT}/cmake/utils.cmake)

project(libphysics)

set(SOURCE_FILES
components/Components.cpp
components/Components.h
systems/BeginPhysicsSimulationSystem.cpp
systems/BeginPhysicsSimulationSystem.h
systems/DynamicIntegrateSystem.cpp
systems/DynamicIntegrateSystem.h
systems/EndPhysicsSimulationSystem.cpp
systems/EndPhysicsSimulationSystem.h
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} libcore)

target_include_directories(${PROJECT_NAME} PRIVATE
	$<BUILD_INTERFACE:${PROJECT_ROOT}/external>
	$<BUILD_INTERFACE:${PROJECT_ROOT}/source>
)

group_sources("${SOURCE_FILES}")
configure_library(${PROJECT_NAME} "blink")
