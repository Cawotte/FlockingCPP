cmake_minimum_required(VERSION 3.6)

project(flockingBoids)

add_subdirectory(extlibs/imgui)

find_package(SFML 2 COMPONENTS graphics window system REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB BOIDS_HEADERS include/*.hpp)
file(GLOB BOIDS_SOURCES src/*.cpp)

add_executable(flockingBoids ${BOIDS_HEADERS} ${BOIDS_SOURCES})

target_link_libraries(flockingBoids imgui)
if (SFML_FOUND)
  target_link_libraries(flockingBoids sfml-window sfml-graphics sfml-system)
endif()
target_link_libraries(flockingBoids ${OPENGL_LIBRARIES})

target_include_directories(flockingBoids
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_EXTENSIONS OFF)