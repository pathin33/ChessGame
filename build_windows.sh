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

echo "Compiling source files for Windows..."

# Compile all source files
x86_64-w64-mingw32-g++ -c src/Piece.cpp -o build_win/Piece.o -Iinclude -std=c++17
x86_64-w64-mingw32-g++ -c src/Position.cpp -o build_win/Position.o -Iinclude -std=c++17
x86_64-w64-mingw32-g++ -c src/Move.cpp -o build_win/Move.o -Iinclude -std=c++17
x86_64-w64-mingw32-g++ -c src/Board.cpp -o build_win/Board.o -Iinclude -std=c++17
x86_64-w64-mingw32-g++ -c src/MoveGenerator.cpp -o build_win/MoveGenerator.o -Iinclude -std=c++17
x86_64-w64-mingw32-g++ -c src/GameState.cpp -o build_win/GameState.o -Iinclude -std=c++17
x86_64-w64-mingw32-g++ -c src/AIPlayer.cpp -o build_win/AIPlayer.o -Iinclude -std=c++17
x86_64-w64-mingw32-g++ -c src/main.cpp -o build_win/main.o -Iinclude -std=c++17

if [ $? -ne 0 ]; then
    echo "==================================="
    echo "✗ Compilation failed!"
    echo "==================================="
    exit 1
fi

echo "Linking..."

# Link with SFML Windows libraries (you need SFML compiled for Windows)
x86_64-w64-mingw32-g++ build_win/*.o -o ChessGame.exe \
    -lsfml-graphics -lsfml-window -lsfml-system \
    -static-libgcc -static-libstdc++ \
    -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic

if [ $? -eq 0 ]; then
    echo "==================================="
    echo "✓ Build successful!"
    echo "Windows executable: ChessGame.exe"
    echo "==================================="
else
    echo "==================================="
    echo "✗ Linking failed!"
    echo "You need SFML libraries compiled for Windows"
    echo "==================================="
    exit 1
fi
