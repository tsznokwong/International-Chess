#ifndef CHESSGAME_H_
#define CHESSGAME_H_

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <algorithm>
using std::fill_n;

#include <QObject>
#include <vector>

#include "Player.h"
#include "ChessPiece.h"
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Pawn.h"

#include "gamewindow.h"
#include "square.h"
#include "Snapshot.h"

class GameWindow;

class ChessGame : public QObject {
    Q_OBJECT
public:
	ChessGame();
	~ChessGame();
    void startConsoleUI(); // Console UI Entry Point.
    void startGraphicUI(); // GUI Entry Point.
    GameWindow* get_game_window() const;

    static bool isInChessBoard(int row, int col);

private:
    GameWindow* game_window; // The GUI.
    std::vector<Snapshot> snapshotList;
    //Snapshot* current;
    ChessPiece* chessBoard[8][8]; // The internal ChessBoard. Passed to all ChessPieces.
    King* whiteKing;
    King* blackKing;
    Pawn* pawnFirstMove; // For En Passant Timing Rules.
	ChessPiece* selectedChessPiece;
	bool selectedChessPieceValidMoves[8][8];
	Player currentPlayer;

	// ChessBoard State.
    void createSnapshot();
    void cleanFutureMoves();
    void moveToStep(int step);
    int step;

    King* getPlayerKing(Player player) const;
	bool hasValidMoves(Player player) const;
	void calculateValidMoves(ChessPiece* chessPiece, bool validMoves[8][8]) const; // Takes into account potential Checks and En Passant. Returns all false if chessPiece is nullptr.
    void promotePawn(Pawn* pawn); // Pawn will be promoted into either Queen, Rook, Knight, or Bishop, when it reaches the last row on the opposite side.

	// ChessGame Flow.	
	void selectChessPiece(int row, int col); // Updates selectedChessPiece and selectedChessPieceValidMoves.
	void moveSelectedChessPiece(int row, int col); // Process movement of selectedChessPiece to the specified position.
    void nextPlayerTurn(); // Will also check if Checkmate/Stalemate.

    // Graphic UI.
    void displaySelectedChessPieceValidMoves() const;
    void displayCurrentPlayerCheck() const;
    void updateCastlingGUI() const;

private slots:
    void processUserInput(int row, int col); // Connect to clicked_with_pos() signal to get coordinates of the clicked ChessBoard square.
    void snapshotPressed(QListWidgetItem *item);
    void undoPressed();
    void redoPressed();
};

#endif /* CHESSGAME_H_ */
