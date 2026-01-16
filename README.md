# Chess Game - C++ & SFML

Game cờ vua với kiến trúc MVC, AI thông minh và đầy đủ luật cờ vua.

## Tính năng

- Kiến trúc MVC (Model-View-Controller)
- 2 chế độ: Player vs Player, Player vs AI
- AI với Minimax + Alpha-Beta Pruning
- Đầy đủ luật: Castling, En Passant, Promotion
- Phát hiện Check, Checkmate, Stalemate
- Lưu/Load game với FEN notation
- GUI với SFML

## Cài đặt & Build

### Ubuntu/Debian:
```bash
sudo apt-get install cmake g++ libsfml-dev
bash compile.sh
./ChessGame
```

### Sử dụng CMake:
```bash
mkdir build && cd build
cmake ..
make
./ChessGame
```

## Điều khiển

**Menu:** UP/DOWN + ENTER  
**Game:** Click chọn quân, click di chuyển  
**Promotion:** 1(Q), 2(R), 3(B), 4(N)

## Cấu trúc

```
ChessGame/
├── include/       # Header files
├── src/           # Source files
├── asset/         # Textures & logo
├── public/        # Save files
└── CMakeLists.txt
```

## License

Educational project - Free to use.