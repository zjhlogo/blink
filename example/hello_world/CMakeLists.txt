cmake_minimum_required(VERSION 3.8)
include(${PROJECT_ROOT}/cmake/utils.cmake)

project(hello_world)

set(SOURCE_FILES
HelloWorldApp.cpp
HelloWorldApp.h
)

group_sources("${SOURCE_FILES}")
configure_runtime(${PROJECT_NAME} ${BIN_ROOT} "example")
