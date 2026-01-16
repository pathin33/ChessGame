#ifndef PIECE_H
#define PIECE_H

#include <string>

/**
 * Enum định nghĩa các loại quân cờ trong game cờ vua
 */
enum class PieceType {
    NONE = 0,
    PAWN = 1,
    KNIGHT = 2,
    BISHOP = 3,
    ROOK = 4,
    QUEEN = 5,
    KING = 6
};

/**
 * Enum định nghĩa màu của quân cờ
 */
enum class PieceColor {
    NONE = 0,
    WHITE = 1,
    BLACK = 2
};

/**
 * Struct đại diện cho một quân cờ
 * Chứa thông tin về loại quân, màu và giá trị material
 */
struct Piece {
    PieceType type;
    PieceColor color;
    int value; // Material value (Pawn=10, Knight=30, Bishop=30, Rook=50, Queen=90, King=900)
    
    // Constructor mặc định - tạo quân cờ rỗng
    Piece() : type(PieceType::NONE), color(PieceColor::NONE), value(0) {}
    
    // Constructor với tham số - tạo quân cờ với type và color cụ thể
    Piece(PieceType t, PieceColor c);
    
    // Kiểm tra quân cờ có rỗng không
    bool isEmpty() const { return type == PieceType::NONE; }
    
    // So sánh hai quân cờ
    bool operator==(const Piece& other) const {
        return type == other.type && color == other.color;
    }
    
    bool operator!=(const Piece& other) const {
        return !(*this == other);
    }
};

/**
 * Chuyển đổi quân cờ thành ký tự FEN
 * Ví dụ: White Pawn = 'P', Black Knight = 'n'
 */
char pieceToChar(const Piece& piece);

/**
 * Chuyển đổi ký tự FEN thành quân cờ
 * Ví dụ: 'P' = White Pawn, 'n' = Black Knight
 */
Piece charToPiece(char c);

/**
 * Lấy tên quân cờ dạng string (để debug/display)
 */
std::string pieceTypeName(PieceType type);

#endif // PIECE_H
