#!/bin/bash

# Script biên dịch Chess Game - MVC Architecture
# Chỉ cần compile main.cpp (nó include tất cả các file khác)

echo "==================================="
echo "Compiling Chess Game (MVC)..."
echo "==================================="

# Compiler và flags
CXX=g++
CXXFLAGS="-std=c++17 -Wall -Wextra"
LIBS="-lsfml-graphics -lsfml-window -lsfml-system"

# Source file (chỉ cần main.cpp)
SOURCE="main.cpp"

# Output binary
OUTPUT="ChessGame"

# Compile
echo "Compiling $SOURCE..."
$CXX $CXXFLAGS $SOURCE $LIBS -o $OUTPUT

if [ $? -eq 0 ]; then
    echo "==================================="
    echo "✓ Compilation successful!"
    echo "Run the game: ./$OUTPUT"
    echo "==================================="
else
    echo "==================================="
    echo "✗ Compilation failed!"
    echo "Make sure SFML is installed:"
    echo "  sudo apt-get install libsfml-dev"
    echo "==================================="
    exit 1
fi
