#ifndef MOVE_H
#define MOVE_H

#include "Position.h"
#include "Piece.h"
#include <string>

/**
 * Enum định nghĩa các loại nước đi đặc biệt
 */
enum class MoveType {
    NORMAL,            // Nước đi thông thường
    CASTLE_KINGSIDE,   // Nhập thành cánh vua
    CASTLE_QUEENSIDE,  // Nhập thành cánh hậu
    EN_PASSANT,        // Bắt tốt qua đường
    PROMOTION          // Phong cấp tốt
};

/**
 * Struct đại diện cho một nước đi trong game
 */
struct Move {
    Position from;              // Vị trí xuất phát
    Position to;                // Vị trí đích
    MoveType moveType;          // Loại nước đi
    Piece capturedPiece;        // Quân bị bắt (nếu có)
    PieceType promotionPiece;   // Quân phong cấp thành (nếu là PROMOTION)
    
    // Constructor mặc định
    Move() : from(), to(), moveType(MoveType::NORMAL), 
             capturedPiece(), promotionPiece(PieceType::NONE) {}
    
    /**
     * Constructor tạo nước đi thông thường
     * @param f: vị trí xuất phát
     * @param t: vị trí đích
     */
    Move(Position f, Position t) 
        : from(f), to(t), moveType(MoveType::NORMAL),
          capturedPiece(), promotionPiece(PieceType::NONE) {}
    
    /**
     * Constructor tạo nước đi với loại cụ thể
     * @param f: vị trí xuất phát
     * @param t: vị trí đích
     * @param type: loại nước đi
     */
    Move(Position f, Position t, MoveType type)
        : from(f), to(t), moveType(type),
          capturedPiece(), promotionPiece(PieceType::NONE) {}
    
    /**
     * Chuyển nước đi sang notation dạng "e2e4" hoặc "e7e8q" (cho promotion)
     * @return string notation
     */
    std::string toNotation() const;
    
    /**
     * Tạo Move từ notation (ví dụ: "e2e4", "e7e8q")
     * @param notation: string notation
     * @return Move object
     */
    static Move fromNotation(const std::string& notation);
    
    // So sánh hai nước đi
    bool operator==(const Move& other) const {
        return from == other.from && to == other.to && 
               moveType == other.moveType && 
               promotionPiece == other.promotionPiece;
    }
};

#endif // MOVE_H
