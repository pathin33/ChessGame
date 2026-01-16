#ifndef MOVEGENERATOR_H
#define MOVEGENERATOR_H

#include "Board.h"
#include "Move.h"
#include "Position.h"
#include <vector>

/**
 * Class sinh ra tất cả các nước đi hợp lệ (pseudo-legal moves)
 * Không kiểm tra check - chỉ sinh moves theo luật di chuyển cơ bản
 */
class MoveGenerator {
private:
    Board& board;
    std::vector<Move> moves;  // Danh sách moves được sinh ra
    
    /**
     * Các hàm sinh nước đi cho từng loại quân
     */
    void generatePawnMoves(const Position& pos, PieceColor color, const Position& enPassantTarget);
    void generateKnightMoves(const Position& pos, PieceColor color);
    void generateBishopMoves(const Position& pos, PieceColor color);
    void generateRookMoves(const Position& pos, PieceColor color);
    void generateQueenMoves(const Position& pos, PieceColor color);
    void generateKingMoves(const Position& pos, PieceColor color);
    
    /**
     * Helper function: thêm move nếu ô đích hợp lệ
     * @return true nếu có thể tiếp tục (ô trống), false nếu bị chặn
     */
    bool addMoveIfValid(const Position& from, const Position& to, PieceColor color);
    
    /**
     * Helper function: sinh moves theo hướng (cho sliding pieces)
     * @param from: vị trí xuất phát
     * @param dirRow, dirCol: hướng di chuyển (-1, 0, 1)
     * @param color: màu quân đang di chuyển
     */
    void generateSlidingMoves(const Position& from, int dirRow, int dirCol, PieceColor color);

public:
    /**
     * Constructor
     */
    MoveGenerator(Board& b) : board(b) {}
    
    /**
     * Sinh tất cả pseudo-legal moves cho một bên
     * @param color: màu của bên cần sinh moves
     * @param enPassantTarget: vị trí en passant (nếu có)
     * @param whiteKingMoved, blackKingMoved: đã di chuyển vua chưa (cho castling)
     * @param whiteRookKingSideMoved, etc: đã di chuyển xe chưa (cho castling)
     * @return vector of moves
     */
    std::vector<Move> generateMoves(
        PieceColor color,
        const Position& enPassantTarget,
        bool whiteKingMoved, bool blackKingMoved,
        bool whiteRookKingSideMoved, bool whiteRookQueenSideMoved,
        bool blackRookKingSideMoved, bool blackRookQueenSideMoved
    );
    
    /**
     * Sinh castling moves nếu hợp lệ
     */
    void generateCastlingMoves(
        PieceColor color,
        bool kingMoved,
        bool rookKingSideMoved,
        bool rookQueenSideMoved
    );
};

#endif // MOVEGENERATOR_H
