cmake_minimum_required(VERSION 3.8)
include(${PROJECT_ROOT}/cmake/utils.cmake)

project(imgui_demo)

set(SOURCE_FILES
    panels/ControlPanel.cpp
    panels/ControlPanel.h
    panels/DetailPanel.cpp
    panels/DetailPanel.h
    panels/MainMenuBar.cpp
    panels/MainMenuBar.h
    panels/PagingPanel.cpp
    panels/PagingPanel.h
    ImguiDemoApp.cpp
    ImguiDemoApp.h
    )

group_sources("${SOURCE_FILES}")
configure_runtime(${PROJECT_NAME} ${BIN_ROOT} "example")
