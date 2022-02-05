cmake_minimum_required(VERSION 3.8)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(libspirv-cross)

set(SOURCE_FILES
GLSL.std.450.h
spirv.h
spirv.hpp
spirv_cfg.cpp
spirv_cfg.hpp
spirv_common.hpp
spirv_cpp.cpp
spirv_cpp.hpp
spirv_cross.cpp
spirv_cross.hpp
spirv_cross_containers.hpp
spirv_cross_error_handling.hpp
spirv_cross_parsed_ir.cpp
spirv_cross_parsed_ir.hpp
spirv_cross_util.hpp
spirv_glsl.cpp
spirv_glsl.hpp
spirv_hlsl.hpp
spirv_msl.hpp
spirv_parser.cpp
spirv_parser.hpp
spirv_reflect.hpp
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
	$<BUILD_INTERFACE:${PROJECT_ROOT}/external>
)

group_sources("${SOURCE_FILES}")
configure_library(${PROJECT_NAME} "external")
