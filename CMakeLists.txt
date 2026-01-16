cmake_minimum_required(VERSION 3.10)
project(ChessGame)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.cpp
    src/Piece.cpp
    src/Position.cpp
    src/Move.cpp
    src/Board.cpp
    src/MoveGenerator.cpp
    src/GameState.cpp
    src/AIPlayer.cpp
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

# Print build info
message(STATUS "Chess Game - CMake Configuration")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "SFML Found: ${SFML_FOUND}")
