cmake_minimum_required(VERSION 3.8)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(libspirv-cross)

set(SOURCE_FILES
dummy.c
GLSL.std.450.h
spirv.h
spirv.hpp
spirv_cfg.hpp
spirv_common.hpp
spirv_cpp.hpp
spirv_cross.hpp
spirv_cross_c.h
spirv_cross_containers.hpp
spirv_cross_error_handling.hpp
spirv_cross_parsed_ir.hpp
spirv_cross_util.hpp
spirv_glsl.hpp
spirv_hlsl.hpp
spirv_msl.hpp
spirv_parser.hpp
spirv_reflect.hpp
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} debug
	${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/spirv-cross-cored.lib
	${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/spirv-cross-cppd.lib
	${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/spirv-cross-glsld.lib
	)

target_link_libraries(${PROJECT_NAME} optimized
	${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/spirv-cross-core.lib
	${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/spirv-cross-cpp.lib
	${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/spirv-cross-glsl.lib
	)

target_include_directories(${PROJECT_NAME} PRIVATE
	$<BUILD_INTERFACE:${PROJECT_ROOT}/external>
)

group_sources("${SOURCE_FILES}")
configure_library(${PROJECT_NAME} "external")
