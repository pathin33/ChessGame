#include "MoveGenerator.h"

/**
 * Thêm move nếu ô đích hợp lệ
 * @return true nếu ô trống (có thể tiếp tục), false nếu bị chặn
 */
bool MoveGenerator::addMoveIfValid(const Position& from, const Position& to, PieceColor color) {
    if (!to.isValid()) return false;
    
    Piece targetPiece = board.getPiece(to);
    
    // Nếu ô trống, thêm move
    if (targetPiece.isEmpty()) {
        moves.push_back(Move(from, to));
        return true; // Có thể tiếp tục
    }
    
    // Nếu là quân địch, có thể bắt
    if (targetPiece.color != color) {
        Move move(from, to);
        move.capturedPiece = targetPiece;
        moves.push_back(move);
    }
    
    return false; // Bị chặn, không thể tiếp tục
}

/**
 * Sinh moves theo một hướng cụ thể (cho Rook, Bishop, Queen)
 */
void MoveGenerator::generateSlidingMoves(const Position& from, int dirRow, int dirCol, PieceColor color) {
    Position current = from;
    
    while (true) {
        current.row += dirRow;
        current.col += dirCol;
        
        if (!addMoveIfValid(from, current, color)) {
            break; // Gặp quân cản hoặc ra ngoài bàn cờ
        }
    }
}

/**
 * Sinh nước đi cho Pawn (Tốt)
 * - Di chuyển 1 ô về phía trước (hoặc 2 ô nếu ở vị trí khởi đầu)
 * - Bắt quân chéo
 * - En passant
 * - Promotion khi đến hàng cuối
 */
