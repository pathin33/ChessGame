#!/bin/bash

# Script biên dịch Chess Game không dùng CMake
# Nếu có lỗi, chạy: chmod +x compile.sh

echo "==================================="
echo "Compiling Chess Game..."
echo "==================================="

# Compiler và flags
CXX=g++
CXXFLAGS="-std=c++17 -Wall -Wextra -I./include"
LIBS="-lsfml-graphics -lsfml-window -lsfml-system"

# Source files
SOURCES="src/Piece.cpp src/Position.cpp src/Move.cpp src/Board.cpp src/MoveGenerator.cpp src/GameState.cpp src/AIPlayer.cpp src/main.cpp"

# Output binary
OUTPUT="ChessGame"

# Compile
echo "Compiling source files..."
$CXX $CXXFLAGS $SOURCES $LIBS -o $OUTPUT

if [ $? -eq 0 ]; then
    echo "==================================="
    echo "✓ Compilation successful!"
    echo "Run the game: ./ChessGame"
    echo "==================================="
else
    echo "==================================="
    echo "✗ Compilation failed!"
    echo "Make sure SFML is installed:"
    echo "  sudo apt-get install libsfml-dev"
    echo "==================================="
    exit 1
fi
