cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(gameoflife LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE})
# if(CMAKE_BUILD_TYPE STREQUAL "debug")
#     message(STATUS "Current build configuration: Debug")
# elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
#     message(STATUS "Current build configuration: Release")
# else()
#     message(STATUS "Unknown build configuration")
# endif()

set(VCPKG_DIR $ENV{VCPKG_DIR})
if(NOT VCPKG_DIR)
    message(FATAL_ERROR "VCPKG_DIR is not set")
endif()
message(STATUS "Using path: ${VCPKG_DIR}")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

find_package(sdl2 CONFIG REQUIRED)
message(STATUS "SDL2 found: ${SDL2_FOUND}")

add_executable(gameoflife src/main.cpp src/core.cpp)

target_include_directories(gameoflife PRIVATE include)

target_link_libraries(gameoflife SDL2::SDL2)
# target_link_libraries()

#copy resources to build directory
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
# message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
# message(STATUS "CMAKE_CURRENT_LIST_DIR: ${CMAKE_CURRENT_LIST_DIR}")
# message(STATUS "CMAKE_CURRENT_exe_DIR: ${CMAKE_CURRENT_}")

add_custom_command(
        TARGET gameoflife POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/resources
                $<TARGET_FILE_DIR:gameoflife>/resources)
