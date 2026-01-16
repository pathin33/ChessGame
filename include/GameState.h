#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Board.h"
#include "Move.h"
#include "MoveGenerator.h"
#include "Position.h"
#include <vector>
#include <string>

/**
 * Class quản lý trạng thái game cờ vua
 * Bao gồm: board, turn, move history, captured pieces, castling rights, en passant
 */
class GameState {
private:
    Board board;                          // Bàn cờ hiện tại
    PieceColor currentTurn;               // Lượt đi hiện tại
    std::vector<Move> moveHistory;        // Lịch sử nước đi (YÊU CẦU)
    std::vector<Piece> capturedPieces;    // Quân bị bắt (YÊU CẦU)
    
    // Castling rights
    bool whiteKingMoved;
    bool blackKingMoved;
    bool whiteRookKingSideMoved;
    bool whiteRookQueenSideMoved;
    bool blackRookKingSideMoved;
    bool blackRookQueenSideMoved;
    
    // En passant target
    Position enPassantTarget;
    
    // Move generator
    MoveGenerator moveGenerator;
    
    /**
     * Tìm vị trí vua của một bên
     */
    Position findKing(PieceColor color) const;
    
    /**
     * Kiểm tra một ô có bị tấn công bởi bên địch không
     */
    bool isSquareAttacked(const Position& pos, PieceColor byColor) const;
    
    /**
     * Apply move lên board (không kiểm tra tính hợp lệ)
     */
    void applyMoveInternal(const Move& move);
    
    /**
     * Undo move cuối cùng
     */
    void undoMoveInternal();

public:
    /**
     * Constructor
     */
    GameState();
    
    /**
     * Reset game về trạng thái khởi đầu
     */
    void reset();
    
    /**
     * Lấy board hiện tại (const)
     */
    const Board& getBoard() const { return board; }
    
    /**
     * Lấy lượt đi hiện tại
     */
    PieceColor getCurrentTurn() const { return currentTurn; }
    
    /**
     * Lấy lịch sử nước đi
     */
    const std::vector<Move>& getMoveHistory() const { return moveHistory; }
    
    /**
     * Lấy danh sách quân bị bắt
     */
    const std::vector<Piece>& getCapturedPieces() const { return capturedPieces; }
    
    /**
     * Lấy en passant target
     */
    Position getEnPassantTarget() const { return enPassantTarget; }
    
    /**
     * Sinh tất cả legal moves cho bên đang đi
     * (Lọc ra moves khiến vua bị chiếu)
     */
    std::vector<Move> getLegalMoves();
    
    /**
     * Thực hiện nước đi
     * @param move: nước đi cần thực hiện
     * @return true nếu thành công, false nếu không hợp lệ
     */
    bool makeMove(const Move& move);
    
    /**
     * Kiểm tra vua có bị chiếu không
     * @param color: màu của bên cần kiểm tra
     * @return true nếu bị chiếu
     */
    bool isInCheck(PieceColor color) const;
    
    /**
     * Kiểm tra checkmate
     * @param color: màu của bên cần kiểm tra
     * @return true nếu bị chiếu hết
     */
    bool isCheckmate(PieceColor color) const;
    
    /**
     * Kiểm tra stalemate (hòa cờ)
     * @param color: màu của bên cần kiểm tra
     * @return true nếu hòa
     */
    bool isStalemate(PieceColor color) const;
    
    /**
     * Tính tổng giá trị material của một bên
     * (YÊU CẦU: phép tính +, ảnh hưởng đầu ra)
     * @param color: màu của bên cần tính
     * @return tổng điểm
     */
    int calculateMaterialValue(PieceColor color) const;
    
    /**
     * Load game state từ FEN string
     * Format: FEN board + turn + castling + en passant
     */
    bool loadFromFEN(const std::string& fen);
    
    /**
     * Export game state ra FEN string
     */
    std::string toFEN() const;
};

#endif // GAMESTATE_H
