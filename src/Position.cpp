#include "Position.h"

/**
 * Chuyển vị trí sang chess notation
 */
std::string Position::toNotation() const {
    if (!isValid()) return "invalid";
    
    std::string notation;
    notation += char('a' + col);
    notation += char('8' - row);
    
    return notation;
}

/**
 * Chuyển chess notation sang Position
 */
Position Position::fromNotation(const std::string& notation) {
    if (notation.length() < 2) return Position();
    
    int col = notation[0] - 'a';
    int row = '8' - notation[1];
    
    Position pos(row, col);
    if (!pos.isValid()) return Position();
    
    return pos;
}
