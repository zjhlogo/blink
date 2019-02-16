cmake_minimum_required(VERSION 3.8)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(noise_test)

set(SOURCE_FILES
module/Crop.cpp
module/Crop.h
module/Step.cpp
module/Step.h
main.cpp
noiseutils.cpp
noiseutils.h
)

group_sources("${SOURCE_FILES}")

set(PROJ_NAME ${PROJECT_NAME})
set(OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../bin)
set(FOLDER_NAME "test")

add_executable(${PROJ_NAME} ${SOURCE_FILES})

target_include_directories(${PROJ_NAME} PRIVATE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../libfoundation>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../external/noise/include>
)

set(ALL_LIBRARIES
libfoundation
libnoise
)

target_link_libraries(${PROJ_NAME} ${ALL_LIBRARIES})

set_target_properties(${PROJ_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH})
set_target_properties(${PROJ_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_PATH})
set_target_properties(${PROJ_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_PATH})
set_target_properties(${PROJ_NAME} PROPERTIES OUTPUT_NAME_DEBUG ${PROJ_NAME}_D)
set_target_properties(${PROJ_NAME} PROPERTIES OUTPUT_NAME_RELEASE ${PROJ_NAME})
set_target_properties(${PROJ_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${OUTPUT_PATH})
if (NOT ${FOLDER_NAME} STREQUAL "")
	set_target_properties(${PROJ_NAME} PROPERTIES FOLDER ${FOLDER_NAME})
endif()
