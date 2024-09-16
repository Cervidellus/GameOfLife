cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(gameoflife LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add submodules
add_subdirectory(submodules/sdl3)
##add_subdirectory(submodules/imgui)
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
    TARGET gameoflife POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#           "$<IF:$<CONFIG:Debug>, ${PROJECT_SOURCE_DIR}/build/submodules/sdl3/Debug/SDL3.dll, ${PROJECT_SOURCE_DIR}/build/submodules/sdl3/Release/SDL3.dll>"
            ${CMAKE_SOURCE_DIR}/build/submodules/sdl3/Release/SDL3.dll
            $<TARGET_FILE_DIR:gameoflife>
)

# Uncomment if you need to add another executable
# add_executable(quadtreetest
#     QuadTreeTest/QuadTreeTest.cpp
#     src/model/LifeQuadTree.hpp
#     src/model/LifeQuadTree.cpp
#     src/model/LifeQuadTreeModel.hpp
#     src/model/LifeQuadTreeModel.cpp
# )

# target_include_directories(quadtreetest PRIVATE src submodules/sdl3/include)