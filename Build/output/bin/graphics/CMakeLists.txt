# Specify the minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(SFML_Project VERSION 1.0)

# Set the C++ standard to use (C++17 in this case)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set SFML path (adjust to where SFML is located)
set(SFML_DIR "${CMAKE_SOURCE_DIR}/SFML")

# Add SFML include directories
include_directories(${SFML_DIR}/include)

# Add SFML libraries
link_directories(${SFML_DIR}/lib)