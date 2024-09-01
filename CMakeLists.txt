cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(gameoflife LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(VCPKG_DIR $ENV{VCPKG_DIR})
if(NOT VCPKG_DIR)
    message(FATAL_ERROR "VCPKG_DIR is not set")
endif()
message(STATUS "Using path: ${VCPKG_DIR}")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

find_package(sdl2 CONFIG REQUIRED)
message(STATUS "SDL2 found: ${SDL2_FOUND}")
find_package(imgui CONFIG REQUIRED)
message(STATUS "imgui found: ${imgui_FOUND}")

add_executable(gameoflife  
    src/main.cpp 
    src/core.cpp 
    src/core.hpp 
    src/model/abstract_model.hpp
    src/model/modelparameters.hpp
    src/model/ColorMapper.hpp
    src/model/ColorMapper.cpp
    src/model/CpuModel.cpp
    src/model/CpuModel.hpp
    src/model/LifeQuadTree.hpp
    src/model/LifeQuadTree.cpp
    src/model/LifeQuadTreeModel.hpp
    src/model/LifeQuadTreeModel.cpp
    src/presets/modelpresets.hpp
    src/sdl_manager.cpp
    src/sdl_manager.hpp
    src/gui/gui.cpp
    src/gui/gui.hpp
    src/gui/interface.cpp 
    src/gui/interface.hpp 
    src/gui/mainwindow.cpp 
    src/gui/mainwindow.hpp
    src/gui/WidgetFunctions.hpp
    src/gui/WidgetFunctions.cpp
    submodules/ImGuiScope/ImGuiScope.hpp
    submodules/ImGuiScope/ImGuiScope.cpp
    submodules/ImGuiScope/TimerResultBuffer.hpp
    submodules/ImGuiScope/TimerResultBuffer.cpp
)


target_include_directories(gameoflife PRIVATE src)

target_link_libraries(gameoflife SDL2::SDL2)

#copy resources
add_custom_command(
        TARGET gameoflife POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/resources
                $<TARGET_FILE_DIR:gameoflife>/resources)

add_executable(quadtreetest
    QuadTreeTest/QuadTreeTest.cpp
    src/model/LifeQuadTree.hpp
    src/model/LifeQuadTree.cpp
    src/model/LifeQuadTreeModel.hpp
    src/model/LifeQuadTreeModel.cpp
)

target_include_directories(quadtreetest PRIVATE src)