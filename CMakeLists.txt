cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(gameoflife LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add submodules
add_subdirectory(submodules/sdl3)
#imgui does not provide a CMakeLists.txt file, so we need to add it manually
add_subdirectory(cmake/imgui)

# Add the executable
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
    src/model/HashLifeTree.cpp
    src/model/HashLifeTree.hpp
 #   src/model/LifeQuadTree.hpp
 #  src/model/LifeQuadTree.cpp
 #   src/model/LifeQuadTreeModel.hpp
 #   src/model/LifeQuadTreeModel.cpp
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

# Include directories for gameoflife
target_include_directories(gameoflife PRIVATE src submodules/sdl3/include submodules/imgui)

# Link against SDL3 and ImGui libraries
target_link_libraries(gameoflife PRIVATE SDL3::SDL3 imgui)

# Set output directories for ImGui library
set_target_properties(imgui PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release"
)

# Copy resources
add_custom_command(
    TARGET gameoflife POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/resources
            $<TARGET_FILE_DIR:gameoflife>/resources
)

#copy SDL3.dll
add_custom_command(
    TARGET gameoflife 
    POST_BUILD

    COMMAND ${CMAKE_COMMAND} -E echo "Copying SDL3.dll from: $<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/build/submodules/sdl3/Debug/SDL3.dll,${CMAKE_SOURCE_DIR}/build/submodules/sdl3/Release/SDL3.dll>"

    COMMAND ${CMAKE_COMMAND} -E copy
    $<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/build/submodules/sdl3/Debug/SDL3.dll,${CMAKE_SOURCE_DIR}/build/submodules/sdl3/Release/SDL3.dll>
    $<TARGET_FILE_DIR:gameoflife>
)

#copy imgui 
add_custom_command(
    TARGET gameoflife 
    POST_BUILD

    COMMAND ${CMAKE_COMMAND} -E echo "Copying imgui.dll from: $<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/build/cmake/imgui/Debug/imgui.dll,${CMAKE_SOURCE_DIR}/build/cmake/imgui/Release/imgui.dll>"

    COMMAND ${CMAKE_COMMAND} -E copy
    $<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/build/cmake/imgui/Debug/imgui.dll,${CMAKE_SOURCE_DIR}/build/cmake/imgui/Release/imgui.dll>
    $<TARGET_FILE_DIR:gameoflife>
)

#configure the .sln file
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT gameoflife)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
# Set runtime library for Debug and Release configurations
target_compile_options(gameoflife PRIVATE
    $<$<CONFIG:Debug>:/MTd>  # Multi-threaded Debug DLL
    $<$<CONFIG:Release>:/MT> # Multi-threaded DLL
)

#if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#    message(STATUS "Debug: YES")
#else()
#    message(STATUS "Debug: NO")
#endif()

# Uncomment if you need to add another executable
# add_executable(quadtreetest
#     QuadTreeTest/QuadTreeTest.cpp
#     src/model/LifeQuadTree.hpp
#     src/model/LifeQuadTree.cpp
#     src/model/LifeQuadTreeModel.hpp
#     src/model/LifeQuadTreeModel.cpp
# )

# target_include_directories(quadtreetest PRIVATE src submodules/sdl3/include)