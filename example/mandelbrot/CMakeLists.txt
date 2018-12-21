cmake_minimum_required(VERSION 3.8)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(mandelbrot)

find_package(glfw3 PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
find_package(glm PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
find_package(fmt PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
find_package(tinyxml2 PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)

set(SOURCE_FILES
MandelbrotApp.cpp
MandelbrotApp.h
MandelbrotMaterial.cpp
MandelbrotMaterial.h
)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
	add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES})
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
	$<BUILD_INTERFACE:$<TARGET_PROPERTY:glm,INTERFACE_INCLUDE_DIRECTORIES>>
	$<BUILD_INTERFACE:$<TARGET_PROPERTY:fmt::fmt,INTERFACE_INCLUDE_DIRECTORIES>>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../external/glad/include>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../libfoundation>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../libblink>
)

set(ALL_LIBRARIES
glfw
fmt::fmt
tinyxml2::tinyxml2
libglad
libfoundation
libblink
)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(ALL_LIBRARIES ${ALL_LIBRARIES} opengl32)
endif ()
target_link_libraries(${PROJECT_NAME} ${ALL_LIBRARIES})

group_sources("${SOURCE_FILES}")
configure_project(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../../bin)
