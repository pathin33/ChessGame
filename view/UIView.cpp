#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>

/**
 * Class render UI elements: status, move history, captured pieces
 */
class UIView {
private:
    sf::Font font;
    const int SIDEBAR_X = 680;  // Vị trí sidebar (bên phải board)
    const int BOARD_MARGIN = 20;
    
public:
    /**
     * Constructor - load font hoặc dùng default
     */
    UIView() {
        // Load bold mono font for better visibility and pixel-like look
        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf")) {
            // Fallback to regular bold
            if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
                // Fallback to regular font
                if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
                    if (!font.loadFromFile("/usr/share/fonts/TTF/DejaVuSans.ttf")) {
                        std::cerr << "WARNING: Could not load font! Text will not render." << std::endl;
                    }
                }
            }
        }
    }
    
    /**
     * Render turn indicator (ai đang đi)
     */
    void renderTurnIndicator(sf::RenderWindow& window, PieceColor turn) {
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(26);
        text.setStyle(sf::Text::Bold);
        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(2);
        text.setPosition(SIDEBAR_X, 30);
        
        std::string turnStr = (turn == PieceColor::WHITE) ? "White's Turn" : "Black's Turn";
        text.setString(turnStr);
        
        window.draw(text);
    }
    
    /**
     * Render status message (Check, Checkmate, Stalemate)
     */
    void renderStatusMessage(sf::RenderWindow& window, const std::string& message) {
        if (message.empty()) return;
        
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(28);
        text.setStyle(sf::Text::Bold);
        text.setFillColor(sf::Color::Red);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(2);
        text.setPosition(SIDEBAR_X, 70);
        text.setString(message);
        
        window.draw(text);
    }
    
    /**
     * Render move history (lịch sử nước đi)
     */
    void renderMoveHistory(sf::RenderWindow& window, const std::vector<Move>& history) {
        sf::Text title;
        title.setFont(font);
        title.setCharacterSize(24);
        title.setStyle(sf::Text::Bold);
        title.setFillColor(sf::Color::White);
        title.setOutlineColor(sf::Color::Black);
        title.setOutlineThickness(2);
        title.setPosition(SIDEBAR_X, 120);
        title.setString("Move History:");
        window.draw(title);
        
        // Hiển thị 10 nước đi gần nhất
        int startIdx = (history.size() > 10) ? history.size() - 10 : 0;
        
        for (size_t i = startIdx; i < history.size(); i++) {
            sf::Text moveText;
            moveText.setFont(font);
            moveText.setCharacterSize(18);
            moveText.setStyle(sf::Text::Bold);
            moveText.setFillColor(sf::Color::White);
            moveText.setOutlineColor(sf::Color::Black);
            moveText.setOutlineThickness(1.5);
            moveText.setPosition(SIDEBAR_X, 155 + (i - startIdx) * 24);
            
            std::string moveStr = std::to_string(i + 1) + ". " + history[i].toNotation();
            moveText.setString(moveStr);
            
            window.draw(moveText);
        }
    }
    
    /**
     * Render captured pieces
     */
    void renderCapturedPieces(sf::RenderWindow& window, const std::vector<Piece>& captured) {
        sf::Text title;
        title.setFont(font);
        title.setCharacterSize(24);
        title.setStyle(sf::Text::Bold);
        title.setFillColor(sf::Color::White);
        title.setOutlineColor(sf::Color::Black);
        title.setOutlineThickness(2);
        title.setPosition(SIDEBAR_X, 400);
        title.setString("Captured:");
        window.draw(title);
        
        // Đếm số quân bị bắt
        int whiteCaptured = 0, blackCaptured = 0;
        for (const Piece& piece : captured) {
            if (piece.color == PieceColor::WHITE) whiteCaptured++;
            else blackCaptured++;
        }
        
        sf::Text capturedText;
        capturedText.setFont(font);
        capturedText.setCharacterSize(20);
        capturedText.setStyle(sf::Text::Bold);
        capturedText.setFillColor(sf::Color::White);
        capturedText.setOutlineColor(sf::Color::Black);
        capturedText.setOutlineThickness(1.5);
        capturedText.setPosition(SIDEBAR_X, 435);
        
        std::string capturedStr = "White: " + std::to_string(whiteCaptured) + 
                                  "\nBlack: " + std::to_string(blackCaptured);
        capturedText.setString(capturedStr);
        
        window.draw(capturedText);
    }
    
    /**
     * Render promotion dialog (chọn quân phong cấp)
     */
    void renderPromotionDialog(sf::RenderWindow& window, PieceColor color) {
        // Background overlay
        sf::RectangleShape overlay(sf::Vector2f(900, 700));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);
        
        // Dialog box
        sf::RectangleShape dialog(sf::Vector2f(400, 200));
        dialog.setPosition(250, 250);
        dialog.setFillColor(sf::Color(50, 50, 50));
        dialog.setOutlineColor(sf::Color::White);
        dialog.setOutlineThickness(2);
        window.draw(dialog);
        
        // Title
        sf::Text title;
        title.setFont(font);
        title.setCharacterSize(28);
        title.setStyle(sf::Text::Bold);
        title.setFillColor(sf::Color::White);
        title.setOutlineColor(sf::Color::Black);
        title.setOutlineThickness(2);
        title.setPosition(280, 270);
        title.setString("Choose Promotion:");
        window.draw(title);
        
        // Options: Q, R, B, N
        std::string options[4] = {"1:Queen", "2:Rook", "3:Bishop", "4:Knight"};
        for (int i = 0; i < 4; i++) {
            sf::Text option;
            option.setFont(font);
            option.setCharacterSize(20);
            option.setStyle(sf::Text::Bold);
            option.setFillColor(sf::Color::White);
            option.setOutlineColor(sf::Color::Black);
            option.setOutlineThickness(1.5);
            option.setPosition(270, 320 + i * 30);
            option.setString(options[i]);
            window.draw(option);
        }
    }
};
