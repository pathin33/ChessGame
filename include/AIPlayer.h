#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "GameState.h"
#include "Move.h"

/**
 * Class AI player sử dụng Minimax với Alpha-Beta pruning
 * Tham khảo từ example.cpp nhưng refactor theo MVC
 */
class AIPlayer {
private:
    int searchDepth;  // Độ sâu search (3 = medium difficulty)
    
    /**
     * Minimax algorithm với Alpha-Beta pruning
     * @param state: game state hiện tại
     * @param depth: độ sâu còn lại
     * @param isMaximizing: true nếu đang maximize (AI), false nếu minimize (player)
     * @param alpha, beta: bounds cho alpha-beta pruning
     * @return điểm đánh giá của position
     */
    int minimax(GameState& state, int depth, bool isMaximizing, int alpha, int beta);
    
    /**
     * Đánh giá vị trí hiện tại
     * @param state: game state
     * @return điểm đánh giá (dương = AI tốt, âm = player tốt)
     */
    int evaluatePosition(const GameState& state);

public:
    /**
     * Constructor
     * @param depth: độ sâu search (1-5, khuyến nghị 3)
     */
    AIPlayer(int depth = 3) : searchDepth(depth) {}
    
    /**
     * Lấy nước đi tốt nhất cho AI
     * @param state: game state hiện tại (AI là BLACK)
     * @return nước đi tốt nhất
     */
    Move getBestMove(GameState& state);
    
    /**
     * Set độ khó (search depth)
     */
    void setDifficulty(int depth) { searchDepth = depth; }
};

#endif // AIPLAYER_H
