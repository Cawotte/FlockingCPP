cmake_minimum_required(VERSION 3.6)

project(imgui)

file(GLOB IMGUI_HEADERS include/imgui/*.h)
file(GLOB IMGUI_SOURCES src/*.cpp)

find_package(SFML 2 COMPONENTS graphics window system REQUIRED)

add_library(imgui ${IMGUI_HEADERS} ${IMGUI_SOURCES})

target_link_libraries(imgui sfml-window sfml-graphics sfml-system)

target_include_directories(imgui
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)