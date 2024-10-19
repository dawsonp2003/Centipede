# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Centipede VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specific to the processor
set(CMAKE_GENERATOR_PLATFORM x64)

# Define a common output directory
set(COMMON_OUTPUT_DIR "${CMAKE_BINARY_DIR}/output")

set(SFML_DIR "${PROJECT_SOURCE_DIR}/SFML/lib/cmake/SFML")

# Set output directories for different types of binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${COMMON_OUTPUT_DIR}/bin")  # For executables
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${COMMON_OUTPUT_DIR}/lib")  # For shared libraries
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${COMMON_OUTPUT_DIR}/lib")  # For static libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Add source files
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/code/*.cpp)

# Add the executable
add_executable(Centipede ${SOURCES})

# Get SFML Libraries
include_directories(${PROJECT_SOURCE_DIR}/SFML/include)

link_directories(${PROJECT_SOURCE_DIR}/SFML/lib)

#find_package(SFML COMPONENTS graphics window system REQUIRED)

# Link the executable to the libraries in the lib directory
target_link_libraries(Centipede PUBLIC sfml-graphics sfml-system sfml-window)

set_target_properties(
    Centipede PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${COMMON_OUTPUT_DIR}/bin"
)

add_subdirectory(SFML)

# Copy graphic and font folders
file(COPY ${PROJECT_SOURCE_DIR}/fonts
    DESTINATION "${COMMON_OUTPUT_DIR}/bin")
file(COPY ${PROJECT_SOURCE_DIR}/graphics
    DESTINATION "${COMMON_OUTPUT_DIR}/bin")
