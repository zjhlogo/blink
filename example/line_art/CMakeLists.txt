cmake_minimum_required(VERSION 3.8)
include(${PROJECT_ROOT}/cmake/utils.cmake)

project(line_art)

set(SOURCE_FILES
systems/LineArtEntityCreationSystem.cpp
systems/LineArtEntityCreationSystem.h
LineArtApp.cpp
LineArtApp.h
)

group_sources("${SOURCE_FILES}")
configure_runtime(${PROJECT_NAME} ${BIN_ROOT} "example")
