# Chess Game - Build Instructions

## ✅ Khuyến nghị: Sử dụng MSYS2 (Dễ nhất)

### Bước 1: Cài đặt MSYS2

1. Tải từ: https://www.msys2.org/
2. Chạy installer và cài vào `C:\msys64`
3. Mở **MSYS2 MINGW64** terminal (icon màu xanh dương)

### Bước 2: Cài GCC và SFML

```bash
# Cập nhật package database (MSYS2 sẽ lấy GCC/SFML bản mới nhất)
pacman -Syu

# Cài GCC và SFML
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-sfml
```

### Bước 3: Build Game

```bash
# Di chuyển vào thư mục dự án (chú ý: /e/ thay vì E:\)
cd /e/ProjectCode/ChessGame

# Compile
g++ -std=c++17 main.cpp -o ChessGame.exe -lsfml-graphics -lsfml-window -lsfml-system

# Chạy
./ChessGame.exe
```

### Bước 4: Chạy Game

Sau khi build thành công:
- Trong MSYS2: `./ChessGame.exe`
- Hoặc double-click `ChessGame.exe` trong Windows Explorer

---

## ⚠️ Phương án thay thế: Native MinGW (Không khuyến nghị)

Nếu bạn muốn dùng MinGW native thay vì MSYS2:

### Yêu cầu
- MinGW GCC 13 trở lên (tương thích các gói SFML chính thức hiện tại)
- SFML 2.6.1 build cho MinGW (file `.a`, không phải `.lib`)

### Tải SFML
https://www.sfml-dev.org/files/SFML-2.6.1-windows-gcc-13.1.0-mingw-64-bit.zip

### Build
```powershell
.\build-dynamic.bat
```

**Lưu ý:** Phương án này thường gặp lỗi linking. Nên dùng MSYS2.

---

## 📁 Cấu trúc dự án

```
ChessGame/
├── model/              # Model Layer - Game logic
├── view/               # View Layer - Rendering
├── controller/         # Controller Layer - Game flow
├── main.cpp           # Entry point
├── asset/             # Textures, fonts
└── public/            # Save files
```

## 🎮 Controls

- **Menu**: UP/DOWN arrows, ENTER
- **Game**: Click to select and move pieces
- **Promotion**: Press 1(Queen), 2(Rook), 3(Bishop), 4(Knight)

## 🐛 Troubleshooting

### "command not found: g++"
Đảm bảo bạn đang dùng **MSYS2 MINGW64** terminal, không phải PowerShell.

### "cannot find -lsfml-graphics"
Cài lại SFML:
```bash
pacman -S mingw-w64-x86_64-sfml
```

### Game không hiển thị gì
Kiểm tra thư mục `asset/` có đầy đủ textures không.

---

## 📖 Tài liệu thêm

- [MSYS2_GUIDE.md](../brain/.../MSYS2_GUIDE.md) - Hướng dẫn chi tiết MSYS2
- [walkthrough.md](../brain/.../walkthrough.md) - Tổng quan về cấu trúc MVC
