#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "AIPlayer.h"
#include "BoardView.h"
#include "UIView.h"
#include "MenuView.h"
#include "SaveLoadManager.h"

/**
 * Enum cho game phase (state machine)
 */
enum class GamePhase {
    MENU,           // Main menu
    MODE_SELECT,    // Chọn mode
    PLAYING,        // Đang chơi
    PROMOTION,      // Chọn quân phong cấp
    GAME_OVER       // Game kết thúc
};

/**
 * Controller chính - điều phối game flow
 * Kết nối Model và View
 */
class GameController {
private:
    GameState gameState;
    AIPlayer aiPlayer;
    BoardView boardView;
    UIView uiView;
    MenuView menuView;
    
    GamePhase currentPhase;
    GameMode gameMode;
    
    // Interaction state
    Position selectedSquare;
    bool pieceSelected;
    std::vector<Move> currentLegalMoves;
    Move pendingPromotionMove;
    
    // Menu state
    int menuSelection;
    int modeSelection;
    
    // Status message
    std::string statusMessage;
    
public:
    /**
     * Constructor
     */
    GameController() 
        : aiPlayer(3), currentPhase(GamePhase::MENU), gameMode(GameMode::PVP),
          pieceSelected(false), menuSelection(0), modeSelection(0) {
    }
    
    /**
     * Handle input events
     */
    void handleEvent(const sf::Event& event) {
        if (currentPhase == GamePhase::MENU) {
            handleMenuInput(event);
        } else if (currentPhase == GamePhase::MODE_SELECT) {
            handleModeSelectInput(event);
        } else if (currentPhase == GamePhase::PLAYING) {
            handleGameInput(event);
        } else if (currentPhase == GamePhase::PROMOTION) {
            handlePromotionInput(event);
        }
    }
    
    /**
     * Update game logic (for AI moves)
     */
    void update() {
        if (currentPhase == GamePhase::PLAYING) {
            // Nếu là AI mode và đến lượt AI (BLACK)
            if (gameMode == GameMode::PVE_AI && 
                gameState.getCurrentTurn() == PieceColor::BLACK) {
                
                // AI tính nước đi
                Move aiMove = aiPlayer.getBestMove(gameState);
                
                if (aiMove.from.isValid()) {
                    gameState.makeMove(aiMove);
                    checkGameOver();
                    
                    // Save game sau mỗi nước đi
                    SaveLoadManager::saveGame(gameState, gameMode, "public/save.txt");
                }
            }
        }
    }
    
