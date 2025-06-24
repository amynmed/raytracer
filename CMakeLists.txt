
cmake_minimum_required(VERSION 3.10)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(raytracer)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
file(GLOB SOURCE_FILES "include/*.cpp")

set(SDL_SOURCE_DIR ${CMAKE_SOURCE_DIR}/include/SDL)

set(SDL_STATIC ON) 
set(SDL_SHARED OFF)
add_subdirectory(${SDL_SOURCE_DIR})


add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3-static)

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES})

