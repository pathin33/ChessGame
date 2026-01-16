#ifndef SAVELOADMANAGER_H
#define SAVELOADMANAGER_H

#include "GameState.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

/**
 * Enum cho game mode
 */
enum class GameMode {
    PVP,      // Player vs Player
    PVE_AI    // Player vs AI
};

/**
 * Class quản lý save/load game
 * Format file: MODE, FEN, HISTORY
 */
class SaveLoadManager {
public:
    /**
     * Lưu game state ra file
     * (YÊU CẦU: đọc/ghi file)
     * @param state: game state hiện tại
     * @param mode: game mode
     * @param filepath: đường dẫn file save
     * @return true nếu thành công
     */
    static bool saveGame(const GameState& state, GameMode mode, const std::string& filepath) {
        std::ofstream file(filepath);
        
        if (!file.is_open()) {
            std::cerr << "ERROR: Cannot open file for writing: " << filepath << std::endl;
            return false;
        }
        
        // Write mode
       file << "MODE:";
        if (mode == GameMode::PVP) {
            file << "PVP\n";
        } else {
            file << "AI\n";
        }
        
        // Write FEN
        file << "FEN:" << state.toFEN() << "\n";
        
        // Write move history
        file << "HISTORY:";
        const std::vector<Move>& history = state.getMoveHistory();
        for (size_t i = 0; i < history.size(); i++) {
            file << history[i].toNotation();
            if (i < history.size() - 1) {
                file << ",";
            }
        }
        file << "\n";
        
        file.close();
        std::cout << "Game saved to: " << filepath << std::endl;
        return true;
    }
    
    /**
     * Load game từ file
     * @param state: game state để fill
     * @param mode: game mode để fill
     * @param filepath: đường dẫn file load
     * @return true nếu thành công
     */
    static bool loadGame(GameState& state, GameMode& mode, const std::string& filepath) {
        std::ifstream file(filepath);
        
        if (!file.is_open()) {
            std::cerr << "ERROR: Cannot open file for reading: " << filepath << std::endl;
            return false;
        }
        
        std::string line;
        
        // Read mode
        if (std::getline(file, line)) {
            if (line.find("MODE:PVP") != std::string::npos) {
                mode = GameMode::PVP;
            } else if (line.find("MODE:AI") != std::string::npos) {
                mode = GameMode::PVE_AI;
            }
        }
        
        // Read FEN
        if (std::getline(file, line)) {
            if (line.find("FEN:") != std::string::npos) {
                std::string fen = line.substr(4); // Skip "FEN:"
                if (!state.loadFromFEN(fen)) {
                    std::cerr << "ERROR: Invalid FEN in save file" << std::endl;
                    file.close();
                    return false;
                }
            }
        }
        
        // Read history (optional, for display purposes)
        if (std::getline(file, line)) {
            // Parse history if needed
            // For now, we skip since state is already loaded from FEN
        }
        
        file.close();
        std::cout << "Game loaded from: " << filepath << std::endl;
        return true;
    }
};

#endif // SAVELOADMANAGER_H
