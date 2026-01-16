#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Position.h"
#include "AssetManager.h"
#include <vector>

/**
 * Class render bàn cờ và quân cờ
 * Tách biệt logic và view (MVC pattern)
 */
class BoardView {
private:
    const int SQUARE_SIZE = 80;        // Kích thước mỗi ô (pixel)
    const int BOARD_SIZE = 640;        // 8 * 80
    const int MARGIN = 20;             // Margin xung quanh bàn cờ
    
    // Highlight states
    Position highlightedSquare;
    std::vector<Position> validMoveSquares;
    
    /**
     * Lấy vị trí pixel từ vị trí bàn cờ
     */
    sf::Vector2f getPixelPosition(const Position& pos) const {
        return sf::Vector2f(
            MARGIN + pos.col * SQUARE_SIZE,
            MARGIN + pos.row * SQUARE_SIZE
        );
    }
    
    /**
     * Lấy filename cho piece từ asset folder
     */
    std::string getPieceFilename(const Piece& piece) const {
        if (piece.isEmpty()) return "";
        
        std::string prefix = (piece.color == PieceColor::WHITE) ? "W_" : "B_";
        std::string pieceName = "";
        
        switch (piece.type) {
            case PieceType::PAWN:   pieceName = "Pawn"; break;
            case PieceType::ROOK:   pieceName = "Rook"; break;
            case PieceType::KNIGHT: pieceName = "Knight"; break;
            case PieceType::BISHOP: pieceName = "Bishop"; break;
            case PieceType::QUEEN:  pieceName = "Queen"; break;
            case PieceType::KING:   pieceName = "King"; break;
            default: return "";
        }
        
        return "asset/16x32 pieces/" + prefix + pieceName + ".png";
    }

public:
    BoardView() {}
    
    /**
     * Draw board với màu classic
     */
    void drawBoard(sf::RenderWindow& window) {
        sf::Color lightSquare(230, 234, 215);
        sf::Color darkSquare(69, 77, 95);
        
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                sf::RectangleShape square(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
                
                bool isLight = (row + col) % 2 == 0;
                square.setFillColor(isLight ? lightSquare : darkSquare);
                
                square.setPosition(MARGIN + col * SQUARE_SIZE, MARGIN + row * SQUARE_SIZE);
                window.draw(square);
            }
        }
    }
    
    /**
     * Render bàn cờ và quân cờ
     */
    void render(sf::RenderWindow& window, const Board& board) {
        drawBoard(window);
        
        // Draw selected square highlight
        if (highlightedSquare.isValid()) {
            sf::RectangleShape highlight(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
            highlight.setPosition(getPixelPosition(highlightedSquare));
            highlight.setFillColor(sf::Color(255, 255, 0, 100)); // Vàng semi-transparent
            highlight.setOutlineColor(sf::Color::Yellow);
            highlight.setOutlineThickness(3);
            highlight.setOutlineColor(sf::Color::Yellow);
            highlight.setOutlineThickness(3);
            window.draw(highlight);
        }
        
        // Draw valid move indicators
        for (const Position& pos : validMoveSquares) {
            sf::CircleShape dot(12);
            sf::Vector2f pixelPos = getPixelPosition(pos);
            
            dot.setPosition(pixelPos.x + SQUARE_SIZE/2 - 12, pixelPos.y + SQUARE_SIZE/2 - 12);
            dot.setFillColor(sf::Color(100, 100, 100, 180));
            dot.setOutlineColor(sf::Color::White);
            dot.setOutlineThickness(2);
            
            window.draw(dot);
        }
        
        // Draw pieces
        AssetManager* assetMgr = AssetManager::getInstance();
        
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                Position pos(row, col);
                Piece piece = board.getPiece(pos);
                
                if (!piece.isEmpty()) {
                    std::string filename = getPieceFilename(piece);
                    if (filename.empty()) continue;
                    
                    sf::Texture& pieceTexture = assetMgr->getTexture(filename, filename);
                    sf::Sprite pieceSprite(pieceTexture);
                    
                    float uniformScale = (float)SQUARE_SIZE / 32.0f;
                    pieceSprite.setScale(uniformScale, uniformScale);
                    
                    sf::Vector2f pixelPos = getPixelPosition(pos);
                    float offsetX = (SQUARE_SIZE - 16.0f * uniformScale) / 2.0f;
                    pieceSprite.setPosition(pixelPos.x + offsetX, pixelPos.y);
                    
                    window.draw(pieceSprite);
                }
            }
        }
    }
    
    /**
     * Set ô được highlight
     */
    void setHighlight(const Position& pos) {
        highlightedSquare = pos;
    }
    
    /**
     * Clear highlight
     */
    void clearHighlight() {
        highlightedSquare = Position();
        validMoveSquares.clear();
    }
    
    /**
     * Hiển thị các nước đi hợp lệ
     */
    void showValidMoves(const std::vector<Move>& moves) {
        validMoveSquares.clear();
        for (const Move& move : moves) {
            validMoveSquares.push_back(move.to);
        }
    }
    
    /**
     * Chuyển click position (pixel) sang board position
     */
    Position getSquareFromPixel(int x, int y) const {
        int row = (y - MARGIN) / SQUARE_SIZE;
        int col = (x - MARGIN) / SQUARE_SIZE;
        
        Position pos(row, col);
        return pos.isValid() ? pos : Position();
    }
    
    int getMargin() const { return MARGIN; }
    int getBoardSize() const { return BOARD_SIZE; }
};

#endif // BOARDVIEW_H
