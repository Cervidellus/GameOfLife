# submodules/imgui/CMakeLists.txt

cmake_minimum_required(VERSION 3.8)
project(imgui)

file(GLOB IMGUI_SOURCES
    ../../submodules/imgui/*.cpp
    ../../submodules/imgui/*.h
    ../../submodules/imgui/backends/imgui_impl_sdl3.cpp
    ../../submodules/imgui/backends/imgui_impl_sdlrenderer3.cpp
    ../../submodules/imgui/misc/cpp/imgui_stdlib.cpp
)
    
add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../../submodules/imgui"
"${CMAKE_CURRENT_SOURCE_DIR}/../../submodules/imgui/backends"
"${CMAKE_CURRENT_SOURCE_DIR}/../../submodules/imgui/misc/cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../../submodules/sdl3/include"
)