    /**
     * Render everything
     */
    void render(sf::RenderWindow& window) {
        window.clear(sf::Color(40, 40, 40));
        
        if (currentPhase == GamePhase::MENU) {
            menuView.renderMainMenu(window, menuSelection);
        } else if (currentPhase == GamePhase::MODE_SELECT) {
            menuView.renderModeSelection(window, modeSelection);
        } else if (currentPhase == GamePhase::PLAYING || currentPhase == GamePhase::GAME_OVER) {
            boardView.render(window, gameState.getBoard());
            uiView.renderTurnIndicator(window, gameState.getCurrentTurn());
            uiView.renderStatusMessage(window, statusMessage);
            uiView.renderMoveHistory(window, gameState.getMoveHistory());
            uiView.renderCapturedPieces(window, gameState.getCapturedPieces());
        } else if (currentPhase == GamePhase::PROMOTION) {
            boardView.render(window, gameState.getBoard());
            uiView.renderPromotionDialog(window, gameState.getCurrentTurn());
        }
    }
    
private:
    /**
     * Handle menu input
     */
    void handleMenuInput(const sf::Event& event) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up) {
                menuSelection = (menuSelection - 1 + 3) % 3;
            } else if (event.key.code == sf::Keyboard::Down) {
                menuSelection = (menuSelection + 1) % 3;
            } else if (event.key.code == sf::Keyboard::Enter) {
                if (menuSelection == 0) { // New Game
                    currentPhase = GamePhase::MODE_SELECT;
                } else if (menuSelection == 1) { // Continue
                    if (SaveLoadManager::loadGame(gameState, gameMode, "public/save.txt")) {
                        currentPhase = GamePhase::PLAYING;
                    }
                } else if (menuSelection == 2) { // Exit
                    // Will be handled in main
                }
            }
        }
    }
    
    /**
     * Handle mode selection input
     */
    void handleModeSelectInput(const sf::Event& event) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up) {
                modeSelection = (modeSelection - 1 + 2) % 2;
            } else if (event.key.code == sf::Keyboard::Down) {
                modeSelection = (modeSelection + 1) % 2;
            } else if (event.key.code == sf::Keyboard::Enter) {
                gameMode = (modeSelection == 0) ? GameMode::PVP : GameMode::PVE_AI;
                startNewGame();
            } else if (event.key.code == sf::Keyboard::Escape) {
                currentPhase = GamePhase::MENU;
            }
        }
    }
    
    /**
     * Handle game input (mouse clicks)
     */
    void handleGameInput(const sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                int x = event.mouseButton.x;
                int y = event.mouseButton.y;
                
                Position clickedSquare = boardView.getSquareFromPixel(x, y);
                
                if (!clickedSquare.isValid()) return;
                
                if (!pieceSelected) {
                    // First click - select piece
                    Piece piece = gameState.getBoard().getPiece(clickedSquare);
                    
                    if (!piece.isEmpty() && piece.color == gameState.getCurrentTurn()) {
                        selectedSquare = clickedSquare;
                        pieceSelected = true;
                        
                        // Get legal moves cho quân này
                        currentLegalMoves = gameState.getLegalMoves();
                        std::vector<Move> pieceMoves;
                        
                        for (const Move& move : currentLegalMoves) {
                            if (move.from == selectedSquare) {
                                pieceMoves.push_back(move);
                            }
                        }
                        
                        boardView.setHighlight(selectedSquare);
                        boardView.showValidMoves(pieceMoves);
                    }
                } else {
                    // Second click - try to move
                    Move move(selectedSquare, clickedSquare);
                    
                    // Tìm move trong legal moves
                    bool found = false;
                    for (const Move& legalMove : currentLegalMoves) {
                        if (legalMove.from == selectedSquare && legalMove.to == clickedSquare) {
                            move = legalMove;
                            found = true;
                            break;
                        }
                    }
                    
                    if (found) {
                        // Kiểm tra promotion
                        if (move.moveType == MoveType::PROMOTION) {
                            pendingPromotionMove = move;
                            currentPhase = GamePhase::PROMOTION;
                        } else {
                            gameState.makeMove(move);
                            checkGameOver();
                            SaveLoadManager::saveGame(gameState, gameMode, "public/save.txt");
                        }
                    }
                    
                    // Reset selection
                    pieceSelected = false;
                    boardView.clearHighlight();
                }
            }
        }
    }
    
    /**
     * Handle promotion input (simple: click to select)
     */
    void handlePromotionInput(const sf::Event& event) {
        if (event.type == sf::Event::KeyPressed) {
            PieceType promoted = PieceType::QUEEN; // Default
            
            if (event.key.code == sf::Keyboard::Num1 || event.key.code == sf::Keyboard::Q) {
                promoted = PieceType::QUEEN;
            } else if (event.key.code == sf::Keyboard::Num2 || event.key.code == sf::Keyboard::R) {
                promoted = PieceType::ROOK;
            } else if (event.key.code == sf::Keyboard::Num3 || event.key.code == sf::Keyboard::B) {
                promoted = PieceType::BISHOP;
            } else if (event.key.code == sf::Keyboard::Num4 || event.key.code == sf::Keyboard::N) {
                promoted = PieceType::KNIGHT;
            } else {
                return; // Invalid key
            }
            
            pendingPromotionMove.promotionPiece = promoted;
            gameState.makeMove(pendingPromotionMove);
            
            currentPhase = GamePhase::PLAYING;
            checkGameOver();
            SaveLoadManager::saveGame(gameState, gameMode, "public/save.txt");
        }
    }
    
    /**
     * Start new game
     */
    void startNewGame() {
        gameState.reset();
        currentPhase = GamePhase::PLAYING;
        pieceSelected = false;
        statusMessage = "";
        boardView.clearHighlight();
    }
    
    /**
     * Check nếu game over (checkmate/stalemate)
     */
    void checkGameOver() {
        PieceColor currentTurn = gameState.getCurrentTurn();
        
        if (gameState.isCheckmate(currentTurn)) {
            PieceColor winner = (currentTurn == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
            statusMessage = (winner == PieceColor::WHITE) ? "White Wins!" : "Black Wins!";
            statusMessage += " (Checkmate)";
            currentPhase = GamePhase::GAME_OVER;
        } else if (gameState.isStalemate(currentTurn)) {
            statusMessage = "Stalemate! (Draw)";
            currentPhase = GamePhase::GAME_OVER;
        } else if (gameState.isInCheck(currentTurn)) {
            statusMessage = "Check!";
        } else {
            statusMessage = "";
        }
    }
    
public:
    /**
     * Getter cho menu selection (để handle exit trong main)
     */
    int getMenuSelection() const { return menuSelection; }
    GamePhase getCurrentPhase() const { return currentPhase; }
};

#endif // GAMECONTROLLER_H
