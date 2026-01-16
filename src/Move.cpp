#include "Move.h"

/**
 * Chuyển Move sang notation string
 */
std::string Move::toNotation() const {
    std::string notation = from.toNotation() + to.toNotation();
    
    if (moveType == MoveType::PROMOTION) {
        switch (promotionPiece) {
            case PieceType::QUEEN:  notation += 'q'; break;
            case PieceType::ROOK:   notation += 'r'; break;
            case PieceType::BISHOP: notation += 'b'; break;
            case PieceType::KNIGHT: notation += 'n'; break;
            default: break;
        }
    }
    
    return notation;
}

/**
 * Tạo Move từ notation string
 */
Move Move::fromNotation(const std::string& notation) {
    if (notation.length() < 4) return Move();
    
    Position from = Position::fromNotation(notation.substr(0, 2));
    Position to = Position::fromNotation(notation.substr(2, 2));
    
    Move move(from, to);
    
    if (notation.length() >= 5) {
        char promo = notation[4];
        move.moveType = MoveType::PROMOTION;
        
        switch (promo) {
            case 'q': move.promotionPiece = PieceType::QUEEN; break;
            case 'r': move.promotionPiece = PieceType::ROOK; break;
            case 'b': move.promotionPiece = PieceType::BISHOP; break;
            case 'n': move.promotionPiece = PieceType::KNIGHT; break;
            default:  move.promotionPiece = PieceType::QUEEN; break;
        }
    }
    
    return move;
}
