#include "Piece.h"

/**
 * Constructor tạo quân cờ với type và color
 */
Piece::Piece(PieceType t, PieceColor c) : type(t), color(c) {
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

/**
 * Chuyển quân cờ sang ký tự FEN notation
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
 * Chuyển ký tự FEN thành quân cờ
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
 * Lấy tên quân cờ dạng string
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
