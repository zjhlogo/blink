cmake_minimum_required(VERSION 3.8)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(foundation_test)

set(SOURCE_FILES
main.cpp
MathTestCase.cpp
PathParserTestCase.cpp
StringBuilderTestCase.cpp
)

group_sources("${SOURCE_FILES}")
configure_test(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../../bin "test")
