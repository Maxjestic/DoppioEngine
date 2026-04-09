################
# --- ROOT --- #
################


# --- 1. Project Setup (C++23) ---
cmake_minimum_required(VERSION 4.1.2)

project(DoppioWorkspace VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- 2. Global C++ Standards ---
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 3. Target Directories ---
add_subdirectory(Engine)
add_subdirectory(Sandbox)
