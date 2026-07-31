cmake_minimum_required(VERSION 3.10)
project(CryptoLabX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR})

# Source files
set(SOURCES
    main.cpp
    utils/logger.cpp
    utils/file_analyzer.cpp
)

# Create executable
add_executable(cryptolabx ${SOURCES})

# Set output directory
set_target_properties(cryptolabx PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Enable warnings
if(MSVC)
    target_compile_options(cryptolabx PRIVATE /W4)
else()
    target_compile_options(cryptolabx PRIVATE -Wall -Wextra -pedantic)
endif()
