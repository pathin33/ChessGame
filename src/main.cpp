#include <SFML/Graphics.hpp>
#include "GameController.h"
#include <iostream>

/**
 * Entry point của chương trình
 * Game cờ vua với kiến trúc MVC, hỗ trợ 2 chế độ PVP và PVE
 */
int main() {
    sf::RenderWindow window(sf::VideoMode(900, 700), "Chess Game");
    window.setFramerateLimit(60);
    
    // Set window icon
    sf::Image icon;
    if (icon.loadFromFile("asset/logo/logoGame.png")) {
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }
    
    GameController controller;
    
    std::cout << "Chess Game Started!\n";
    std::cout << "Controls:\n";
    std::cout << "- Menu: UP/DOWN arrows, ENTER\n";
    std::cout << "- Game: Click to select and move\n";
    std::cout << "- Promotion: 1(Q), 2(R), 3(B), 4(N)\n\n";
    
    // Main game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            controller.handleEvent(event);
            
            // Exit from menu
            if (controller.getCurrentPhase() == GamePhase::MENU && 
                controller.getMenuSelection() == 2 &&
                event.type == sf::Event::KeyPressed && 
                event.key.code == sf::Keyboard::Enter) {
                window.close();
            }
        }
        
        // Update logic và AI moves
        controller.update();
        
        // Render
        controller.render(window);
        window.display();
    }
    
    return 0;
}
