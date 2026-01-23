cmake_minimum_required(VERSION 3.10)
project(ChessGame)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML (yêu cầu 2.6 trở lên để tương thích MinGW mới)
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# Main source file (includes all other .cpp files)
set(SOURCES
    main.cpp
)

# Create executable
add_executable(ChessGame ${SOURCES})

# Link SFML libraries
target_link_libraries(ChessGame sfml-graphics sfml-window sfml-system)

# Copy public folder to build directory (để load assets)
add_custom_command(TARGET ChessGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/public
    ${CMAKE_BINARY_DIR}/public
)

# Copy asset folder to build directory
add_custom_command(TARGET ChessGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/asset
    ${CMAKE_BINARY_DIR}/asset
)

# Print build info
message(STATUS "Chess Game - MVC Architecture - CMake Configuration")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "SFML Found: ${SFML_FOUND}")
message(STATUS "Source Structure: MVC (Model-View-Controller)")
message(STATUS "  - Model: model/*.cpp")
message(STATUS "  - View: view/*.cpp")
message(STATUS "  - Controller: controller/*.cpp")
message(STATUS "  - Main: main.cpp")
