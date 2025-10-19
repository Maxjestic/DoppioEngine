# --- 1. Project Setup (C++23) ---
cmake_minimum_required(VERSION 3.20)
project(DoppioEngine VERSION 0.1.0)

# Set the C++ standard to C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include the FetchContent module
include(FetchContent)

# --- 2. Fetch Dependencies (GLFW & GLM only) ---
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
add_library(glad
        Source/ThirdParty/glad/src/glad.c
)

target_include_directories(glad
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/ThirdParty/glad/include
)

# --- 4. Build Our Engine ---
add_executable(DoppioEngine
        Source/main.cpp
)

# --- 5. Link Libraries ---
target_link_libraries(DoppioEngine
        PUBLIC
        glfw
        glm
        glad
)

# On macOS
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_link_libraries(DoppioEngine
            PRIVATE
            "-framework OpenGL"
    )
endif ()
