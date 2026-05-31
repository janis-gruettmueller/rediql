cmake_minimum_required(VERSION 3.15)
project(RediQL VERSION 0.1 LANGUAGES CXX)

# Force C++23 standard for features like std::variant optimizations and concepts
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable aggressive compiler warnings to catch bugs early in development
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Define where the compiler should look for header (.hpp) files
include_directories(src tests/cpp)

# Set all C++ implementation source files
set(SRC_FILES)

set(TEST_FILES
    tests/cpp/unit/main.cpp
    tests/cpp/unit/test_cell.cpp
    tests/cpp/unit/test_table.cpp
)

# -------------------------------------------------------------------
# Target 1: Executable Sandbox (For local C++ development/testing)
# -------------------------------------------------------------------
add_executable(rediql_sandbox src/main.cpp ${SRC_FILES})

# -------------------------------------------------------------------
# Target 2: Test Executable
# -------------------------------------------------------------------
add_executable(rediql_tests ${TEST_FILES} ${SRC_FILES})

# -------------------------------------------------------------------
# Target 3: Python Bindings Extension (Phase 5 Prep)
# -------------------------------------------------------------------
# find_package(pybind11 REQUIRED)
# pybind11_add_module(rediql_core bindings/python_module.cpp ${SRC_FILES})