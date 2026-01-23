#!/bin/bash

echo "==================================="
echo "Building Chess Game for Windows..."
echo "==================================="

# Check if MinGW is installed
if ! command -v x86_64-w64-mingw32-g++ &> /dev/null; then
    echo "MinGW not found. Installing..."
    echo "Run: sudo apt-get install mingw-w64"
    exit 1
fi

# Create build directory
mkdir -p build_win

echo "Compiling main.cpp for Windows..."

# Compile main.cpp (nó include tất cả các file khác)
x86_64-w64-mingw32-g++ -std=c++17 main.cpp -o ChessGame.exe \
    -lsfml-graphics -lsfml-window -lsfml-system \
    -static-libgcc -static-libstdc++

if [ $? -eq 0 ]; then
    echo "==================================="
    echo "✓ Build successful!"
    echo "Windows executable: ChessGame.exe"
    echo "==================================="
else
    echo "==================================="
    echo "✗ Build failed!"
    echo "You need SFML libraries compiled for Windows"
    echo "==================================="
    exit 1
fi
