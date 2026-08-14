cmake_minimum_required(VERSION 3.16)
project(LibraryManagement LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SQLite3 REQUIRED)

add_executable(library_management src/library_management.cpp)
target_link_libraries(library_management PRIVATE SQLite::SQLite3)

if(MSVC)
    target_compile_options(library_management PRIVATE /W3)
else()
    target_compile_options(library_management PRIVATE -Wall -Wextra)
endif()
