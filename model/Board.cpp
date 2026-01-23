#include <string>
#include <iostream>

/**
 * Class đại diện cho bàn cờ vua 8x8
 * Sử dụng mảng 1D để lưu trạng thái (ĐÁP ỨNG YÊU CẦU mảng 1D)
 */
class Board {
private:
    Piece board[64];  // Mảng 1D chứa 64 ô bàn cờ (YÊU CẦU BẮT BUỘC)
    
    /**
     * Chuyển đổi Position 2D sang index 1D
     * @param pos: vị trí (row, col)
     * @return index trong mảng 1D (0-63)
     */
    int posToIndex(const Position& pos) const {
        return pos.row * 8 + pos.col;
    }
    
    /**
     * Chuyển đổi index 1D sang Position 2D
     * @param index: index trong mảng (0-63)
     * @return Position (row, col)
     */
    Position indexToPos(int index) const {
        return Position(index / 8, index % 8);
    }

public:
    /**
     * Constructor - khởi tạo bàn cờ rỗng
     */
    Board() {
        clear();
    }
    
    /**
     * Lấy quân cờ tại vị trí cụ thể
     * @param pos: vị trí cần lấy
     * @return Piece tại vị trí đó
     */
    Piece getPiece(const Position& pos) const {
        if (!pos.isValid()) return Piece();
        return board[posToIndex(pos)];
    }
    
    /**
     * Đặt quân cờ vào vị trí cụ thể
     * @param pos: vị trí cần đặt
     * @param piece: quân cờ cần đặt
     */
    void setPiece(const Position& pos, const Piece& piece) {
        if (!pos.isValid()) return;
        board[posToIndex(pos)] = piece;
    }
    
    /**
     * Xóa toàn bộ bàn cờ (đặt tất cả ô về rỗng)
     */
    void clear() {
        for (int i = 0; i < 64; i++) {
            board[i] = Piece(); // Empty piece
        }
    }
    
    /**
     * Setup vị trí khởi đầu chuẩn của cờ vua
     */
    void setupInitialPosition() {
        clear();
        
        // Black pieces (row 0)
        setPiece(Position(0, 0), Piece(PieceType::ROOK, PieceColor::BLACK));
        setPiece(Position(0, 1), Piece(PieceType::KNIGHT, PieceColor::BLACK));
        setPiece(Position(0, 2), Piece(PieceType::BISHOP, PieceColor::BLACK));
        setPiece(Position(0, 3), Piece(PieceType::QUEEN, PieceColor::BLACK));
        setPiece(Position(0, 4), Piece(PieceType::KING, PieceColor::BLACK));
        setPiece(Position(0, 5), Piece(PieceType::BISHOP, PieceColor::BLACK));
        setPiece(Position(0, 6), Piece(PieceType::KNIGHT, PieceColor::BLACK));
        setPiece(Position(0, 7), Piece(PieceType::ROOK, PieceColor::BLACK));
        
        // Black pawns (row 1)
        for (int col = 0; col < 8; col++) {
            setPiece(Position(1, col), Piece(PieceType::PAWN, PieceColor::BLACK));
        }
        
        // White pawns (row 6)
        for (int col = 0; col < 8; col++) {
            setPiece(Position(6, col), Piece(PieceType::PAWN, PieceColor::WHITE));
        }
        
        // White pieces (row 7)
        setPiece(Position(7, 0), Piece(PieceType::ROOK, PieceColor::WHITE));
        setPiece(Position(7, 1), Piece(PieceType::KNIGHT, PieceColor::WHITE));
        setPiece(Position(7, 2), Piece(PieceType::BISHOP, PieceColor::WHITE));
        setPiece(Position(7, 3), Piece(PieceType::QUEEN, PieceColor::WHITE));
        setPiece(Position(7, 4), Piece(PieceType::KING, PieceColor::WHITE));
        setPiece(Position(7, 5), Piece(PieceType::BISHOP, PieceColor::WHITE));
        setPiece(Position(7, 6), Piece(PieceType::KNIGHT, PieceColor::WHITE));
        setPiece(Position(7, 7), Piece(PieceType::ROOK, PieceColor::WHITE));
    }
    
    /**
     * Export bàn cờ ra FEN (Forsyth-Edwards Notation) string
     * VD: "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"
     * @return FEN string (chỉ phần board, không có turn/castling/etc)
     */
    std::string toFEN() const {
        std::string fen;
        
        for (int row = 0; row < 8; row++) {
            int emptyCount = 0;
            
            for (int col = 0; col < 8; col++) {
                Piece piece = getPiece(Position(row, col));
                
                if (piece.isEmpty()) {
                    emptyCount++;
                } else {
                    if (emptyCount > 0) {
                        fen += std::to_string(emptyCount);
                        emptyCount = 0;
                    }
                    fen += pieceToChar(piece);
                }
            }
            
            if (emptyCount > 0) {
                fen += std::to_string(emptyCount);
            }
            
            if (row < 7) {
                fen += '/';
            }
        }
        
        return fen;
    }
    
    /**
     * Load bàn cờ từ FEN string
     * @param fen: FEN string (chỉ phần board)
     * @return true nếu load thành công
     */
    bool fromFEN(const std::string& fen) {
        clear();
        
        int row = 0, col = 0;
        
        for (char c : fen) {
            if (c == '/') {
                row++;
                col = 0;
            } else if (c >= '1' && c <= '8') {
                int emptyCount = c - '0';
                col += emptyCount;
            } else {
                Piece piece = charToPiece(c);
                if (!piece.isEmpty()) {
                    setPiece(Position(row, col), piece);
                }
                col++;
            }
            
            if (row >= 8 || col > 8) {
                return false;
            }
        }
        
        return true;
    }
    
    /**
     * In bàn cờ ra console (để debug)
     */
    void print() const {
        std::cout << "  a b c d e f g h\n";
        for (int row = 0; row < 8; row++) {
            std::cout << (8 - row) << " ";
            for (int col = 0; col < 8; col++) {
                Piece piece = getPiece(Position(row, col));
                char c = pieceToChar(piece);
                if (c == '.') c = '.';
                std::cout << c << " ";
            }
            std::cout << (8 - row) << "\n";
        }
        std::cout << "  a b c d e f g h\n";
    }
};
