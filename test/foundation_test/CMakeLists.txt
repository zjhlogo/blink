cmake_minimum_required(VERSION 3.8)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(foundation_test)

find_package (GTest PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
find_package (fmt PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)

set(SOURCE_FILES
main.cpp
PathParserTestCase.cpp
StringBuilderTestCase.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
	$<BUILD_INTERFACE:$<TARGET_PROPERTY:GTest::gtest,INTERFACE_INCLUDE_DIRECTORIES>>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../libfoundation>
)

set(ALL_LIBRARIES
$<TARGET_PROPERTY:fmt::fmt,IMPORTED_LOCATION_DEBUG>
libfoundation
$<TARGET_PROPERTY:GTest::gtest,IMPORTED_LOCATION_DEBUG>
)
target_link_libraries(${PROJECT_NAME} ${ALL_LIBRARIES})

group_sources("${SOURCE_FILES}")
configure_project(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../../bin)
