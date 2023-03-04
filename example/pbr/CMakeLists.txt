cmake_minimum_required(VERSION 3.8)
include(${PROJECT_ROOT}/cmake/utils.cmake)

project(pbr)

set(SOURCE_FILES
        PbrApp.cpp
PbrApp.h
)

group_sources("${SOURCE_FILES}")
configure_runtime(${PROJECT_NAME} ${BIN_ROOT} "example")
