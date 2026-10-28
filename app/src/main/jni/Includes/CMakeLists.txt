cmake_minimum_required(VERSION 3.10.2)
project(Menu)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fvisibility=hidden -fexceptions -frtti")

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Includes
    ${CMAKE_SOURCE_DIR}/ImGui
    ${CMAKE_SOURCE_DIR}/ImGui/backends
)

file(GLOB_RECURSE SRC "*.cpp" "Menu/*.cpp")

add_library(Menu SHARED ${SRC})

find_library(log-lib log)
find_library(android-lib android)
find_library(GLESv2-lib GLESv2)

target_link_libraries(Menu
    ${log-lib}
    ${android-lib}
    ${GLESv2-lib}
    dl
    z
    m
)
