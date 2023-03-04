cmake_minimum_required(VERSION 3.8)
include(${PROJECT_ROOT}/cmake/utils.cmake)

project(pendulum)

set(SOURCE_FILES
    components/PendulumComponents.h
    components/UserCommandComponents.h
    systems/EntityCreationSystem.cpp
    systems/EntityCreationSystem.h
    systems/PrefabInitializeSystem.cpp
    systems/PrefabInitializeSystem.h
    systems/SinglePendulumSystem.cpp
    systems/SinglePendulumSystem.h
    systems/UserCommandSystem.cpp
    systems/UserCommandSystem.h
    PendulumApp.cpp
    PendulumApp.h
    )

group_sources("${SOURCE_FILES}")
configure_runtime(${PROJECT_NAME} ${BIN_ROOT} "example")
