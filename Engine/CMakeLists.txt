##################
# --- ENGINE --- #
##################


# --- Fetch Dependencies (GLFW & GLM) ---
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

# --- Configure GLAD (Static Library) ---
add_library(glad STATIC
        Source/ThirdParty/glad/src/glad.c
)

target_include_directories(glad
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/ThirdParty/glad/include
)

# --- Variable Grouping ---
set(IMGUI_SOURCES
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

set(ENGINE_RENDERER_SOURCES
        # DoppioEngine
        Source/Renderer/Renderer.h
        Source/Renderer/Renderer.cpp
        Source/Renderer/VertexBuffer.h
        Source/Renderer/VertexBuffer.cpp
        Source/Renderer/IndexBuffer.h
        Source/Renderer/IndexBuffer.cpp
        Source/Renderer/VertexArray.h
        Source/Renderer/VertexArray.cpp
        Source/Renderer/VertexBufferLayout.h
        Source/Renderer/VertexBufferLayout.cpp
)

# --- Build Engine Library Static. --- #TODO change to Shared (DLL)
add_library(DoppioEngine STATIC
        ${IMGUI_SOURCES}
        ${ENGINE_RENDERER_SOURCES}
)

# --- Expose Engine API ---
target_include_directories(DoppioEngine
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

# --- Internal Engine Dependencies ---
target_include_directories(DoppioEngine
        PUBLIC # TODO change to PRIVATE later
        Source/ThirdParty/imgui
        Source/ThirdParty/imgui/backends
)

target_link_libraries(DoppioEngine
        PUBLIC # TODO change to PRIVATE later
        glfw
        glm
        glad
)

# --- Compiler Flags ---
if (MSVC)
    target_compile_options(DoppioEngine PRIVATE /std:c++latest /EHsc)
endif ()