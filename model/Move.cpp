// Include Piece and Position from model layer
// Note: In a single .cpp file approach, we rely on compilation order
// Piece.cpp and Position.cpp should be compiled before this file

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
    std::string toNotation() const {
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
     * Tạo Move từ notation (ví dụ: "e2e4", "e7e8q")
     * @param notation: string notation
     * @return Move object
     */
    static Move fromNotation(const std::string& notation) {
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
    
    // So sánh hai nước đi
    bool operator==(const Move& other) const {
        return from == other.from && to == other.to && 
               moveType == other.moveType && 
               promotionPiece == other.promotionPiece;
    }
};
