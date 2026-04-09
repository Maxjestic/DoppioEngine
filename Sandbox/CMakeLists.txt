###################
# --- SANDBOX --- #
###################


# --- 1. Build Executable ---
add_executable(Sandbox
        Source/main.cpp
)

# --- 2. Link Engine Library ---
target_link_libraries(Sandbox
        PRIVATE
        DoppioEngine
)

# --- 3. Compiler Flags ---
if (MSVC)
    target_compile_options(Sandbox PRIVATE /std:c++latest /EHsc)
endif ()