cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(gameoflife LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE})


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
    src/model/modelparameters.hpp
    src/presets/modelpresets.hpp
    src/sdl_manager.cpp
    src/sdl_manager.hpp
    src/gui/interface.cpp 
    src/gui/interface.hpp 
    src/gui/mainwindow.cpp 
    src/gui/mainwindow.hpp
    src/gui/gui.cpp
    src/gui/gui.hpp
)

target_include_directories(gameoflife PRIVATE src)

target_link_libraries(gameoflife SDL2::SDL2)

#copy resources
add_custom_command(
        TARGET gameoflife POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/resources
                $<TARGET_FILE_DIR:gameoflife>/resources)


# Set the "Use Debug Libraries" property for Debug configuration
# doesn't seem to work.
#if(MSVC)
#    set_target_properties(gameoflife PROPERTIES
#       VS_DEBUGGER_USE_DEBUG_LIBRARIES TRUE
 #   )
#endif()

#ImGui conflicts with window default libraries
if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:MSVCRT")
endif()
