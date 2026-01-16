#include "AIPlayer.h"
#include <algorithm>
#include <climits>

/**
 * Đánh giá vị trí hiện tại dựa trên material value
 */
int AIPlayer::evaluatePosition(const GameState& state) {
    int blackMaterial = state.calculateMaterialValue(PieceColor::BLACK);
    int whiteMaterial = state.calculateMaterialValue(PieceColor::WHITE);
    
    int evaluation = blackMaterial - whiteMaterial;
    
    // Bonus/penalty cho game over
    if (state.isCheckmate(PieceColor::WHITE)) {
        evaluation += 10000;
    } else if (state.isCheckmate(PieceColor::BLACK)) {
        evaluation -= 10000;
    }
    
    if (state.isStalemate(PieceColor::WHITE) || state.isStalemate(PieceColor::BLACK)) {
        evaluation = 0;
    }
    
    return evaluation;
}

/**
 * Minimax với Alpha-Beta pruning
 */
int AIPlayer::minimax(GameState& state, int depth, bool isMaximizing, int alpha, int beta) {
    if (depth == 0) {
        return evaluatePosition(state);
    }
    
    std::vector<Move> moves = state.getLegalMoves();
    
    if (moves.empty()) {
        return evaluatePosition(state);
    }
    
    if (isMaximizing) {
        int maxEval = INT_MIN;
        
        for (const Move& move : moves) {
            GameState tempState = state;
            tempState.makeMove(move);
            
            int eval = minimax(tempState, depth - 1, false, alpha, beta);
            
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            
            if (beta <= alpha) {
                break;
            }
        }
        
        return maxEval;
        
    } else {
        int minEval = INT_MAX;
        
        for (const Move& move : moves) {
            GameState tempState = state;
            tempState.makeMove(move);
            
            int eval = minimax(tempState, depth - 1, true, alpha, beta);
            
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            
            if (beta <= alpha) {
                break;
            }
        }
        
        return minEval;
    }
}

/**
 * Lấy nước đi tốt nhất cho AI
 */
Move AIPlayer::getBestMove(GameState& state) {
    std::vector<Move> moves = state.getLegalMoves();
    
    if (moves.empty()) {
        return Move();
    }
    
    Move bestMove = moves[0];
    int bestScore = INT_MIN;
    int alpha = INT_MIN;
    int beta = INT_MAX;
    
    for (const Move& move : moves) {
        GameState tempState = state;
        tempState.makeMove(move);
        
        int score = minimax(tempState, searchDepth - 1, false, alpha, beta);
        
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
        
        alpha = std::max(alpha, bestScore);
    }
    
    return bestMove;
}
