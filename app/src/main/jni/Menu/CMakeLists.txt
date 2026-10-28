cmake_minimum_required(VERSION 3.10.2)
project(Menu)

include_directories(
    ${CMAKE_SOURCE_DIR}/Includes
    ${CMAKE_SOURCE_DIR}/ImGui
)

file(GLOB_RECURSE IMGUI_SRC "ImGui/*.cpp")
file(GLOB_RECURSE SRC "*.cpp")

add_library(Menu SHARED
    ${SRC}
    ${IMGUI_SRC}
)

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
