#include "Board.h"
#include <iostream>
#include <sstream>

/**
 * Constructor - khởi tạo bàn cờ rỗng
 */
Board::Board() {
    clear();
}

/**
 * Chuyển Position sang index trong mảng 1D
 */
int Board::posToIndex(const Position& pos) const {
    return pos.row * 8 + pos.col;
}

/**
 * Chuyển index sang Position
 */
Position Board::indexToPos(int index) const {
    return Position(index / 8, index % 8);
}

/**
 * Lấy quân cờ tại vị trí
 */
Piece Board::getPiece(const Position& pos) const {
    if (!pos.isValid()) return Piece();
    return board[posToIndex(pos)];
}

/**
 * Đặt quân cờ vào vị trí
 */
void Board::setPiece(const Position& pos, const Piece& piece) {
    if (!pos.isValid()) return;
    board[posToIndex(pos)] = piece;
}

/**
 * Xóa toàn bộ bàn cờ
 */
void Board::clear() {
    for (int i = 0; i < 64; i++) {
        board[i] = Piece(); // Empty piece
    }
}

/**
 * Setup vị trí khởi đầu chuẩn của cờ vua
 */
void Board::setupInitialPosition() {
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
 * Export bàn cờ sang FEN notation
 */
std::string Board::toFEN() const {
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
 */
bool Board::fromFEN(const std::string& fen) {
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
void Board::print() const {
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
