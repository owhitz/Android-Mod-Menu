cmake_minimum_required(VERSION 3.10.2)
project(Menu)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fvisibility=hidden -fexceptions -frtti -Wno-error=format-security -Wno-format-security")

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Includes
    ${CMAKE_SOURCE_DIR}/ImGui
    ${CMAKE_SOURCE_DIR}/ImGui/backends
)

# Aqui a gente diz pra compilar o Menu.cpp e os arquivos da ImGui, e ignorar o resto
file(GLOB IMGUI_SRC "ImGui/*.cpp")
set(SRC_LIST "Menu/Menu.cpp" ${IMGUI_SRC})

add_library(Menu SHARED ${SRC_LIST})

find_library(log-lib log)
find_library(android-lib android)
find_library(GLESv2-lib GLESv2)

target_link_libraries(Menu ${log-lib} ${android-lib} ${GLESv2-lib} dl z m)
