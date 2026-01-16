#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <iostream>

/**
 * Singleton class quản lý tài nguyên
 */
class AssetManager {
private:
    static AssetManager* instance;
    std::map<std::string, sf::Texture> textures;
    std::map<std::string, sf::Font> fonts;
    
    AssetManager() {}

public:
    /**
     * Lấy instance duy nhất
     */
    static AssetManager* getInstance() {
        if (!instance) {
            instance = new AssetManager();
        }
        return instance;
    }
    
    /**
     * Load và cache texture
     */
    sf::Texture& getTexture(const std::string& name, const std::string& filepath) {
        if (textures.find(name) != textures.end()) {
            return textures[name];
        }
        
        sf::Texture texture;
        if (!texture.loadFromFile(filepath)) {
            std::cerr << "ERROR: Failed to load texture: " << filepath << std::endl;
        } else {
            std::cout << "Loaded texture: " << filepath << std::endl;
        }
        
        textures[name] = texture;
        return textures[name];
    }
    
    /**
     * Load và cache font
     */
    sf::Font& getFont(const std::string& name, const std::string& filepath) {
        if (fonts.find(name) != fonts.end()) {
            return fonts[name];
        }
        
        sf::Font font;
        if (!font.loadFromFile(filepath)) {
            std::cerr << "ERROR: Failed to load font: " << filepath << std::endl;
        } else {
            std::cout << "Loaded font: " << filepath << std::endl;
        }
        
        fonts[name] = font;
        return fonts[name];
    }
    
    ~AssetManager() {
        textures.clear();
        fonts.clear();
    }
};

AssetManager* AssetManager::instance = nullptr;

#endif // ASSETMANAGER_H
