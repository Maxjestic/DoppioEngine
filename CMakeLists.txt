# --- 1. Project Setup (C++23) ---
cmake_minimum_required(VERSION 4.1.2)

project(DoppioEngine VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 2. Fetch Dependencies (GLFW & GLM only) ---
include(FetchContent)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.0
)
FetchContent_MakeAvailable(glm)

# --- 3. Configure GLAD (from local files) ---
add_library(glad STATIC
        Source/ThirdParty/glad/src/glad.c
)

target_include_directories(glad
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/ThirdParty/glad/include
)

# --- 4. Build Our Engine ---
add_executable(DoppioEngine
        Source/main.cpp

        # Dear ImGui Core
        Source/ThirdParty/imgui/imconfig.h
        Source/ThirdParty/imgui/imgui.h
        Source/ThirdParty/imgui/imgui_internal.h
        Source/ThirdParty/imgui/imstb_rectpack.h
        Source/ThirdParty/imgui/imstb_textedit.h
        Source/ThirdParty/imgui/imstb_truetype.h
        Source/ThirdParty/imgui/imgui.cpp
        Source/ThirdParty/imgui/imgui_demo.cpp
        Source/ThirdParty/imgui/imgui_draw.cpp
        Source/ThirdParty/imgui/imgui_tables.cpp
        Source/ThirdParty/imgui/imgui_widgets.cpp

        # Dear ImGui Backends
        Source/ThirdParty/imgui/backends/imgui_impl_glfw.cpp
        Source/ThirdParty/imgui/backends/imgui_impl_opengl3.cpp
        
        # Debugger Visualizer
        Source/ThirdParty/imgui/imgui.natvis
)

# --- 5. Link Libraries ---
target_include_directories(DoppioEngine
        PRIVATE
        Source/ThirdParty/imgui
        Source/ThirdParty/imgui/backends
)

target_link_libraries(DoppioEngine
        PRIVATE
        glfw
        glm
        glad
)

if (MSVC)
    target_compile_options(DoppioEngine PRIVATE /std:c++latest /EHsc)
endif()
