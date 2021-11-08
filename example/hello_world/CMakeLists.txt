cmake_minimum_required(VERSION 3.8)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(hello_world)

set(SOURCE_FILES
systems/EntityCreationSystem.cpp
systems/EntityCreationSystem.h
HelloWorldApp.cpp
HelloWorldApp.h
)

group_sources("${SOURCE_FILES}")
configure_runtime(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../../bin "example")
