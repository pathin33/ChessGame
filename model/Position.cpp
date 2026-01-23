#include <string>

/**
 * Struct đại diện cho vị trí trên bàn cờ (row, col)
 * row: 0-7 (0 = hàng trên cùng, 7 = hàng dưới cùng)
 * col: 0-7 (0 = cột trái nhất 'a', 7 = cột phải nhất 'h')
 */
struct Position {
    int row;
    int col;
    
    // Constructor mặc định
    Position() : row(-1), col(-1) {}
    
    // Constructor với tham số
    Position(int r, int c) : row(r), col(c) {}
    
    /**
     * Kiểm tra vị trí có hợp lệ trong bàn cờ 8x8 không
     * @return true nếu 0 <= row,col <= 7
     */
    bool isValid() const {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }
    
    /**
     * Chuyển vị trí sang notation chuẩn (e.g., "e2", "a8")
     * @return string dạng "a1" đến "h8"
     */
    std::string toNotation() const {
        if (!isValid()) return "invalid";
        
        std::string notation;
        notation += char('a' + col);
        notation += char('8' - row);
        
        return notation;
    }
    
    /**
     * Tạo Position từ notation (e.g., "e2" -> Position(6,4))
     * @param notation: string dạng "a1" đến "h8"
     * @return Position object
     */
    static Position fromNotation(const std::string& notation) {
        if (notation.length() < 2) return Position();
        
        int col = notation[0] - 'a';
        int row = '8' - notation[1];
        
        Position pos(row, col);
        if (!pos.isValid()) return Position();
        
        return pos;
    }
    
    // Toán tử so sánh
    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
    
    bool operator!=(const Position& other) const {
        return !(*this == other);
    }
};
