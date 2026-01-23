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
    Piece(PieceType t, PieceColor c) : type(t), color(c) {
        switch (type) {
            case PieceType::PAWN:   value = 10; break;
            case PieceType::KNIGHT: value = 30; break;
            case PieceType::BISHOP: value = 30; break;
            case PieceType::ROOK:   value = 50; break;
            case PieceType::QUEEN:  value = 90; break;
            case PieceType::KING:   value = 900; break;
            default:                value = 0; break;
        }
    }
    
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
char pieceToChar(const Piece& piece) {
    if (piece.type == PieceType::NONE) return '.';
    
    char c;
    switch (piece.type) {
        case PieceType::KING:   c = 'K'; break;
        case PieceType::QUEEN:  c = 'Q'; break;
        case PieceType::ROOK:   c = 'R'; break;
        case PieceType::BISHOP: c = 'B'; break;
        case PieceType::KNIGHT: c = 'N'; break;
        case PieceType::PAWN:   c = 'P'; break;
        default:                c = '.'; break;
    }
    
    if (piece.color == PieceColor::BLACK) {
        c = c + ('a' - 'A');
    }
    
    return c;
}

/**
 * Chuyển đổi ký tự FEN thành quân cờ
 * Ví dụ: 'P' = White Pawn, 'n' = Black Knight
 */
Piece charToPiece(char c) {
    PieceColor color = (c >= 'A' && c <= 'Z') ? PieceColor::WHITE : PieceColor::BLACK;
    
    if (c >= 'a' && c <= 'z') {
        c = c - ('a' - 'A');
    }
    
    PieceType type;
    switch (c) {
        case 'K': type = PieceType::KING; break;
        case 'Q': type = PieceType::QUEEN; break;
        case 'R': type = PieceType::ROOK; break;
        case 'B': type = PieceType::BISHOP; break;
        case 'N': type = PieceType::KNIGHT; break;
        case 'P': type = PieceType::PAWN; break;
        default:  return Piece();
    }
    
    return Piece(type, color);
}

/**
 * Lấy tên quân cờ dạng string (để debug/display)
 */
std::string pieceTypeName(PieceType type) {
    switch (type) {
        case PieceType::PAWN:   return "Pawn";
        case PieceType::KNIGHT: return "Knight";
        case PieceType::BISHOP: return "Bishop";
        case PieceType::ROOK:   return "Rook";
        case PieceType::QUEEN:  return "Queen";
        case PieceType::KING:   return "King";
        default:                return "None";
    }
}
