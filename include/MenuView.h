#ifndef MENUVIEW_H
#define MENUVIEW_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

/**
 * Class render menu screens
 */
class MenuView {
private:
    sf::Font font;
    int selectedOption;
    
public:
    MenuView() : selectedOption(0) {
        // Load bold mono font for better visibility
        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf")) {
            if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
                if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
                    if (!font.loadFromFile("/usr/share/fonts/TTF/DejaVuSans.ttf")) {
                        std::cerr << "WARNING: Could not load font for menu!" << std::endl;
                    }
                }
            }
        }
    }
    
    /**
     * Render main menu
     * Options: New Game, Continue, Exit
     */
    void renderMainMenu(sf::RenderWindow& window, int selected) {
        selectedOption = selected;
        
        // Title
        sf::Text title;
        title.setFont(font);
        title.setCharacterSize(52);  // Larger
        title.setStyle(sf::Text::Bold);
        title.setFillColor(sf::Color::White);
        title.setOutlineColor(sf::Color::Black);
        title.setOutlineThickness(3);
        title.setPosition(230, 100);
        title.setString("CHESS GAME");
        window.draw(title);
        
        // Menu options
        std::vector<std::string> options = {"New Game", "Continue", "Exit"};
        
        for (size_t i = 0; i < options.size(); i++) {
            sf::Text optionText;
            optionText.setFont(font);
            optionText.setCharacterSize(36);  // Larger
            optionText.setStyle(sf::Text::Bold);
            
            // Highlight selected option
            if ((int)i == selectedOption) {
                optionText.setFillColor(sf::Color::Yellow);
                optionText.setOutlineColor(sf::Color::Black);
                optionText.setOutlineThickness(2.5);
            } else {
                optionText.setFillColor(sf::Color::White);
                optionText.setOutlineColor(sf::Color::Black);
                optionText.setOutlineThickness(2);
            }
            
            optionText.setPosition(280, 250 + i * 65);
            optionText.setString(options[i]);
            window.draw(optionText);
        }
        
        // Instructions
        sf::Text instructions;
        instructions.setFont(font);
        instructions.setCharacterSize(20);  // Larger
        instructions.setStyle(sf::Text::Bold);
        instructions.setFillColor(sf::Color(200, 200, 200));
        instructions.setOutlineColor(sf::Color::Black);
        instructions.setOutlineThickness(1);
        instructions.setPosition(140, 550);
        instructions.setString("Use UP/DOWN arrows to navigate, ENTER to select");
        window.draw(instructions);
    }
    
    /**
     * Render mode selection menu
     * Options: PVP, vs AI
     */
    void renderModeSelection(sf::RenderWindow& window, int selected) {
        selectedOption = selected;
        
        // Title
        sf::Text title;
        title.setFont(font);
        title.setCharacterSize(44);  // Larger
        title.setStyle(sf::Text::Bold);
        title.setFillColor(sf::Color::White);
        title.setOutlineColor(sf::Color::Black);
        title.setOutlineThickness(2.5);
        title.setPosition(240, 150);
        title.setString("Select Mode");
        window.draw(title);
        
        // Mode options
        std::vector<std::string> modes = {"Player vs Player", "Player vs Computer"};
        
        for (size_t i = 0; i < modes.size(); i++) {
            sf::Text modeText;
            modeText.setFont(font);
            modeText.setCharacterSize(36);  // Larger
            modeText.setStyle(sf::Text::Bold);
            
            if ((int)i == selectedOption) {
                modeText.setFillColor(sf::Color::Yellow);
                modeText.setOutlineColor(sf::Color::Black);
                modeText.setOutlineThickness(2.5);
            } else {
                modeText.setFillColor(sf::Color::White);
                modeText.setOutlineColor(sf::Color::Black);
                modeText.setOutlineThickness(2);
            }
            
            modeText.setPosition(220, 280 + i * 65);
            modeText.setString(modes[i]);
            window.draw(modeText);
        }
    }
};

#endif // MENUVIEW_H
