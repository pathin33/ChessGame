#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cstdlib>

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
    Position findKing(PieceColor color) const {
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                Position pos(row, col);
                Piece piece = board.getPiece(pos);
                
                if (piece.type == PieceType::KING && piece.color == color) {
                    return pos;
                }
            }
        }
        
        return Position();
    }
    
    /**
     * Kiểm tra một ô có bị tấn công bởi bên địch không
     */
    bool isSquareAttacked(const Position& pos, PieceColor byColor) const {
        // Check pawn attacks
        int pawnDir = (byColor == PieceColor::WHITE) ? 1 : -1;
        for (int dcol : {-1, 1}) {
            Position pawnPos(pos.row + pawnDir, pos.col + dcol);
            if (pawnPos.isValid()) {
                Piece piece = board.getPiece(pawnPos);
                if (piece.type == PieceType::PAWN && piece.color == byColor) {
                    return true;
                }
            }
        }
        
        // Check knight attacks
        int knightMoves[8][2] = {
            {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
            {1, -2}, {1, 2}, {2, -1}, {2, 1}
        };
        for (int i = 0; i < 8; i++) {
            Position knightPos(pos.row + knightMoves[i][0], pos.col + knightMoves[i][1]);
            if (knightPos.isValid()) {
                Piece piece = board.getPiece(knightPos);
                if (piece.type == PieceType::KNIGHT && piece.color == byColor) {
                    return true;
                }
            }
        }
        
        // Check king attacks
        for (int drow = -1; drow <= 1; drow++) {
            for (int dcol = -1; dcol <= 1; dcol++) {
                if (drow == 0 && dcol == 0) continue;
                Position kingPos(pos.row + drow, pos.col + dcol);
                if (kingPos.isValid()) {
                    Piece piece = board.getPiece(kingPos);
                    if (piece.type == PieceType::KING && piece.color == byColor) {
                        return true;
                    }
                }
            }
        }
        
        // Check sliding pieces (Rook, Bishop, Queen)
        int rookDirections[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (int i = 0; i < 4; i++) {
            int drow = rookDirections[i][0];
            int dcol = rookDirections[i][1];
            Position current = pos;
            
            while (true) {
                current.row += drow;
                current.col += dcol;
                if (!current.isValid()) break;
                
                Piece piece = board.getPiece(current);
                if (!piece.isEmpty()) {
                    if (piece.color == byColor && 
                        (piece.type == PieceType::ROOK || piece.type == PieceType::QUEEN)) {
                        return true;
                    }
                    break; // Bị chặn
                }
            }
        }
        
        // Check diagonal (Bishop, Queen)
        int bishopDirections[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
        for (int i = 0; i < 4; i++) {
            int drow = bishopDirections[i][0];
            int dcol = bishopDirections[i][1];
            Position current = pos;
            
            while (true) {
                current.row += drow;
                current.col += dcol;
                if (!current.isValid()) break;
                
                Piece piece = board.getPiece(current);
                if (!piece.isEmpty()) {
                    if (piece.color == byColor && 
                        (piece.type == PieceType::BISHOP || piece.type == PieceType::QUEEN)) {
                        return true;
                    }
                    break;
                }
            }
        }
        
        return false;
    }
    
    /**
     * Apply move lên board (không kiểm tra tính hợp lệ)
     */
    void applyMoveInternal(const Move& move) {
        Piece movingPiece = board.getPiece(move.from);
        
        // Handle castling
        if (move.moveType == MoveType::CASTLE_KINGSIDE) {
            board.setPiece(move.to, movingPiece);
            board.setPiece(move.from, Piece());
            
            int row = move.from.row;
            Piece rook = board.getPiece(Position(row, 7));
            board.setPiece(Position(row, 5), rook);
            board.setPiece(Position(row, 7), Piece());
            return;
        }
        
        if (move.moveType == MoveType::CASTLE_QUEENSIDE) {
            board.setPiece(move.to, movingPiece);
            board.setPiece(move.from, Piece());
            
            int row = move.from.row;
            Piece rook = board.getPiece(Position(row, 0));
            board.setPiece(Position(row, 3), rook);
            board.setPiece(Position(row, 0), Piece());
            return;
        }
        
        // Handle en passant
        if (move.moveType == MoveType::EN_PASSANT) {
            board.setPiece(move.to, movingPiece);
            board.setPiece(move.from, Piece());
            
            int capturedRow = move.from.row;
            int capturedCol = move.to.col;
            board.setPiece(Position(capturedRow, capturedCol), Piece());
            return;
        }
        
        // Xử lý promotion
        if (move.moveType == MoveType::PROMOTION) {
            Piece promotedPiece(move.promotionPiece, movingPiece.color);
            board.setPiece(move.to, promotedPiece);
            board.setPiece(move.from, Piece());
            return;
        }
        
        // Normal move
        board.setPiece(move.to, movingPiece);
        board.setPiece(move.from, Piece());
    }

public:
    /**
     * Constructor
     */
    GameState() : moveGenerator(board) {
        reset();
    }
    
    /**
     * Reset game về trạng thái khởi đầu
     */
    void reset() {
        board.setupInitialPosition();
        currentTurn = PieceColor::WHITE;
        moveHistory.clear();
        capturedPieces.clear();
        
        // Reset castling rights
        whiteKingMoved = false;
        blackKingMoved = false;
        whiteRookKingSideMoved = false;
        whiteRookQueenSideMoved = false;
        blackRookKingSideMoved = false;
        blackRookQueenSideMoved = false;
        
        // Reset en passant
        enPassantTarget = Position(); // Invalid position
    }
    
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
    std::vector<Move> getLegalMoves() {
        // Sinh pseudo-legal moves
        std::vector<Move> pseudoMoves = moveGenerator.generateMoves(
            currentTurn, enPassantTarget,
            whiteKingMoved, blackKingMoved,
            whiteRookKingSideMoved, whiteRookQueenSideMoved,
            blackRookKingSideMoved, blackRookQueenSideMoved
        );
        
        std::vector<Move> legalMoves;
        
        // Lọc ra moves không khiến vua bị chiếu
        for (const Move& move : pseudoMoves) {
            // Thử move
            Piece capturedPiece = board.getPiece(move.to);
            applyMoveInternal(move);
            
            // Kiểm tra vua có bị chiếu không
            bool inCheck = isInCheck(currentTurn);
            
            // Undo move
            board.setPiece(move.from, board.getPiece(move.to));
            board.setPiece(move.to, capturedPiece);
            
            // Undo castling
            if (move.moveType == MoveType::CASTLE_KINGSIDE) {
                int row = move.from.row;
                board.setPiece(Position(row, 7), board.getPiece(Position(row, 5)));
                board.setPiece(Position(row, 5), Piece());
            } else if (move.moveType == MoveType::CASTLE_QUEENSIDE) {
                int row = move.from.row;
                board.setPiece(Position(row, 0), board.getPiece(Position(row, 3)));
                board.setPiece(Position(row, 3), Piece());
            } else if (move.moveType == MoveType::EN_PASSANT) {
                int capturedRow = move.from.row;
                board.setPiece(Position(capturedRow, move.to.col), move.capturedPiece);
            }
            
            // Nếu không bị chiếu, move hợp lệ
            if (!inCheck) {
                // Kiểm tra thêm cho castling: không đi qua ô bị chiếu
                if (move.moveType == MoveType::CASTLE_KINGSIDE || 
                    move.moveType == MoveType::CASTLE_QUEENSIDE) {
                    // Vua không được đang bị chiếu
                    if (isInCheck(currentTurn)) continue;
                    
                    // Kiểm tra ô giữa không bị tấn công
                    int row = move.from.row;
                    PieceColor enemyColor = (currentTurn == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
                    
                    if (move.moveType == MoveType::CASTLE_KINGSIDE) {
                        if (isSquareAttacked(Position(row, 5), enemyColor)) continue;
                    } else {
                        if (isSquareAttacked(Position(row, 3), enemyColor)) continue;
                    }
                }
                
                legalMoves.push_back(move);
            }
        }
        
        return legalMoves;
    }
    
    /**
     * Thực hiện nước đi
     * @param move: nước đi cần thực hiện
     * @return true nếu thành công, false nếu không hợp lệ
     */
    bool makeMove(const Move& move) {
        // Kiểm tra move có hợp lệ không
        std::vector<Move> legalMoves = getLegalMoves();
        bool isLegal = false;
        
        for (const Move& legalMove : legalMoves) {
            if (legalMove.from == move.from && legalMove.to == move.to) {
                isLegal = true;
                break;
            }
        }
        
        if (!isLegal) return false;
        
        // Lưu quân bị bắt
        Piece capturedPiece = board.getPiece(move.to);
        if (!capturedPiece.isEmpty()) {
            capturedPieces.push_back(capturedPiece);
        }
        
        // Lưu lại nếu là en passant
        if (move.moveType == MoveType::EN_PASSANT && !move.capturedPiece.isEmpty()) {
            capturedPieces.push_back(move.capturedPiece);
        }
        
        // Apply move
        Piece movingPiece = board.getPiece(move.from);
        applyMoveInternal(move);
        
        // Cập nhật castling rights
        if (movingPiece.type == PieceType::KING) {
            if (movingPiece.color == PieceColor::WHITE) whiteKingMoved = true;
            else blackKingMoved = true;
        }
        if (movingPiece.type == PieceType::ROOK) {
            if (movingPiece.color == PieceColor::WHITE) {
                if (move.from == Position(7, 0)) whiteRookQueenSideMoved = true;
                if (move.from == Position(7, 7)) whiteRookKingSideMoved = true;
            } else {
                if (move.from == Position(0, 0)) blackRookQueenSideMoved = true;
                if (move.from == Position(0, 7)) blackRookKingSideMoved = true;
            }
        }
        
        // Cập nhật en passant target
        if (movingPiece.type == PieceType::PAWN) {
            int rowDiff = abs(move.to.row - move.from.row);
            if (rowDiff == 2) {
                // Pawn di chuyển 2 ô, set en passant target
                int targetRow = (move.from.row + move.to.row) / 2;
                enPassantTarget = Position(targetRow, move.from.col);
            } else {
                enPassantTarget = Position(); // Invalid
            }
        } else {
            enPassantTarget = Position();
        }
        
        // Thêm vào history
        moveHistory.push_back(move);
        
        // Đổi lượt
        currentTurn = (currentTurn == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
        
        return true;
    }
    
    /**
     * Kiểm tra vua có bị chiếu không
     * @param color: màu của bên cần kiểm tra
     * @return true nếu bị chiếu
     */
    bool isInCheck(PieceColor color) const {
        Position kingPos = findKing(color);
        if (!kingPos.isValid()) return false;
        
        PieceColor enemyColor = (color == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
        return isSquareAttacked(kingPos, enemyColor);
    }
    
    /**
     * Kiểm tra checkmate
     * @param color: màu của bên cần kiểm tra
     * @return true nếu bị chiếu hết
     */
    bool isCheckmate(PieceColor color) const {
        // Phải đang bị chiếu VÀ không có nước đi hợp lệ
        if (!isInCheck(color)) return false;
        
        // Tạm thời thay đổi turn để getLegalMoves
        GameState* mutableThis = const_cast<GameState*>(this);
        PieceColor savedTurn = mutableThis->currentTurn;
        mutableThis->currentTurn = color;
        std::vector<Move> legalMoves = mutableThis->getLegalMoves();
        mutableThis->currentTurn = savedTurn;
        
        return legalMoves.empty();
    }
    
    /**
     * Kiểm tra stalemate (hòa cờ)
     * @param color: màu của bên cần kiểm tra
     * @return true nếu hòa
     */
    bool isStalemate(PieceColor color) const {
        // KHÔNG bị chiếu NHƯNG không có nước đi hợp lệ
        if (isInCheck(color)) return false;
        
        // Tạm thời thay đổi turn để getLegalMoves
        GameState* mutableThis = const_cast<GameState*>(this);
        PieceColor savedTurn = mutableThis->currentTurn;
        mutableThis->currentTurn = color;
        std::vector<Move> legalMoves = mutableThis->getLegalMoves();
        mutableThis->currentTurn = savedTurn;
        
        return legalMoves.empty();
    }
    
    /**
     * Tính tổng giá trị material của một bên
     * (YÊU CẦU: phép tính +, ảnh hưởng đầu ra)
     * @param color: màu của bên cần tính
     * @return tổng điểm
     */
    int calculateMaterialValue(PieceColor color) const {
        int total = 0;
        
        // Duyệt qua bàn cờ, cộng giá trị quân của bên này
        for (int i = 0; i < 64; i++) {
            Position pos(i / 8, i % 8);
            Piece piece = board.getPiece(pos);
            
            if (piece.color == color) {
                total += piece.value;  // Phép cộng (+) - YÊU CẦU
            }
        }
        
        return total;
    }
    
    /**
     * Load game state từ FEN string
     * Format: FEN board + turn + castling + en passant
     */
    bool loadFromFEN(const std::string& fen) {
        std::istringstream iss(fen);
        std::string boardFEN, turnStr, castlingStr, enPassantStr;
        
        iss >> boardFEN >> turnStr >> castlingStr >> enPassantStr;
        
        // Load board
        if (!board.fromFEN(boardFEN)) return false;
        
        // Load turn
        currentTurn = (turnStr == "w") ? PieceColor::WHITE : PieceColor::BLACK;
        
        // Load castling rights
        whiteKingMoved = (castlingStr.find('K') == std::string::npos && castlingStr.find('Q') == std::string::npos);
        blackKingMoved = (castlingStr.find('k') == std::string::npos && castlingStr.find('q') == std::string::npos);
        whiteRookKingSideMoved = (castlingStr.find('K') == std::string::npos);
        whiteRookQueenSideMoved = (castlingStr.find('Q') == std::string::npos);
        blackRookKingSideMoved = (castlingStr.find('k') == std::string::npos);
        blackRookQueenSideMoved = (castlingStr.find('q') == std::string::npos);
        
        // Load en passant
        if (enPassantStr != "-") {
            enPassantTarget = Position::fromNotation(enPassantStr);
        } else {
            enPassantTarget = Position();
        }
        
        return true;
    }
    
    /**
     * Export game state ra FEN string
     */
    std::string toFEN() const {
        std::string fen = board.toFEN();
        
        // Thêm turn
        fen += " ";
        fen += (currentTurn == PieceColor::WHITE) ? "w" : "b";
        
        // Thêm castling rights
        fen += " ";
        std::string castling;
        if (!whiteKingMoved) {
            if (!whiteRookKingSideMoved) castling += "K";
            if (!whiteRookQueenSideMoved) castling += "Q";
        }
        if (!blackKingMoved) {
            if (!blackRookKingSideMoved) castling += "k";
            if (!blackRookQueenSideMoved) castling += "q";
        }
        if (castling.empty()) castling = "-";
        fen += castling;
        
        // Thêm en passant
        fen += " ";
        if (enPassantTarget.isValid()) {
            fen += enPassantTarget.toNotation();
        } else {
            fen += "-";
        }
        
        return fen;
    }
};
