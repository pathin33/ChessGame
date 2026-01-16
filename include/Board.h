#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include "Position.h"
#include <string>

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
    int posToIndex(const Position& pos) const;
    
    /**
     * Chuyển đổi index 1D sang Position 2D
     * @param index: index trong mảng (0-63)
     * @return Position (row, col)
     */
    Position indexToPos(int index) const;

public:
    /**
     * Constructor - khởi tạo bàn cờ rỗng
     */
    Board();
    
    /**
     * Lấy quân cờ tại vị trí cụ thể
     * @param pos: vị trí cần lấy
     * @return Piece tại vị trí đó
     */
    Piece getPiece(const Position& pos) const;
    
    /**
     * Đặt quân cờ vào vị trí cụ thể
     * @param pos: vị trí cần đặt
     * @param piece: quân cờ cần đặt
     */
    void setPiece(const Position& pos, const Piece& piece);
    
    /**
     * Xóa toàn bộ bàn cờ (đặt tất cả ô về rỗng)
     */
    void clear();
    
    /**
     * Setup vị trí khởi đầu chuẩn của cờ vua
     */
    void setupInitialPosition();
    
    /**
     * Export bàn cờ ra FEN (Forsyth-Edwards Notation) string
     * VD: "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"
     * @return FEN string (chỉ phần board, không có turn/castling/etc)
     */
    std::string toFEN() const;
    
    /**
     * Load bàn cờ từ FEN string
     * @param fen: FEN string (chỉ phần board)
     * @return true nếu load thành công
     */
    bool fromFEN(const std::string& fen);
    
    /**
     * In bàn cờ ra console (để debug)
     */
    void print() const;
};

#endif // BOARD_H
