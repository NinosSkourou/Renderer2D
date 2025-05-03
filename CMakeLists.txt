cmake_minimum_required(VERSION 3.20)

project("Renderer")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
SET(BUILD_MODE Debug)
SET(CMAKE_CXX_FLAGS "-Wall")
SET(CMAKE_CXX_FLAGS_DEBUG "-ggdb")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(BUILD_DEMO "build demo executable" FALSE)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_NAME} STATIC)

target_sources( ${CMAKE_PROJECT_NAME} PRIVATE ${SRC})

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)

FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(glm)

add_subdirectory( vendored/glad )

target_link_libraries( ${CMAKE_PROJECT_NAME} PUBLIC glfw glad glm)

target_include_directories( ${CMAKE_PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")


if(BUILD_DEMO)
    file(GLOB_RECURSE DEMO_SRC "${CMAKE_CURRENT_SOURCE_DIR}/demo/*.cpp")
    add_executable("demo" ${DEMO_SRC})
    add_subdirectory( demo/vendored/imgui )
    target_link_libraries("demo" PRIVATE Renderer imgui)
endif()
