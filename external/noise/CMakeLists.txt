cmake_minimum_required(VERSION 3.8)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(libnoise)

set(SOURCE_FILES
include/noise/basictypes.h
include/noise/exception.h
include/noise/interp.h
include/noise/latlon.h
include/noise/mathconsts.h
include/noise/misc.h
include/noise/noise.h
include/noise/noisegen.h
include/noise/vectortable.h
include/noise/model/cylinder.h
include/noise/model/line.h
include/noise/model/model.h
include/noise/model/plane.h
include/noise/model/sphere.h
include/noise/module/abs.h
include/noise/module/add.h
include/noise/module/billow.h
include/noise/module/blend.h
include/noise/module/cache.h
include/noise/module/checkerboard.h
include/noise/module/clamp.h
include/noise/module/const.h
include/noise/module/curve.h
include/noise/module/cylinders.h
include/noise/module/displace.h
include/noise/module/exponent.h
include/noise/module/invert.h
include/noise/module/max.h
include/noise/module/min.h
include/noise/module/module.h
include/noise/module/modulebase.h
include/noise/module/multiply.h
include/noise/module/perlin.h
include/noise/module/power.h
include/noise/module/ridgedmulti.h
include/noise/module/rotatepoint.h
include/noise/module/scalebias.h
include/noise/module/scalepoint.h
include/noise/module/select.h
include/noise/module/spheres.h
include/noise/module/terrace.h
include/noise/module/translatepoint.h
include/noise/module/turbulence.h
include/noise/module/voronoi.h
src/latlon.cpp
src/noisegen.cpp
src/model/cylinder.cpp
src/model/line.cpp
src/model/plane.cpp
src/model/sphere.cpp
src/module/abs.cpp
src/module/add.cpp
src/module/billow.cpp
src/module/blend.cpp
src/module/cache.cpp
src/module/checkerboard.cpp
src/module/clamp.cpp
src/module/const.cpp
src/module/curve.cpp
src/module/cylinders.cpp
src/module/displace.cpp
src/module/exponent.cpp
src/module/invert.cpp
src/module/max.cpp
src/module/min.cpp
src/module/modulebase.cpp
src/module/multiply.cpp
src/module/perlin.cpp
src/module/power.cpp
src/module/ridgedmulti.cpp
src/module/rotatepoint.cpp
src/module/scalebias.cpp
src/module/scalepoint.cpp
src/module/select.cpp
src/module/spheres.cpp
src/module/terrace.cpp
src/module/translatepoint.cpp
src/module/turbulence.cpp
src/module/voronoi.cpp
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/noise>
)

group_sources("${SOURCE_FILES}")
configure_library(${PROJECT_NAME} "external")
