cmake_minimum_required(VERSION 3.16)
project(VCS VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/repository.cpp
    src/utils.cpp
    src/commit.cpp
)

# Executable
add_executable(vcs ${SOURCES})

# Link libraries if needed (e.g., for filesystem)
target_link_libraries(vcs stdc++fs)  # For std::filesystem on some compilers