void MoveGenerator::generatePawnMoves(const Position& pos, PieceColor color, const Position& enPassantTarget) {
    int direction = (color == PieceColor::WHITE) ? -1 : 1;  // White đi lên, Black đi xuống
    int startRow = (color == PieceColor::WHITE) ? 6 : 1;     // Hàng khởi đầu
    int promotionRow = (color == PieceColor::WHITE) ? 0 : 7; // Hàng phong cấp
    
    // Di chuyển 1 ô về phía trước
    Position forward(pos.row + direction, pos.col);
    if (forward.isValid() && board.getPiece(forward).isEmpty()) {
        Move move(pos, forward);
        
        // Kiểm tra promotion
        if (forward.row == promotionRow) {
            move.moveType = MoveType::PROMOTION;
            move.promotionPiece = PieceType::QUEEN; // Default, sẽ cho user chọn sau
        }
        
        moves.push_back(move);
        
        // Di chuyển 2 ô nếu ở vị trí khởi đầu
        if (pos.row == startRow) {
            Position forward2(pos.row + 2 * direction, pos.col);
            if (board.getPiece(forward2).isEmpty()) {
                moves.push_back(Move(pos, forward2));
            }
        }
    }
    
    // Bắt quân chéo (capture)
    for (int dcol : {-1, 1}) {
        Position diagonal(pos.row + direction, pos.col + dcol);
        if (diagonal.isValid()) {
            Piece targetPiece = board.getPiece(diagonal);
            
            // Bắt quân địch
            if (!targetPiece.isEmpty() && targetPiece.color != color) {
                Move move(pos, diagonal);
                move.capturedPiece = targetPiece;
                
                // Promotion
                if (diagonal.row == promotionRow) {
                    move.moveType = MoveType::PROMOTION;
                    move.promotionPiece = PieceType::QUEEN;
                }
                
                moves.push_back(move);
            }
            
            // En passant
            if (diagonal == enPassantTarget && enPassantTarget.isValid()) {
                Move move(pos, diagonal, MoveType::EN_PASSANT);
                // Quân bị bắt là pawn ở cạnh
                move.capturedPiece = Piece(PieceType::PAWN, (color == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE);
                moves.push_back(move);
            }
        }
    }
}

/**
 * Sinh nước đi cho Knight (Mã) - hình chữ L
 */
void MoveGenerator::generateKnightMoves(const Position& pos, PieceColor color) {
    // 8 vị trí có thể đi của mã
    int knightMoves[8][2] = {
        {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
        {1, -2}, {1, 2}, {2, -1}, {2, 1}
    };
    
    for (int i = 0; i < 8; i++) {
        Position to(pos.row + knightMoves[i][0], pos.col + knightMoves[i][1]);
        addMoveIfValid(pos, to, color);
    }
}

/**
 * Sinh nước đi cho Bishop (Tượng) - đi chéo
 */
void MoveGenerator::generateBishopMoves(const Position& pos, PieceColor color) {
    // 4 hướng chéo
    generateSlidingMoves(pos, -1, -1, color);  // Trái trên
    generateSlidingMoves(pos, -1, 1, color);   // Phải trên
    generateSlidingMoves(pos, 1, -1, color);   // Trái dưới
    generateSlidingMoves(pos, 1, 1, color);    // Phải dưới
}

/**
 * Sinh nước đi cho Rook (Xe) - đi ngang/dọc
 */
void MoveGenerator::generateRookMoves(const Position& pos, PieceColor color) {
    // 4 hướng ngang/dọc
    generateSlidingMoves(pos, -1, 0, color);   // Lên
    generateSlidingMoves(pos, 1, 0, color);    // Xuống
    generateSlidingMoves(pos, 0, -1, color);   // Trái
    generateSlidingMoves(pos, 0, 1, color);    // Phải
}

/**
 * Sinh nước đi cho Queen (Hậu) - kết hợp Rook + Bishop
 */
void MoveGenerator::generateQueenMoves(const Position& pos, PieceColor color) {
    generateRookMoves(pos, color);
    generateBishopMoves(pos, color);
}

/**
 * Sinh nước đi cho King (Vua) - 8 ô xung quanh
 */
void MoveGenerator::generateKingMoves(const Position& pos, PieceColor color) {
    // 8 ô xung quanh
    for (int drow = -1; drow <= 1; drow++) {
        for (int dcol = -1; dcol <= 1; dcol++) {
            if (drow == 0 && dcol == 0) continue;
            Position to(pos.row + drow, pos.col + dcol);
            addMoveIfValid(pos, to, color);
        }
    }
}

/**
 * Sinh castling moves (nhập thành)
 * Điều kiện:
 * - Vua và xe chưa di chuyển
 * - Không có quân ở giữa
 * - Vua không đang bị chiếu (kiểm tra ở GameState)
 * - Vua không đi qua ô bị chiếu (kiểm tra ở GameState)
 */
void MoveGenerator::generateCastlingMoves(
    PieceColor color,
    bool kingMoved,
    bool rookKingSideMoved,
    bool rookQueenSideMoved
) {
    if (kingMoved) return; // Vua đã di chuyển
    
    int row = (color == PieceColor::WHITE) ? 7 : 0;
    Position kingPos(row, 4);
    
    // Castling kingside (O-O)
    if (!rookKingSideMoved) {
        Position rookPos(row, 7);
        Piece rook = board.getPiece(rookPos);
        
        // Kiểm tra đường giữa vua và xe có trống không
        if (board.getPiece(Position(row, 5)).isEmpty() &&
            board.getPiece(Position(row, 6)).isEmpty() &&
            rook.type == PieceType::ROOK && rook.color == color) {
            
            Move move(kingPos, Position(row, 6), MoveType::CASTLE_KINGSIDE);
            moves.push_back(move);
        }
    }
    
    // Castling queenside (O-O-O)
    if (!rookQueenSideMoved) {
        Position rookPos(row, 0);
        Piece rook = board.getPiece(rookPos);
        
        // Kiểm tra đường giữa
        if (board.getPiece(Position(row, 1)).isEmpty() &&
            board.getPiece(Position(row, 2)).isEmpty() &&
            board.getPiece(Position(row, 3)).isEmpty() &&
            rook.type == PieceType::ROOK && rook.color == color) {
            
            Move move(kingPos, Position(row, 2), MoveType::CASTLE_QUEENSIDE);
            moves.push_back(move);
        }
    }
}

/**
 * Hàm chính: sinh tất cả pseudo-legal moves cho một bên
 */
std::vector<Move> MoveGenerator::generateMoves(
    PieceColor color,
    const Position& enPassantTarget,
    bool whiteKingMoved, bool blackKingMoved,
    bool whiteRookKingSideMoved, bool whiteRookQueenSideMoved,
    bool blackRookKingSideMoved, bool blackRookQueenSideMoved
) {
    moves.clear();
    
    // Duyệt qua toàn bộ bàn cờ (vòng lặp FOR - YÊU CẦU)
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Position pos(row, col);
            Piece piece = board.getPiece(pos);
            
            // Chỉ xét quân của bên đang đi
            if (piece.isEmpty() || piece.color != color) continue;
            
            // Sinh moves theo loại quân (SWITCH - YÊU CẦU)
            switch (piece.type) {
                case PieceType::PAWN:
                    generatePawnMoves(pos, color, enPassantTarget);
                    break;
                case PieceType::KNIGHT:
                    generateKnightMoves(pos, color);
                    break;
                case PieceType::BISHOP:
                    generateBishopMoves(pos, color);
                    break;
                case PieceType::ROOK:
                    generateRookMoves(pos, color);
                    break;
                case PieceType::QUEEN:
                    generateQueenMoves(pos, color);
                    break;
                case PieceType::KING:
                    generateKingMoves(pos, color);
                    break;
                default:
                    break;
            }
        }
    }
    
    // Thêm castling moves
    bool kingMoved = (color == PieceColor::WHITE) ? whiteKingMoved : blackKingMoved;
    bool rookKSMoved = (color == PieceColor::WHITE) ? whiteRookKingSideMoved : blackRookKingSideMoved;
    bool rookQSMoved = (color == PieceColor::WHITE) ? whiteRookQueenSideMoved : blackRookQueenSideMoved;
    
    generateCastlingMoves(color, kingMoved, rookKSMoved, rookQSMoved);
    
    return moves;
}
