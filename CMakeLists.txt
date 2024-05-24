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

add_executable(gameoflife src/main.cpp src/core.cpp)

target_include_directories(gameoflife PRIVATE include)

# if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#     target_link_libraries(my_app debug_library1 debug_library2)
# else()
#     target_link_libraries(gameoflife SDL2::SDL2)
# endif()





target_link_libraries(gameoflife SDL2::SDL2)

add_custom_command(
        TARGET gameoflife POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/resources
                $<TARGET_FILE_DIR:gameoflife>/resources)